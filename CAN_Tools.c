//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
// CAN Tools
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
#include "environ.h"
#include "J1939.h"

ErrorCodeIntType loadCANData(pgnEnumType, pgnDataType *);
ErrorCodeIntType decodeCANRxMessage(CANRxMessageBuffer *);

// Private Functions & data
static ErrorCodeIntType _open(void);
static ErrorCodeIntType _read(void);
static ErrorCodeIntType _write(U8, spnEnumType, dataUnionType);
static ErrorCodeIntType _writeMST(U8, spnEnumType, U8 *);
static void vCANProtocolTask( void *);
static ErrorCodeIntType _sendPGN(pgnEnumType);
static boolean updateTxTimeAndTxIfTRUE(pgnEnumType, U16);
static ErrorCodeIntType _sendMessage(CANTxMessageBuffer *);
#ifdef USE_PRODUCTION_TEST_HW
	static void _updateActivityCounters(void);
	static void _monitorReset(U8);
#endif

// Private Data - volatile so the optimization won't mess with it
static volatile BYTE CANMessageFifoArea[2 * 8 * 16];

// Public Data
#ifdef USE_PRODUCTION_TEST_HW
	const monitorType monitor = MONITOR_DEFAULTS;
#endif
const canType myCAN = CAN_STRUCT_DEFAULTS;

//-------------------------------------------------------------------------------------------------
// CAN Protocol Task
//-------------------------------------------------------------------------------------------------
static void vCANProtocolTask( void *pvParameters ) {
	#ifndef PRODUCTION_TEST
		thermocoupleEnumType thermocoupleIndex;
	#endif
	U16 temp16 = 0;
	U16 pgnTableIndex = 0;
	ErrorCodeIntType result = 0;
	static operationalStateType operationalState_z = OS_INITIAL_STATE;
	static int resetCounter = 0;
	
	INTEnable(INT_SOURCE_CAN(MY_CAN_MODULE), INT_DISABLED);

	// The granularity of timing of this loop must be higher than the granularity of the updateRates in the pgnTable.
	while (TRUE) {
		if (systemData.operationalState != OS_NORMAL_OPERATION) {
			vTaskDelay(100);
			canData.startupTimer_ms = 0;
		} else {
			// At the transition, clear all pending interrupts and data
#define RESTART_CAN_AFTER_TIMEOUT
#ifdef RESTART_CAN_AFTER_TIMEOUT
			if ((canData.startupTimer_ms >= CAN_STARTUP_TIME_MS) &&
				(operationalState_z != systemData.operationalState))  {
				systemData.interruptCounter.canRx = 123+resetCounter;
				systemData.interruptCounter.canTx = 456+resetCounter;
				resetCounter++;
#else
			if (FALSE)  {
#endif
				operationalState_z = systemData.operationalState;

				canData.rx.timeSinceLastRx_ms = 0;
				pgnTableIndex = 0;
				result = 0;
				
				#define RESET_CAN_HERE
				#ifdef RESET_CAN_HERE
					// This isn't necessary
					CANSetOperatingMode(MY_CAN_MODULE, CAN_CONFIGURATION);
					while (CANGetOperatingMode(MY_CAN_MODULE) != CAN_CONFIGURATION);
					
					CANEnableModule(MY_CAN_MODULE,FALSE);
					myCAN.open();
					
					CANSetOperatingMode(MY_CAN_MODULE, CAN_CONFIGURATION);
					while (CANGetOperatingMode(MY_CAN_MODULE) != CAN_CONFIGURATION);

					CANEnableModule(MY_CAN_MODULE,FALSE);
					CANEnableModule(MY_CAN_MODULE,TRUE);
					CANSetOperatingMode(MY_CAN_MODULE, CAN_NORMAL_OPERATION);
					while (CANGetOperatingMode(MY_CAN_MODULE) != CAN_NORMAL_OPERATION);
				#endif

				CANAbortPendingTx(MY_CAN_MODULE, CAN_ALL_CHANNELS);
				INTClearFlag(INT_SOURCE_CAN(MY_CAN_MODULE));
				INTEnable(INT_SOURCE_CAN(MY_CAN_MODULE), INT_ENABLED);
			}
			
			// If this module is stuck in Busy mode, restart it
			temp16 = 0;
			do {
				if (C1CONbits.CANBUSY) {
					ONE_MS_DELAY;
					temp16++;
				} else {
					// Get out of do/while loop
					break;
				}
				#ifdef RESET_CAN_HERE
					// This isn't necessary
					CANSetOperatingMode(MY_CAN_MODULE, CAN_CONFIGURATION);
					while (CANGetOperatingMode(MY_CAN_MODULE) != CAN_CONFIGURATION);

					CANEnableModule(MY_CAN_MODULE,FALSE);
					myCAN.open();

					CANSetOperatingMode(MY_CAN_MODULE, CAN_CONFIGURATION);
					while (CANGetOperatingMode(MY_CAN_MODULE) != CAN_CONFIGURATION);

					CANEnableModule(MY_CAN_MODULE,FALSE);
					CANEnableModule(MY_CAN_MODULE,TRUE);
					CANSetOperatingMode(MY_CAN_MODULE, CAN_NORMAL_OPERATION);
					while (CANGetOperatingMode(MY_CAN_MODULE) != CAN_NORMAL_OPERATION);
				#endif
			} while (temp16 < 5);
			
			// Keep the RS line low while CAN is active
			SET_CAN_RS_TRIS_OUTPUT;
			CAN_RS = LOW;

			// Loop through pgn table
			VALIDATE_PGN_TABLE_INDEX(pgnTableIndex);

			#ifdef __DEBUG
				// Catch a terrible compiler error! MAX_PGNS_SUPPORTED_PER_MODULE needs to be increased if this happens.
				if (NUMBER_OF_PGNS_SUPPORTED_BY_THIS_MODULE >= MAX_PGNS_SUPPORTED_PER_MODULE) {
					HangForever_BlinkIDCode();
				}
			#endif

			if (pgnTableIndex>=NUMBER_OF_PGNS_SUPPORTED_BY_THIS_MODULE) {
				pgnTableIndex=0;
			}

			result = updateTxTimeAndTxIfTRUE(pgnTableIndex, (CAN_BLOCK_TIME_MS*NUMBER_OF_PGNS_SUPPORTED_BY_THIS_MODULE));

			if (result) {
				// Send the message
			#ifndef DONT_SEND_PGNS
				if (myCAN.sendPGN(supportedPGNS[pgnTableIndex].pgn) != PASS) {
					canData.errorCounter++;
					#ifdef RETRY_SEND_PGN
						//-------------------------------------------
						// This isn't necessary
						//-------------------------------------------
						// Delay and retry if buffer was unavailable
						//-------------------------------------------
						vTaskDelay((CAN_BLOCK_TIME_MS/2)/portTICK_RATE_MS);
						if (myCAN.sendPGN(supportedPGNS[pgnTableIndex].pgn) != PASS) {
							canData.errorCounter++;
						}
					#endif
				}
			#else
				systemData.led[LED_CAN_INDEX].state = LED_ACTIVE_COMMUNICATION_STATE;
			#endif
			} // if
			pgnTableIndex++;

			// Read from the incoming CAN data buffer
			temp16 = 0;
			do {
				result = myCAN.read();
				if (result == PASS) {
					temp16++;
				}
			} while (result == PASS);
			systemData.taskCounter.CANwait++;
			
			if (temp16 > 0) {
				canData.rx.timeSinceLastRx_ms = 0;
			} else {
				if (canData.rx.timeSinceLastRx_ms < CAN_ACTIVITY_TIMEOUT_MS) {
					canData.rx.timeSinceLastRx_ms += CAN_BLOCK_TIME_MS;
					systemData.led[LED_CAN_INDEX].state = LED_ACTIVE_COMMUNICATION_STATE;
				} else {
					#ifndef PRODUCTION_TEST
						for (thermocoupleIndex=0; thermocoupleIndex<NUMBER_OF_THERMOCOUPLES; thermocoupleIndex++) {
							operationalData.OKtoUseRemotePrePostCatTemperature[thermocoupleIndex] = FALSE;
							operationalData.exhaustTemperatureRxCounter[thermocoupleIndex] = 0;
						}
					#endif
					systemData.led[LED_CAN_INDEX].state = LED_COMMUNICATION_ERROR_STATE;
				}
			}

			#ifdef USE_PRODUCTION_TEST_HW
				_updateActivityCounters();
			#endif

			if (canData.startupTimer_ms < CAN_STARTUP_TIME_MS) {
				canData.startupTimer_ms += CAN_BLOCK_TIME_MS;
			}
			systemData.taskCounter.CAN++;
			#ifdef CHECK_STACK_HIGH_WATERMARK
				systemData.taskStack.CAN = uxTaskGetStackHighWaterMark(NULL);
			#endif
			vTaskDelay(CAN_BLOCK_TIME_MS/portTICK_RATE_MS);
		}
	}
}

//-------------------------------------------------------------------------------------------------
// Send PGN with a pgn input value
//-------------------------------------------------------------------------------------------------
static ErrorCodeIntType _sendPGN(pgnEnumType pgn) {
	volatile CANTxMessageBuffer *pTXMessage;
	int result, pgnTableIndex;
	U16 PDUFormat, PDUSpecific;

	// Find the pgn in the supported PGN table
	result = FALSE;
	for (pgnTableIndex=0; (pgnTableIndex<NUMBER_OF_PGNS_SUPPORTED_BY_THIS_MODULE) && (result != TRUE); pgnTableIndex++) {
		if (pgn == supportedPGNS[pgnTableIndex].pgn) {
			result = TRUE;
		}
	}

	if (result != TRUE) {
		// This pgn is not supported by this module
		canData.failReason = 11;
		return(FAIL);
	}

	// Find the pgn in the master pgn table
	result = FALSE;
	for (pgnTableIndex=0; (pgnTableIndex<NUMBER_OF_PGNS_IN_MASTER_TABLE) && (result != TRUE); pgnTableIndex++) {
		if (pgn == pgnMasterTable[pgnTableIndex].pgn) {
			result = TRUE;
		}
	}

	if (result != TRUE) {
		// This pgn is not in the master pgn table
		canData.failReason = 22;
		return(FAIL);
	}

	/* Get a pointer to the next buffer in the channel
		* check if the returned value is null. */

	pTXMessage = CANGetTxMessageBuffer(MY_CAN_MODULE,CAN_CHANNEL0);

	if (pTXMessage == NULL) {
		canData.failReason = 33;

		#ifdef FLUSH_TX_BUFFER_IF_NONE_AVAILABLE
			// Flush the TX buffers
			CANUpdateChannel(MY_CAN_MODULE, CAN_CHANNEL0);
			CANFlushTxChannel(MY_CAN_MODULE, CAN_CHANNEL0);
		#endif
		return(BUFFER_UNAVAILABLE);
	}

	result = loadCANData(pgn, (pgnDataType *)pTXMessage->data);

	// If result is 0 or more, then it is the length of the data field
	if (result >= 0) {
		ETS_ENTER_CRITICAL;
			PDUFormat = (pgn & 0xFF00) >> 8;
			PDUSpecific = (pgn & 0xFF);

			// Clear header
			pTXMessage->messageWord[0] = 0;
			pTXMessage->messageWord[1] = 0;

			switch (pgn) {
			case ELECTRONIC_ENGINE_CONTROLLER_3:
				pTXMessage->msgSID.SID = (J1939_INFO_PRIORITY<<8) + (PDUFormat>>2);
				pTXMessage->msgEID.EID = ((PDUFormat&0x3)<<16) + (PDUSpecific<<8) + UNINOX_CAN_ADDRESS;
				break;
			default:
				pTXMessage->msgSID.SID = (J1939_CONTROL_PRIORITY<<8) + (PDUFormat>>2);
				pTXMessage->msgEID.EID = ((PDUFormat&0x3)<<16) + (PDUSpecific<<8) + configurationData.CAN_sourceAddress;
				break;
			}

			pTXMessage->msgEID.IDE = 1;
			pTXMessage->msgEID.SRR = 1;
			pTXMessage->msgEID.RTR = 0;
			pTXMessage->msgEID.RB0 = 0;
			pTXMessage->msgEID.RB1 = 0;
			pTXMessage->msgEID.DLC = result;

			/* This function lets the CAN module
			* know that the message processing is done
			* and message is ready to be processed. */
			CANUpdateChannel(MY_CAN_MODULE, CAN_CHANNEL0);
		ETS_EXIT_CRITICAL;

		/* Direct the CAN module to flush the
			* TX channel. This will send any pending
			* message in the TX channel. */
		// 2010-11-23: Why does this take so long?  What can be done about it?
		CANFlushTxChannel(MY_CAN_MODULE, CAN_CHANNEL0);

		canData.flags.write++;
		canData.failReason = 99;

		// Give another task a chance
		taskYIELD();
		return(PASS);
	} else {
		canData.failReason = 44;
		return(FAIL);
	}

}

//-------------------------------------------------------------------------------------------------
// CAN Initialization
//-------------------------------------------------------------------------------------------------
#define RANDOM_TIME	1000
#define MAX_TRIES	1000
static boolean CANwasPreviouslyInitialized = FALSE;
static ErrorCodeIntType _open(void) {
	int i;
	CAN_BIT_CONFIG canBitConfig;
	CAN_TX_MSG_SID msgSID;
	CAN_MSG_EID msgEID;
	U16 PDUFormat, PDUSpecific;

	/* This function will intialize
	 * MY_CAN_MODULE module. */
	if (!CANwasPreviouslyInitialized) {
		CANwasPreviouslyInitialized = TRUE;
		memset(&canData, 0, sizeof(canData));
	}

#if (MY_MODULE_TYPE == AFRC_ADVANCED_TYPE)
	SET_CAN_PIN_OUTPUT;
#endif

	// Seed random number generator
	srand(ReadCoreTimer());


	// Fill this structure with random numbers to randomize the CAN transmit times
	for (i=0;(i<NUMBER_OF_PGNS_SUPPORTED_BY_THIS_MODULE) && (i<MAX_PGNS_SUPPORTED_PER_MODULE);i++) {
		canData.pgnWaitTimeBeforeNextTransmit[i] = rand()%RANDOM_TIME;
	}

	canData.flags.read = 0;
	canData.flags.write = 0;

	/* Step 1: Switch the CAN module
	 * ON and switch it to Configuration
	 * mode. Wait till the switch is 
	 * complete */

	// Added 2011/08/12
    CANAbortPendingTx(MY_CAN_MODULE, CAN_ALL_CHANNELS);

	// Clear the SERRIF bit by shutting off the CAN module
	CANSetOperatingMode(MY_CAN_MODULE, CAN_CONFIGURATION);
	CANEnableModule(MY_CAN_MODULE,FALSE);

	delay_ms(10);
	CANEnableModule(MY_CAN_MODULE,TRUE);
	CANSetOperatingMode(MY_CAN_MODULE, CAN_CONFIGURATION);

	// If this module is stuck in Busy mode, restart it
	if (C1CONbits.CANBUSY) {
		CANSetOperatingMode(MY_CAN_MODULE, CAN_CONFIGURATION);
		while (CANGetOperatingMode(MY_CAN_MODULE) != CAN_CONFIGURATION);

		CANEnableModule(MY_CAN_MODULE,FALSE);

		CANSetOperatingMode(MY_CAN_MODULE, CAN_CONFIGURATION);
		while (CANGetOperatingMode(MY_CAN_MODULE) != CAN_CONFIGURATION);

		CANEnableModule(MY_CAN_MODULE,FALSE);
		CANEnableModule(MY_CAN_MODULE,TRUE);
		CANSetOperatingMode(MY_CAN_MODULE, CAN_NORMAL_OPERATION);
		while (CANGetOperatingMode(MY_CAN_MODULE) != CAN_NORMAL_OPERATION);
	}

#ifdef IGNORE_RETRY_FAILURE
	// Wait forever
	//------------------------------------------------------------------
	// Wait forever
	//------------------------------------------------------------------
	// Borrow i to store the LED state
	//------------------------------------------------------------------
	while (CANGetOperatingMode(MY_CAN_MODULE) != CAN_CONFIGURATION);
#else
	for (i=0;i<MAX_TRIES;i++) {
		if (CANGetOperatingMode(MY_CAN_MODULE) == CAN_CONFIGURATION) {
			break;
		}
	}
	if (i>= MAX_TRIES) {
		return(TIMEOUT);
	}
#endif

	/* Step 2: Configure the CAN Module Clock. The
	 * CAN_BIT_CONFIG data structure is used
	 * for this purpose. The propagation segment, 
	 * phase segment 1 and phase segment 2
	 * are configured to have 3TQ. The
     * CANSetSpeed function sets
	 the baud.*/
	
	canBitConfig.phaseSeg2Tq            = CAN_BIT_3TQ;
	canBitConfig.phaseSeg1Tq            = CAN_BIT_3TQ;
	canBitConfig.propagationSegTq       = CAN_BIT_3TQ;
	canBitConfig.phaseSeg2TimeSelect    = TRUE;
	canBitConfig.sample3Time            = TRUE;
    canBitConfig.syncJumpWidth          = CAN_BIT_2TQ;

    CANSetSpeed(MY_CAN_MODULE,&canBitConfig,GetSystemClock(),CAN_BUS_SPEED);
  
	/* Step 3: Assign the buffer area to the
     * CAN module.
     */ 

	CANAssignMemoryBuffer(MY_CAN_MODULE,(void *)CANMessageFifoArea,(2 * 8 * 16));	

	/* Step 4: Configure channel 0 for TX and size of
     * 8 message buffers with RTR disabled and low medium
     * priority. Configure channel 1 for RX and size
     * of 8 message buffers and receive the full message.
     */

    CANConfigureChannelForTx(MY_CAN_MODULE, CAN_CHANNEL0, 8, CAN_TX_RTR_DISABLED, CAN_LOW_MEDIUM_PRIORITY);
    CANConfigureChannelForRx(MY_CAN_MODULE, CAN_CHANNEL1, 8, CAN_RX_FULL_RECEIVE);
	
	//------------------------------------------------------------------------------------------------
	// Ideally, these filters should only allow incoming data from an EIM (SA=0-3) from PF_TRANSMIT_DATA to this
	// module's CAN source address.
	//------------------------------------------------------------------------------------------------

	//------------------------------------------------------------------------------------------------
	// Filter 0: Accept data sent to this module's address with commands to configure this device.
	//------------------------------------------------------------------------------------------------
	msgSID.SID = (PF_TRANSMIT_DATA>>2);
	// This isn't quite correct but it works because EIM_TYPE is Zero.
	msgEID.EID = ((PF_TRANSMIT_DATA&0x3)<<16) + (configurationData.CAN_sourceAddress<<8) + (EIM_TYPE*MAX_NUMBER_OF_SAME_MODULE_TYPES_ON_BUS);

	// Disable all filters before configuring (supports 0-31)
	CANEnableFilter(MY_CAN_MODULE, CAN_FILTER0, FALSE);
	CANEnableFilter(MY_CAN_MODULE, CAN_FILTER1, FALSE);
	CANEnableFilter(MY_CAN_MODULE, CAN_FILTER2, FALSE);
	CANEnableFilter(MY_CAN_MODULE, CAN_FILTER3, FALSE);
	CANEnableFilter(MY_CAN_MODULE, CAN_FILTER4, FALSE);

	//------------------------------------------------------------------------------------------------
	// Filter 0: Accept PF_TRANSMIT_DATA from any EIM
	//------------------------------------------------------------------------------------------------
	#ifdef ACCEPT_ALL_MESSAGES
		#define MY_FILTER_MASK	0x00000
	#else
		//#define MY_FILTER_MASK	0x3FFFF	// Only accept correct address
		//#define MY_FILTER_MASK	0x0FFFF

		// Before 2013-02-06: May not be correct
		//#define MY_FILTER_MASK		0x00FFF	// Only accept correct address, allow any priority

		// 2013-02-06 Appears to work correctly per description in comment
		#define MY_FILTER_MASK		0x00FFFF	// Only accept correct address, allow any priority
	#endif

	CANConfigureFilter(MY_CAN_MODULE, CAN_FILTER0, msgEID.EID, CAN_EID);
#ifdef CAN_SPN_RECEIVE_TEST
	CANConfigureFilterMask(MY_CAN_MODULE, CAN_FILTER_MASK0, 0x30000, CAN_EID, CAN_FILTER_MASK_IDE_TYPE);
#else
	CANConfigureFilterMask(MY_CAN_MODULE, CAN_FILTER_MASK0, MY_FILTER_MASK, CAN_EID, CAN_FILTER_MASK_IDE_TYPE);
#endif
	CANLinkFilterToChannel(MY_CAN_MODULE, CAN_FILTER0, CAN_FILTER_MASK0, CAN_CHANNEL1); 
	CANEnableFilter(MY_CAN_MODULE, CAN_FILTER0, TRUE);

	//------------------------------------------------------------------------------------------------
	// Filter 1: Accept PF_TRANSMIT_DATA from the EIM that is addressed to all modules
	//------------------------------------------------------------------------------------------------
	msgEID.EID = ((PF_TRANSMIT_DATA&0x3)<<16) + (0xff<<8) + EIM_TYPE;
	CANConfigureFilter(MY_CAN_MODULE, CAN_FILTER1, msgEID.EID, CAN_EID);
	CANConfigureFilterMask(MY_CAN_MODULE, CAN_FILTER_MASK1, MY_FILTER_MASK, CAN_EID, CAN_FILTER_MASK_IDE_TYPE);
	CANLinkFilterToChannel(MY_CAN_MODULE, CAN_FILTER1, CAN_FILTER_MASK1, CAN_CHANNEL1); 
	CANEnableFilter(MY_CAN_MODULE, CAN_FILTER1, TRUE);

#ifdef ACCEPT_ANY_PREPOST_TC
	//------------------------------------------------------------------------------------------------
	// Filter 2: Accept the following PGN's from any address:
	// EMD's accept AFRC_TEMPERATURE
	// AFRC's accept EMD_TEMPERATURE_1
	//------------------------------------------------------------------------------------------------
	#if (MY_MODULE_TYPE == EMD_TYPE)
		PDUFormat = (AFRC_TEMPERATURE & 0xFF00) >> 8;
		PDUSpecific = (AFRC_TEMPERATURE & 0xFF);
	#else
		PDUFormat = (EMD_TEMPERATURE_1 & 0xFF00) >> 8;
		PDUSpecific = (EMD_TEMPERATURE_1 & 0xFF);
	#endif

	msgSID.SID = (PDUFormat>>2);
	msgEID.EID = ((PDUFormat&0x3)<<16) + (PDUSpecific<<8);
	CANConfigureFilter(MY_CAN_MODULE, CAN_FILTER2, msgEID.EID, CAN_EID);

	msgSID.SID = (0xFF>>2);
	msgEID.EID = ((0xFF&0x3)<<16) + (0xFF<<8);
	CANConfigureFilterMask(MY_CAN_MODULE, CAN_FILTER_MASK2, msgEID.EID, CAN_EID, CAN_FILTER_MASK_IDE_TYPE);

	CANLinkFilterToChannel(MY_CAN_MODULE, CAN_FILTER2, CAN_FILTER_MASK2, CAN_CHANNEL1); 
	CANEnableFilter(MY_CAN_MODULE, CAN_FILTER2, TRUE);

	//------------------------------------------------------------------------------------------------
	// Filter 3: Accept the following PGN's from any address:
	// EXHAUST_TEMPERATURE_C
	//------------------------------------------------------------------------------------------------
	PDUFormat = (EXHAUST_TEMPERATURE_C & 0xFF00) >> 8;
	PDUSpecific = (EXHAUST_TEMPERATURE_C & 0xFF);

	msgSID.SID = (PDUFormat>>2);
	msgEID.EID = ((PDUFormat&0x3)<<16) + (PDUSpecific<<8);
	CANConfigureFilter(MY_CAN_MODULE, CAN_FILTER3, msgEID.EID, CAN_EID);

	CANLinkFilterToChannel(MY_CAN_MODULE, CAN_FILTER3, CAN_FILTER_MASK2, CAN_CHANNEL1); 
	CANEnableFilter(MY_CAN_MODULE, CAN_FILTER3, TRUE);
#else
	//------------------------------------------------------------------------------------------------
	// Filter 2: Accept CLEAR_ALL_DTCS from the EIM address
	//------------------------------------------------------------------------------------------------
	PDUFormat = (CLEAR_ALL_DTCS & 0xFF00) >> 8;
	PDUSpecific = (CLEAR_ALL_DTCS & 0xFF);

	msgSID.SID = (PDUFormat>>2);
	// This isn't quite correct but it works because EIM_TYPE is Zero.
	msgEID.EID = ((PDUFormat&0x3)<<16) + (PDUSpecific<<8) + (EIM_TYPE*MAX_NUMBER_OF_SAME_MODULE_TYPES_ON_BUS);
	CANConfigureFilter(MY_CAN_MODULE, CAN_FILTER2, msgEID.EID, CAN_EID);
	CANConfigureFilterMask(MY_CAN_MODULE, CAN_FILTER_MASK2, MY_FILTER_MASK, CAN_EID, CAN_FILTER_MASK_IDE_TYPE);
	CANLinkFilterToChannel(MY_CAN_MODULE, CAN_FILTER2, CAN_FILTER_MASK2, CAN_CHANNEL1); 
	CANEnableFilter(MY_CAN_MODULE, CAN_FILTER2, TRUE);

	//------------------------------------------------------------------------------------------------
	// Filter 3: Accept data sent to this module's address with commands to configure this device.
	//------------------------------------------------------------------------------------------------
	msgSID.SID = (PF_REQUEST_PGN>>2);
	// This isn't quite correct but it works because EIM_TYPE is Zero.
	msgEID.EID = ((PF_REQUEST_PGN&0x3)<<16) + (configurationData.CAN_sourceAddress<<8) + (EIM_TYPE*MAX_NUMBER_OF_SAME_MODULE_TYPES_ON_BUS);

	//------------------------------------------------------------------------------------------------
	// Filter 3: Accept PF_REQUEST_PGN from any EIM
	//------------------------------------------------------------------------------------------------
	CANConfigureFilter(MY_CAN_MODULE, CAN_FILTER0, msgEID.EID, CAN_EID);
	CANConfigureFilterMask(MY_CAN_MODULE, CAN_FILTER_MASK0, MY_FILTER_MASK, CAN_EID, CAN_FILTER_MASK_IDE_TYPE);
	CANLinkFilterToChannel(MY_CAN_MODULE, CAN_FILTER0, CAN_FILTER_MASK0, CAN_CHANNEL1); 
	CANEnableFilter(MY_CAN_MODULE, CAN_FILTER0, TRUE);

	//------------------------------------------------------------------------------------------------
	// Filter 4: Accept PF_REQUEST_PGN from the EIM that is addressed to all modules
	//------------------------------------------------------------------------------------------------
	msgEID.EID = ((PF_REQUEST_PGN&0x3)<<16) + (0xff<<8) + EIM_TYPE;
	CANConfigureFilter(MY_CAN_MODULE, CAN_FILTER1, msgEID.EID, CAN_EID);
	CANConfigureFilterMask(MY_CAN_MODULE, CAN_FILTER_MASK1, MY_FILTER_MASK, CAN_EID, CAN_FILTER_MASK_IDE_TYPE);
	CANLinkFilterToChannel(MY_CAN_MODULE, CAN_FILTER1, CAN_FILTER_MASK1, CAN_CHANNEL1); 
	CANEnableFilter(MY_CAN_MODULE, CAN_FILTER1, TRUE);
#endif

	/* Step 6: Enable interrupt and events. Enable the receive
     * channel not empty event (channel event) and the receive
     * channel event (module event).
     * The interrrupt peripheral library is used to enable
     * the CAN interrupt to the CPU. */

	CANEnableChannelEvent(MY_CAN_MODULE, CAN_CHANNEL1, CAN_RX_CHANNEL_NOT_EMPTY, TRUE);
	CANEnableModuleEvent (MY_CAN_MODULE, CAN_RX_EVENT, TRUE);

    /* These functions are from interrupt peripheral library. */
    
	INTSetVectorPriority(INT_VECTOR_CAN(MY_CAN_MODULE), INT_PRIORITY_LEVEL_4);
	INTSetVectorSubPriority(INT_VECTOR_CAN(MY_CAN_MODULE), INT_SUB_PRIORITY_LEVEL_0);

	INTEnableSystemMultiVectoredInt();

	// Enable the module
	CANEnableModule(MY_CAN_MODULE,TRUE);

	/* Step 7: Switch the CAN mode back to normal mode. */
	CANSetOperatingMode(MY_CAN_MODULE, CAN_NORMAL_OPERATION);

#ifdef IGNORE_RETRY_FAILURE
	//------------------------------------------------------------------
	// Wait forever
	//------------------------------------------------------------------
	// Borrow i to store the LED state
	//------------------------------------------------------------------
	while (CANGetOperatingMode(MY_CAN_MODULE) != CAN_NORMAL_OPERATION) {
		CANSetOperatingMode(MY_CAN_MODULE, CAN_NORMAL_OPERATION);

		// If this module is stuck in Busy mode, restart it
		if (C1CONbits.CANBUSY) {
			CANSetOperatingMode(MY_CAN_MODULE, CAN_CONFIGURATION);
			while (CANGetOperatingMode(MY_CAN_MODULE) != CAN_CONFIGURATION);

			CANEnableModule(MY_CAN_MODULE,FALSE);

			CANSetOperatingMode(MY_CAN_MODULE, CAN_CONFIGURATION);
			while (CANGetOperatingMode(MY_CAN_MODULE) != CAN_CONFIGURATION);

			CANEnableModule(MY_CAN_MODULE,FALSE);
			CANEnableModule(MY_CAN_MODULE,TRUE);
			CANSetOperatingMode(MY_CAN_MODULE, CAN_NORMAL_OPERATION);
			while (CANGetOperatingMode(MY_CAN_MODULE) != CAN_NORMAL_OPERATION);
		}
	}
#else
	for (i=0;i<MAX_TRIES;i++) {
		if (CANGetOperatingMode(MY_CAN_MODULE) == CAN_NORMAL_OPERATION) {
			break;
		}
	}
	if (i>= MAX_TRIES) {
		return(TIMEOUT);
	}
#endif

	SET_CAN_RS_TRIS_OUTPUT;

	#ifndef DISABLE_CAN_TASK
		// Create RX semaphore
		if (canData.rx.semaphore == NULL) {
			canData.rx.semaphore = xSemaphoreCreateCounting(SIZE_OF_CAN_MESSAGE_RX_QUEUE, 0);
			if (canData.rx.semaphore == NULL) {
				return(FAIL);
			}
		}

		// Start the CAN protocol task
		if (canData.xHandle == NULL) {
			if (xTaskCreate( vCANProtocolTask, "CAN", NORMAL_TASK_STACK_SIZE+256, NULL, CAN_TASK_PRIORITY, &canData.xHandle ) != pdPASS) {
				return(TASK_COULD_NOT_BE_CREATED);
			}
		}
	#endif

	systemData.led[LED_CAN_INDEX].state = LED_OFF_STATE;

	return(PASS);
}

//-------------------------------------------------------------------------------------------------
// Read (not fully implemented)
//-------------------------------------------------------------------------------------------------
CANRxMessageBuffer * pReadMessage;
static ErrorCodeIntType _read(void)
{
#ifdef __DEBUG
	spnEnumType spn;
	U8 *pSource;
	CANRxMessageBuffer *pMessage;
#endif

	/* This function will check if a CAN
	 * message is available in MY_CAN_MODULE channel 1.
     * If so , the message is read. Byte 0 of
     * the received message will indicate if 
	 * LED6 should be switched ON or OFF. */

//	static CANRxMessageBuffer * pMessage;

	// If a message is not waiting, return
	if (xSemaphoreTake(canData.rx.semaphore, 0) != pdPASS) {
		systemData.led[LED_GEN_PURPOSE_INDEX].state = LED_OFF_STATE;
		return(EMPTY_BUFFER);
	}

	while (canData.rx.tail != canData.rx.head) {
		/* Message was received. Reset message received flag to catch
		 * the next message and read the message. Note that
		 * you could also check the CANGetRxMessage function
		 * return value for null to check if a message has 
		 * been received. */

		pReadMessage = &CANRxBuffer[canData.rx.tail];
		if (pReadMessage == NULL) {
			systemData.led[LED_GEN_PURPOSE_INDEX].state = LED_OFF_STATE;
			canData.rx.errorCounter++;
			return(BUFFER_UNAVAILABLE);
		}
	#ifdef __DEBUG
		pMessage = (CANRxMessageBuffer *)pReadMessage;
		pSource = pMessage->data;
		spn = (spnEnumType)*pSource++;
		spn += (spnEnumType)*pSource++ * 256;
		if (operationalData.debug.enable && (spn == SPN_AUTO_MANUAL_MODE)) {
			operationalData.debug.breakpoint = 888;
		}
	#endif

		// Process the message
		if (decodeCANRxMessage(pReadMessage) == PASS) {
			canData.rx.timeSinceLastRx_ms = 0;
			systemData.led[LED_GEN_PURPOSE_INDEX].state = LED_ON_STATE;
		}

		RX_INCREMENT_CAN_TAIL;
		canData.rx.counter++;

	#ifndef PUT_RX_UPDATE_INSIDE_INTERRUPT	// This isn't right 2011/12/08
		/* Call the CANUpdateChannel() function to let
		 * CAN 1 module know that the message processing
		 * is done. Enable the receive channel not empty event
		 * so that the CAN module generates an interrupt when
		 * the event occurs the next time.*/

		CANUpdateChannel(MY_CAN_MODULE, CAN_CHANNEL1);
		CANEnableChannelEvent(MY_CAN_MODULE, CAN_CHANNEL1, CAN_RX_CHANNEL_NOT_EMPTY, TRUE);
	#endif
	}

	return(PASS);
}

//-------------------------------------------------------------------------------------------------
// Write
//-------------------------------------------------------------------------------------------------
// This function adds the Transmit PGN/SPN to the TX queue.  Another function checks what is on the
// queue and sends it.
//-------------------------------------------------------------------------------------------------
static ErrorCodeIntType _write(U8 destinationAddress, spnEnumType spn, dataUnionType dataUnion) {
	CANTxMessageBuffer *pMessage;
	int ioTableIndex,
		slotIndex,
		spnMasterIndex;
	U8 *pSource;
	U8 *pDestination;
	U8 temp8;
	U16 temp16;
	U32 temp32;
	float tempFloat;
	U16 PDUFormat, PDUSpecific;

	spnMasterIndex = getSPNMasterTableIndex(spn);
	if (spnMasterIndex < 0) {
		return(FAIL);
	}

	slotIndex = getSLOTindex(spnMasterTable[spnMasterIndex].slotIdentifier);
	if (slotIndex < 0) {
		return(FAIL);
	}

	pMessage = &CANTxBuffer[canData.tx.head];
	TX_INCREMENT_CAN_HEAD;

	// Use connected CAN module's source address as the destination address
	PDUFormat = PF_TRANSMIT_DATA;
	PDUSpecific = destinationAddress;

	pMessage->msgSID.SID = (J1939_CONTROL_PRIORITY<<8) + (PDUFormat>>2);
	pMessage->msgEID.EID = ((PDUFormat&0x3)<<16) + (PDUSpecific<<8) + configurationData.CAN_sourceAddress;
	pMessage->msgEID.IDE = 1;				/* IDE = 1 means Extended ID pTXMessage.	*/
	pMessage->msgEID.RTR = 0;				/* Not an RTR pTXMessage.              	*/

	// 2012/04/11 Programmer's Note - this is only guaranteed to work for Production Test SPN's
	switch (spn) {
	case SPN_PRODUCTION_TEST_SET_SENSOR_HEATER_PWM:
		tempFloat = dataUnion.f;
		break;
	default:
		ioTableIndex = get_IOtableIndex(spn);
		if (ioTableIndex >= 0) {
			// If this one is in our local IO Table, then it has a data type associated with it.
			switch (ioTable[ioTableIndex].dataType) {
			case TYPE_S8:
			case TYPE_U8:
				tempFloat = dataUnion.u8;
				break;
			case TYPE_S16:
			case TYPE_U16:
				tempFloat = dataUnion.u16;
				break;
			case TYPE_S32:
			case TYPE_U32:
				tempFloat = dataUnion.u32;
				break;
			case TYPE_FLOAT:
				tempFloat = dataUnion.f;
				break;
			default:
				tempFloat = dataUnion.u8;
				break;
			}
		} else {
			tempFloat = dataUnion.u8;
		}
		break;
	}
	tempFloat = SlotConvert_outgoing(tempFloat, slotIndex);

	pDestination = pMessage->data;

	// Add SPN
	pSource	= (U8 *)&spn;
	*pDestination++ = *pSource++;
	*pDestination++ = *pSource++;

	switch (slotTable[slotIndex].bytes) {
	case 1:
		temp8 = tempFloat;
		pSource	= (U8 *)&temp8;
		*pDestination++ = *pSource++;
		pMessage->msgEID.DLC = 1;
		break;
	case 2:
		temp16 = tempFloat;
		pSource	= (U8 *)&temp16;
		*pDestination++ = *pSource++;
		*pDestination++ = *pSource++;
		break;
	case 4:
		temp32 = tempFloat;
		pSource	= (U8 *)&temp32;
		*pDestination++ = *pSource++;
		*pDestination++ = *pSource++;
		*pDestination++ = *pSource++;
		*pDestination++ = *pSource++;
		break;
	}
	pMessage->msgEID.DLC = pDestination - pMessage->data;
	canData.tx.counter++;

	_sendMessage(pMessage);

	return(PASS);
}

//-------------------------------------------------------------------------------------------------
static CANRxMessageBuffer *_pISRmessage;
void __ISR(_CAN_1_VECTOR, ipl4) CANInterruptHandler(void) {
	static portBASE_TYPE xHigherPriorityTaskWasWoken;
	xHigherPriorityTaskWasWoken = pdFALSE;

	/* This is the MY_CAN_MODULE Interrupt Handler.
	 * Note that there are many source events in the
	 * MY_CAN_MODULE module for this interrupt. These
	 * events are enabled by the  CANEnableModuleEvent()
     * function. In this example, only the RX_EVENT
	 * is enabled. */

	/* Check if the source of the interrupt is
	 * RX_EVENT. This is redundant since only this
     * event is enabled in this example but
     * this shows one scheme for handling events. */

// #define USE_NEW_VERSION
#ifdef USE_NEW_VERSION	// 2013-04-02
	systemData.interruptCounter.canStatus = CANGetModuleEvent(MY_CAN_MODULE);

	// Set flags for tracking and debug
	switch (systemData.interruptCounter.canStatus) {
	case CAN_TX_EVENT:
		systemData.interruptCounter.canTx++;
		break;
	case CAN_RX_EVENT:
		systemData.interruptCounter.canRx++;
		break;
	case CAN_TIMESTAMP_TIMER_OVERFLOW_EVENT:
		systemData.interruptCounter.canTimestampOverflow++;
		break;
	case CAN_OPERATION_MODE_CHANGE_EVENT:
		systemData.interruptCounter.canModeChange++;
		break;
	case CAN_RX_OVERFLOW_EVENT:
		systemData.interruptCounter.canRxOverflow++;
		break;
	case CAN_SYSTEM_ERROR_EVENT:
		systemData.interruptCounter.canSystemError++;
		break;
	case CAN_BUS_ERROR_EVENT:
		systemData.interruptCounter.canBusError++;
		break;
	case CAN_BUS_ACTIVITY_WAKEUP_EVENT:
		systemData.interruptCounter.canBusWakeup++;
		break;
	case CAN_INVALID_RX_MESSAGE_EVENT:
		systemData.interruptCounter.canInvalidRx++;
		break;
	default:
		// Unknown event
		systemData.interruptCounter.canUnknown++;
		break;
	}

	// Take action if required
	switch (systemData.interruptCounter.canStatus) {
	case CAN_TX_EVENT:
		break;
	default:
		if(CANGetPendingEventCode(MY_CAN_MODULE) == CAN_CHANNEL1_EVENT) {
			/* This means that channel 1 caused the event.
			 * The CAN_RX_CHANNEL_NOT_EMPTY event is persistent. You
			 * could either read the channel in the ISR
			 * to clear the event condition or as done 
			 * here, disable the event source, and set
			 * an application flag to indicate that a message
			 * has been received. The event can be
			 * enabled by the application when it has processed
			 * one message.
			 *
			 * Note that leaving the event enabled would
			 * cause the CPU to keep executing the ISR since
			 * the CAN_RX_CHANNEL_NOT_EMPTY event is persistent (unless
			 * the not empty condition is cleared.) 
			 * */
			
            CANEnableChannelEvent(MY_CAN_MODULE, CAN_CHANNEL1, CAN_RX_CHANNEL_NOT_EMPTY, FALSE);

			_pISRmessage = CANGetRxMessage(MY_CAN_MODULE,CAN_CHANNEL1);

			if (_pISRmessage != NULL) {
				memcpy(&CANRxBuffer[canData.rx.head], _pISRmessage, sizeof(CANRxMessageBuffer));
				RX_INCREMENT_CAN_HEAD_FROM_ISR;
				xSemaphoreGiveFromISR(canData.rx.semaphore, &xHigherPriorityTaskWasWoken);
			}

			#ifdef PUT_RX_UPDATE_INSIDE_INTERRUPT	// This isn't right 2011/12/08
				/* Call the CANUpdateChannel() function to let
				 * CAN 1 module know that the message processing
				 * is done. Enable the receive channel not empty event
				 * so that the CAN module generates an interrupt when
				 * the event occurs the next time.*/

				CANUpdateChannel(MY_CAN_MODULE, CAN_CHANNEL1);
				CANEnableChannelEvent(MY_CAN_MODULE, CAN_CHANNEL1, CAN_RX_CHANNEL_NOT_EMPTY, TRUE);
			#endif
		}
		break;
	}
#else
	if((CANGetModuleEvent(MY_CAN_MODULE) & CAN_RX_EVENT) != 0) {
		systemData.interruptCounter.canRx++;

		/* Within this, you can check which channel caused the 
		 * event by using the CANGetModuleEvent() function
         * which returns a code representing the highest priority
         * pending event. */ 
		
		if(CANGetPendingEventCode(MY_CAN_MODULE) == CAN_CHANNEL1_EVENT) {
			/* This means that channel 1 caused the event.
			 * The CAN_RX_CHANNEL_NOT_EMPTY event is persistent. You
			 * could either read the channel in the ISR
			 * to clear the event condition or as done 
			 * here, disable the event source, and set
			 * an application flag to indicate that a message
			 * has been received. The event can be
			 * enabled by the application when it has processed
			 * one message.
			 *
			 * Note that leaving the event enabled would
			 * cause the CPU to keep executing the ISR since
			 * the CAN_RX_CHANNEL_NOT_EMPTY event is persistent (unless
			 * the not empty condition is cleared.) 
			 * */
			
            CANEnableChannelEvent(MY_CAN_MODULE, CAN_CHANNEL1, CAN_RX_CHANNEL_NOT_EMPTY, FALSE);

			_pISRmessage = CANGetRxMessage(MY_CAN_MODULE,CAN_CHANNEL1);

			if (_pISRmessage != NULL) {
				memcpy(&CANRxBuffer[canData.rx.head], _pISRmessage, sizeof(CANRxMessageBuffer));
				RX_INCREMENT_CAN_HEAD_FROM_ISR;
				xSemaphoreGiveFromISR(canData.rx.semaphore, &xHigherPriorityTaskWasWoken);
			}

			#ifdef PUT_RX_UPDATE_INSIDE_INTERRUPT	// This isn't right 2011/12/08
				/* Call the CANUpdateChannel() function to let
				 * CAN 1 module know that the message processing
				 * is done. Enable the receive channel not empty event
				 * so that the CAN module generates an interrupt when
				 * the event occurs the next time.*/

				CANUpdateChannel(MY_CAN_MODULE, CAN_CHANNEL1);
				CANEnableChannelEvent(MY_CAN_MODULE, CAN_CHANNEL1, CAN_RX_CHANNEL_NOT_EMPTY, TRUE);
			#endif
		}
	} else {
		systemData.interruptCounter.canTx++;
	}
#endif

	if (xHigherPriorityTaskWasWoken == pdTRUE) {
		taskYIELD();
	}

   /* The MY_CAN_MODULE Interrupt flag is  cleared at the end of the 
	* interrupt routine. This is because the event source
    * that could have caused this interrupt  to occur 
    * (CAN_RX_CHANNEL_NOT_EMPTY) is disabled. Attempting to 
	* clear the MY_CAN_MODULE interrupt flag when the the CAN_RX_CHANNEL_NOT_EMPTY
    * interrupt is enabled will not have any effect because the 
	* base event is still present. */ 
	
	INTClearFlag(INT_SOURCE_CAN(MY_CAN_MODULE));
}

//-------------------------------------------------------------------------------------------------
// Returns length of data on success or ErrorCodeIntType on failure
//-------------------------------------------------------------------------------------------------
ErrorCodeIntType loadCANData(pgnEnumType pgn, pgnDataType *pDestination) {
	U8 *pString = pNULL;
	U8 *pInput;
	U8 *pOutput;
	volatile spnEnumType spn;
	int spnMasterIndex;
	int ioTableIndex = 0, 
		slotIndex,
		spnIndex, 
		result, 
		pgnTableIndex,
		tempInt;
	U8 temp8 = 0;
	U16 temp16 = 0;
	U32 temp32 = 0;
	U64 temp64 = 0;
	dataUnionType dataUnion;
	float tempFloat;
	float tempFloat2;
	boolean loadWithBlanks;

	if (pDestination == NULL) {
		return(BUFFER_UNAVAILABLE);
	}

#ifdef __DEBUG
	volatile int dwhBreak;
	if (pgn == PGN_TACHOGRAPH) {
		dwhBreak = 999;
	} else {
		dwhBreak = 111;
	}
#endif

	// Find the PGN in the master table
	result = FALSE;
	for (pgnTableIndex=0; (pgnTableIndex<NUMBER_OF_PGNS_IN_MASTER_TABLE) && (result != TRUE); pgnTableIndex++) {
		if (pgn == pgnMasterTable[pgnTableIndex].pgn) {
			result = TRUE;
			break;
		}
	}

	/// PGN was not in the master table
	if (result != TRUE) {
		return(FAIL);
	}

	pOutput = (U8 *)pDestination;

	// Loop through list of SPN's
	for (spnIndex=0; spnIndex<MAX_SPNS_PER_PGN; spnIndex++) {

		spn = pgnMasterTable[pgnTableIndex].spn[spnIndex];

		#ifdef __DEBUG
			if ((dwhBreak == 888) ||
				(dwhBreak == 999)) {
				dwhBreak = 888;
			}
		#endif

		if (spn != SPN_INVALID) {
			//---------------------------------------
			// Find this spn in the master spn table
			//---------------------------------------
			spnMasterIndex = getSPNMasterTableIndex(spn);
			#ifdef __DEBUG
				if (spnMasterIndex == FAIL) {
					while (TRUE);
				}
			#endif

			//-----------------------------------------------
			// Find this spn in this module's spn data table
			//-----------------------------------------------
			ioTableIndex = getSPNmoduleIndex(spn);

			if ((spnMasterIndex == FAIL) ||
				(ioTableIndex == FAIL)) {
				// Load with blank data
				loadWithBlanks = TRUE;
			} else {
				loadWithBlanks = FALSE;
			}

			#if (MY_MODULE_TYPE == AFRC_LITE_TYPE) || (MY_MODULE_TYPE == AFRC_ADVANCED_TYPE)
				switch (spn) {
				// The multisetpoint table Input column uses the slot index from the sourceSPN 
				case SPN_MULTISETPOINT_TABLE_ENTRY_0_INPUT:
				case SPN_MULTISETPOINT_TABLE_ENTRY_1_INPUT:
				case SPN_MULTISETPOINT_TABLE_ENTRY_2_INPUT:
				case SPN_MULTISETPOINT_TABLE_ENTRY_3_INPUT:
				case SPN_MULTISETPOINT_TABLE_ENTRY_4_INPUT:
					result = TRUE;
					if (getSPNMasterTableIndex(operationalData.multiSetpointTable.sourceSPN) == FAIL) {
						result = FALSE;
					}
					if (operationalData.multiSetpointTable.config != MST_ENABLED) {
						result = FALSE;
					}

					tempInt = getSPNMasterTableIndex(operationalData.multiSetpointTable.sourceSPN);
					if (tempInt != FAIL) {
						slotIndex = getSLOTindex(spnMasterTable[tempInt].slotIdentifier);
					} else {
						// Fill in with dummy data
						tempInt = getSPNMasterTableIndex(SPN_ANALOG_CURRENT_1);
						if (tempInt != FAIL) {
							slotIndex = getSLOTindex(spnMasterTable[tempInt].slotIdentifier);
						} else {
							slotIndex = FAIL;
						}
						loadWithBlanks = TRUE;
					}
					break;
				default:
					slotIndex = getSLOTindex(spnMasterTable[spnMasterIndex].slotIdentifier);
					break;
				}
			#else
				slotIndex = getSLOTindex(spnMasterTable[spnMasterIndex].slotIdentifier);
			#endif

			if (slotIndex == FAIL) {
				// exit outer for() loop
				break;
			}

			if (loadWithBlanks) {
				for (tempInt=0; tempInt<slotTable[slotIndex].bytes; tempInt++) {
					*pOutput++ = 0xFF;
				}
			} else {
				switch (ioTable[ioTableIndex].dataType) {
				case TYPE_STRING:
					pInput = (void *)ioTable[ioTableIndex].pValue;
					tempInt = 0;
					while ((*pInput != NULL_CHAR) &&
						   (tempInt < slotTable[slotIndex].bytes)) {
						*pOutput++ = *pInput++;
						tempInt++;
					}
					break;
				case TYPE_FLOAT:
					dataUnion = *ioTable[ioTableIndex].pValue;
	
					tempFloat2 = dataUnion.f;
					tempFloat = SlotConvert_outgoing(tempFloat2, slotIndex);

					// Rely on the compiler to convert from a float to an integer
					switch (slotTable[slotIndex].bytes) {
					case 1:
						temp8 = tempFloat;
						break;
					case 2:
						temp16 = tempFloat;
						break;
					case 4:
						temp32 = tempFloat;
						break;
					case 8:
						temp64 = tempFloat;
						break;
					default:
						// Do nothing
						break;
					}
					break;
				default:
					// Assume integer type which doesn't use the SLOT table
					switch (slotTable[slotIndex].bytes) {
					case 1:
						temp8 = SPN_GET_U8_VALUE(ioTableIndex);
						break;
					case 2:
						temp16 = SPN_GET_U16_VALUE(ioTableIndex);
						break;
					case 4:
						temp32 = SPN_GET_U32_VALUE(ioTableIndex);
						break;
					case 8:
						temp64 = SPN_GET_U64_VALUE(ioTableIndex);
						break;
					default:
						// Do nothing
						break;
					}
				}

				switch (slotTable[slotIndex].bytes) {
				case 1:
					*pOutput++ = temp8;
					break;
				case 2:
					pInput	= (U8 *)&temp16;
					*pOutput++ = *pInput++;
					*pOutput++ = *pInput++;
					break;
				case 4:
					pInput	= (U8 *)&temp32;
					*pOutput++ = *pInput++;
					*pOutput++ = *pInput++;
					*pOutput++ = *pInput++;
					*pOutput++ = *pInput++;
					break;
				case 6:
					switch (ioTable[ioTableIndex].dataType) {
					case TYPE_STRING:
						// 2013-07-23 Programmer's Note - I recognize this is a bit of a hack.
						// Loading the buffer is handled above because it is a variable length.
						break;
					default:
						pInput	= pString;
						*pOutput++ = *pInput++;
						*pOutput++ = *pInput++;
						*pOutput++ = *pInput++;
						*pOutput++ = *pInput++;
						*pOutput++ = *pInput++;
						*pOutput++ = *pInput++;
						break;
					}
					break;
				case 8:
					pInput	= (U8 *)&temp64;
					*pOutput++ = *pInput++;
					*pOutput++ = *pInput++;
					*pOutput++ = *pInput++;
					*pOutput++ = *pInput++;
					*pOutput++ = *pInput++;
					*pOutput++ = *pInput++;
					*pOutput++ = *pInput++;
					*pOutput++ = *pInput++;
					break;
				default:
					// Do nothing
					break;
				}
			}
		} else {
			break;
		}
	}

	return(pOutput - (U8 *)pDestination);
}

//=================================================================================================
ErrorCodeIntType decodeCANRxMessage(CANRxMessageBuffer *pMessage) {
	pgnEnumType pgn;
	ErrorCodeIntType result;
	boolean rxFromAFRCorEMD;

	canData.rx.PDUFormat = ((pMessage->msgSID.SID & 0x3F)<<2) + ((pMessage->msgEID.EID>>16)&0x03);
	canData.rx.PDUSpecific = (pMessage->msgEID.EID & 0xff00) >> 8;
	canData.rx.sourceAddress = pMessage->msgEID.EID & 0xff;

	rxFromAFRCorEMD = FALSE;
	#ifdef USE_PRODUCTION_TEST_HW
		if (InRange(EMD_TYPE*MAX_NUMBER_OF_SAME_MODULE_TYPES_ON_BUS, canData.rx.sourceAddress, (EMD_TYPE*MAX_NUMBER_OF_SAME_MODULE_TYPES_ON_BUS)+(MAX_NUMBER_OF_SAME_MODULE_TYPES_ON_BUS-1))) {
			if (operationalData.presenceCounter_EMD < MAX_PRESENCE_COUNT) {
				operationalData.presenceCounter_EMD++;
			}
		} else {
			if (operationalData.presenceCounter_EMD > 0) {
				operationalData.presenceCounter_EMD--;
			}
		}
		if (InRange(AFRC_LITE_TYPE*MAX_NUMBER_OF_SAME_MODULE_TYPES_ON_BUS, canData.rx.sourceAddress, (AFRC_LITE_TYPE*MAX_NUMBER_OF_SAME_MODULE_TYPES_ON_BUS)+(MAX_NUMBER_OF_SAME_MODULE_TYPES_ON_BUS-1))) {
			if (operationalData.presenceCounter_AFRC_Lite < MAX_PRESENCE_COUNT) {
				operationalData.presenceCounter_AFRC_Lite++;
			}
			rxFromAFRCorEMD = TRUE;
		} else {
			if (operationalData.presenceCounter_AFRC_Lite > 0) {
				operationalData.presenceCounter_AFRC_Lite--;
			}
		}
		if (InRange(AFRC_ADVANCED_TYPE*MAX_NUMBER_OF_SAME_MODULE_TYPES_ON_BUS, canData.rx.sourceAddress, (AFRC_ADVANCED_TYPE*MAX_NUMBER_OF_SAME_MODULE_TYPES_ON_BUS)+(MAX_NUMBER_OF_SAME_MODULE_TYPES_ON_BUS-1))) {
			if (operationalData.presenceCounter_AFRC_Advanced < MAX_PRESENCE_COUNT) {
				operationalData.presenceCounter_AFRC_Advanced++;
			}
			rxFromAFRCorEMD = TRUE;
		} else {
			if (operationalData.presenceCounter_AFRC_Advanced > 0) {
				operationalData.presenceCounter_AFRC_Advanced--;
			}
		}
		if (!rxFromAFRCorEMD) {
			if (operationalData.exhaustTemperatureRxCounter[TC_PRECAT] > 0) {
				operationalData.exhaustTemperatureRxCounter[TC_PRECAT]--;
			}
			if (operationalData.exhaustTemperatureRxCounter[TC_POSTCAT] > 0) {
				operationalData.exhaustTemperatureRxCounter[TC_POSTCAT]--;
			}
		}
	#else
		if (InRange(AFRC_LITE_TYPE*MAX_NUMBER_OF_SAME_MODULE_TYPES_ON_BUS, canData.rx.sourceAddress, (AFRC_LITE_TYPE*MAX_NUMBER_OF_SAME_MODULE_TYPES_ON_BUS)+(MAX_NUMBER_OF_SAME_MODULE_TYPES_ON_BUS-1))) {
			rxFromAFRCorEMD = TRUE;
		}
		if (InRange(AFRC_ADVANCED_TYPE*MAX_NUMBER_OF_SAME_MODULE_TYPES_ON_BUS, canData.rx.sourceAddress, (AFRC_ADVANCED_TYPE*MAX_NUMBER_OF_SAME_MODULE_TYPES_ON_BUS)+(MAX_NUMBER_OF_SAME_MODULE_TYPES_ON_BUS-1))) {
			rxFromAFRCorEMD = TRUE;
		}
	#endif

	if (!rxFromAFRCorEMD) {
		if (operationalData.exhaustTemperatureRxCounter[TC_PRECAT] > 0) {
			operationalData.exhaustTemperatureRxCounter[TC_PRECAT]--;
		}
		if (operationalData.exhaustTemperatureRxCounter[TC_POSTCAT] > 0) {
			operationalData.exhaustTemperatureRxCounter[TC_POSTCAT]--;
		}
	}

	pgn = (canData.rx.PDUFormat<<8) + canData.rx.PDUSpecific;

	result = decodePGN(pgn, pMessage->msgEID.DLC, (pgnDataType *)pMessage->data);

	return(result);
}

//-------------------------------------------------------------------------------------------------
static boolean updateTxTimeAndTxIfTRUE(pgnEnumType pgnTableIndex, U16 updateRate_ms) {
	S16 temp16;

	VALIDATE_PGN_TABLE_INDEX(pgnTableIndex);

	if (supportedPGNS[pgnTableIndex].transmitRate <= 0) {
		return(FALSE);
	}
		
	temp16 = canData.pgnWaitTimeBeforeNextTransmit[pgnTableIndex] - updateRate_ms;
	if (temp16 > 0) {
		canData.pgnWaitTimeBeforeNextTransmit[pgnTableIndex] = temp16;
		return(FALSE);
	} else {
		canData.pgnWaitTimeBeforeNextTransmit[pgnTableIndex] = supportedPGNS[pgnTableIndex].transmitRate;
		return(TRUE);
	}
}

//-------------------------------------------------------------------------------------------------
static ErrorCodeIntType _sendMessage(CANTxMessageBuffer *pMessage) {
	CANTxMessageBuffer *pTXMessage;

	/* Get a pointer to the next buffer in the channel and check if the returned value is null. */
	pTXMessage = CANGetTxMessageBuffer(MY_CAN_MODULE, CAN_CHANNEL0);

	if (pTXMessage == NULL) {
		#ifdef FLUSH_TX_BUFFER_IF_NONE_AVAILABLE
			// Flush the TX buffers
			CANUpdateChannel(MY_CAN_MODULE, CAN_CHANNEL0);
			CANFlushTxChannel(MY_CAN_MODULE, CAN_CHANNEL0);
		#endif
		return(BUFFER_UNAVAILABLE);
	}

	memcpy(pTXMessage, pMessage, sizeof(CANTxMessageBuffer));

	// Make darn sure the IDE bit is always set!
	pTXMessage->msgEID.IDE = 1;

	/* This function lets the CAN module
		* know that the message processing is done
		* and message is ready to be processed. */
	CANUpdateChannel(MY_CAN_MODULE, CAN_CHANNEL0);

	/* Direct the CAN module to flush the
		* TX channel. This will send any pending
		* message in the TX channel. */
	// 2010-11-23: Why does this take so long?  What can be done about it?
	CANFlushTxChannel(MY_CAN_MODULE, CAN_CHANNEL0);

	// Give another task a chance
	taskYIELD();
	return(PASS);
}

// Last Line
