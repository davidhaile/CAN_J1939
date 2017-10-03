//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
// CAN Tools
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
#include "environ.h"

static ErrorCodeIntType _writeFloatToIOtable(int, float);
static boolean _isUsableSPN(spnEnumType);
ErrorCodeIntType getIndexOfPGNthatContainsSPN(spnEnumType);

//-------------------------------------------------------------------------------------------------
// Convert input value using slot table that is accessed through the spn.
// Returns PASS or FAIL
//-------------------------------------------------------------------------------------------------
// 2012/04/27 Programmer's Note: The CAN outgoing data function should be modified to use this
// version.
//-------------------------------------------------------------------------------------------------
#ifdef ENABLE_RS485_MODBUS
	ErrorCodeIntType ioTableConvert_outgoing(U8 *pDestination, spnEnumType spn) {
		int ioTableIndex;
		ErrorCodeIntType result;
		U8 temp8;
		U16 temp16;
		U32 temp32;
		float tempFloat;
		U8 *pInput;
		U8 *pOutput;
		slotEnumType slotIndex;
		dataUnionType dataUnion;

		result = getSPNMasterTableIndex(spn);
		if (result == FAIL) {
			return(FAIL);
		}
		result = getSLOTindex(spnMasterTable[result].slotIdentifier);
		if (result == FAIL) {
			return(FAIL);
		}
		slotIndex = result;

		result = getSPNmoduleIndex(spn);
		if (result == FAIL) {
			return(FAIL);
		}
		ioTableIndex = result;

		pOutput = pDestination;

		switch (spnMasterTable[slotIndex].slotIdentifier) {
		case SAE_TM04:
		case SAE_EV01:
		case SAE_PC03:
		case SAE_PC04:
		case SAE_PC05:
		case SAE_PC06:
			// 1 byte float
			switch (ioTable[ioTableIndex].dataType) {
			case TYPE_FLOAT:
				dataUnion = *ioTable[ioTableIndex].pValue;

				tempFloat = dataUnion.f;
				tempFloat = SlotConvert_outgoing(tempFloat, slotIndex);
				temp16 = tempFloat;
				pInput	= (U8 *)&temp16;

				*pOutput++ = *pInput++;
				break;
			default:
				*pOutput++ = 0xFF;
				break;
			}
			break;
				
		// 2-byte floats
		case EMIT_GAIN1:
		case EMIT_VOLT1:
		case EMIT_CURRENT1:
		case EMIT_LAMBDA:
		case EMIT_LAMBDA_OFFSET:
		case EMIT_2BYTE_FLOAT:
		case SAE_FQ01:
		case SAE_FQ02:
		case SAE_AD02:
		case SAE_AD05:
		case SAE_DS06:
		case SAE_TM02:
		case SAE_TP02:
		case SAE_PC01:
		case SAE_PC07:
		case SAE_VL02:
		case EMIT_PRESSURE_PSI:
		case EMIT_PRESSURE_HG:
		case SAE_RS01:
		case SAE_VR01:
		case EMIT_TEMPERATURE:
		case EMIT_TEMPERATURE_OFFSET:
		case EMIT_US:
		case SAE_AD04:
		case SAE_EV02:
		case SAE_TM01:
			switch (ioTable[ioTableIndex].dataType) {
			case TYPE_FLOAT:
				dataUnion = *ioTable[ioTableIndex].pValue;

				tempFloat = dataUnion.f;
				tempFloat = SlotConvert_outgoing(tempFloat, slotIndex);
				temp16 = tempFloat;
				pInput	= (U8 *)&temp16;

				*pOutput++ = *pInput++;
				*pOutput++ = *pInput++;
				break;
			default:
				*pOutput++ = 0xFF;
				*pOutput++ = 0xFF;
				break;
			}
			break;

		// 4-byte floats
		case EMIT_4BYTE_FLOAT:
		case SAE_TM09:
			switch (ioTable[ioTableIndex].dataType) {
			case TYPE_FLOAT:
				dataUnion = *ioTable[ioTableIndex].pValue;

				tempFloat = dataUnion.f;
				tempFloat = SlotConvert_outgoing(tempFloat, slotIndex);
				temp32 = tempFloat;
				pInput	= (U8 *)&temp32;

				*pOutput++ = *pInput++;
				*pOutput++ = *pInput++;
				*pOutput++ = *pInput++;
				*pOutput++ = *pInput++;
				break;
			default:
				*pOutput++ = 0xFF;
				*pOutput++ = 0xFF;
				*pOutput++ = 0xFF;
				*pOutput++ = 0xFF;
				break;
			}
			break;
		case EMIT_TEXT:	// TBD
			*pOutput++ = 'E';
			*pOutput++ = 'M';
			*pOutput++ = 'I';
			*pOutput++ = 'T';
			break;

		// 1-byte
		case SAE_BM08:
		case EMIT_U8:
		case EMIT_STATUS:
		case EMIT_CONTROL_STATUS:
		case EMIT_SENSOR_STATUS:
		case EMIT_MULTISETPOINT_CONFIG:
		case SAE_TM08:
		case SAE_TM11:
		case SAE_CD01:
		case SAE_CM01:
		case SAE_CY01:
		case EMIT_DEW_POINT:
		case EMIT_NOX_SENSOR_STATUS:
		case EMIT_NOX_HEATER_STATUS:
		case EMIT_NOX_FMI:
		case EMIT_O2_FMI:
			switch (ioTable[ioTableIndex].dataType) {
			case TYPE_U8:
				temp8 = SPN_GET_U8_VALUE(ioTableIndex);
				*pOutput++ = temp8;
				break;
			default:
				break;
			}
			break;

		// 2 bytes
		case SAE_BM16:
		case EMIT_MS_TO_SECONDS:
			switch (ioTable[ioTableIndex].dataType) {
			case TYPE_U16:
				temp16 = (SPN_GET_U16_VALUE(ioTableIndex)*4)/1000;
				*pOutput++ = (U8)temp16;
				break;
			default:
				break;
			}
			break;
		case EMIT_SPN:
		case EMIT_U16:
		case SAE_CT05:
		case SAE_TM05:
		case SAE_PP01:
		case SAE_PC16:
			switch (ioTable[ioTableIndex].dataType) {
			case TYPE_U16:
				temp16 = SPN_GET_U16_VALUE(ioTableIndex);
				pInput	= (U8 *)&temp16;

				*pOutput++ = *pInput++;
				*pOutput++ = *pInput++;
				break;
			default:
				break;
			}
			break;
		case SAE_BM32:
		case EMIT_U32:
			switch (ioTable[ioTableIndex].dataType) {
			case TYPE_U32:
				dataUnion = *ioTable[ioTableIndex].pValue;
				pInput	= (U8 *)&dataUnion.u32;

				*pOutput++ = *pInput++;
				*pOutput++ = *pInput++;
				*pOutput++ = *pInput++;
				*pOutput++ = *pInput++;
				break;
			default:
				break;
			}
			break;

		case EMIT_MULTISETPOINT_SETUP:
		case EMIT_SENSOR_TYPE:
		case EMIT_BAUDRATE:
		case EMIT_CLOSED_LOOP_STATUS:
		case EMIT_U64:
		case EMIT_STRING:
		case SAE_BM64:
			break;
		} // switch slot identifier

		return(pOutput - pDestination);
	}
#endif // ENABLE_RS485_MODBUS


//-------------------------------------------------------------------------------------------------
// Returns FAIL or the number of bytes used from pSource
//-------------------------------------------------------------------------------------------------
ErrorCodeIntType ioTableConvert_incoming(U8 *pSource, spnEnumType spn) {
	int slotIndex,
		ioTableIndex,
		spnMasterIndex;
	dataUnionType dataUnion;
	U8 *pStart;
	float tempFloat;
	volatile boolean allowThisSPN = FALSE;

	//----------------------------------------------------------
	// Check for acceptable SPN's
	//----------------------------------------------------------
	allowThisSPN = _isUsableSPN(spn);

	pStart = pSource;

	//---------------------------------------
	// Find this spn in the master spn table
	//---------------------------------------
	spnMasterIndex = getSPNMasterTableIndex(spn);

	if (spnMasterIndex < 0) {
		return(FAIL);
	}

	slotIndex = getSLOTindex(spnMasterTable[spnMasterIndex].slotIdentifier);
	if (slotIndex < 0) {
		return(FAIL);
	}

	//-----------------------------------------------
	// Find this spn in this module's IO table
	//-----------------------------------------------
	ioTableIndex = getSPNmoduleIndex(spn);

	if ((ioTableIndex < 0) ||
		!allowThisSPN) {
		// This is a special case.  The SPN is valid but is not supported by this module.  Return the
		// number of bytes in the slot table so the calling function can move to the next spn.
		return(slotTable[slotIndex].bytes);
	}

	switch (slotTable[slotIndex].bytes) {
	case 1:
		dataUnion.u8 = *pSource++;
		tempFloat = dataUnion.u8;
		tempFloat = SlotConvert_incoming(tempFloat, slotIndex);
		break;
	case 2:
		dataUnion.u16 = *pSource++;
		dataUnion.u16 += (*pSource++)<<8;

		tempFloat = dataUnion.u16;
		tempFloat = SlotConvert_incoming(tempFloat, slotIndex);
		break;
	case 4:
		dataUnion.u32 = *pSource++;
		dataUnion.u32 += (*pSource++)<<8;
		dataUnion.u32 += (*pSource++)<<16;
		dataUnion.u32 += (*pSource++)<<24;

		tempFloat = dataUnion.u32;
		tempFloat = SlotConvert_incoming(tempFloat, slotIndex);
		break;
	default:
		return(FAIL);
	}

	_writeFloatToIOtable(ioTableIndex, tempFloat);

	return(pSource - pStart);
}

//-------------------------------------------------------------------------------------------------
// Converts an incoming value (from CAN) to the locally stored value
//-------------------------------------------------------------------------------------------------
float SlotConvert_incoming(float input, int slotIndex) {
	float returnValue = 0.0;
	float tempFloat2;

	if (InRange(0, slotIndex, LAST_SLOT_INDEX)) {
		 returnValue = input * slotTable[slotIndex].gain;
		 returnValue += slotTable[slotIndex].offset;
	}

	// Special Case - check configurationData.temperatureIsFarenheitIfTrue
	// Internally stored temperatures are always in C. Convert to F for In/Out.
	switch (slotTable[slotIndex].slotIdentifier) {
	case EMIT_TEMPERATURE_OFFSET:
		// Without +32
		if (configurationData.temperatureIsFarenheitIfTrue) {
			tempFloat2	= returnValue*(5.0/9.0);
			returnValue	= tempFloat2;
		}
		break;
	case EMIT_TEMPERATURE:
		if (configurationData.temperatureIsFarenheitIfTrue) {
			tempFloat2	= (returnValue-32.0)*(5.0/9.0);
			returnValue	= tempFloat2;
		}
		break;
	default:
		break;
	}
	return(returnValue);
}

//-------------------------------------------------------------------------------------------------
// Prepares a locally stored value to the format that is used for CAN transmit
//-------------------------------------------------------------------------------------------------
float SlotConvert_outgoing(float input, int slotIndex) {
	float returnValue = 0.0;
	float tempFloat;
	if (InRange(0, slotIndex, LAST_SLOT_INDEX)) {
		// Special Case - check configurationData.temperatureIsFarenheitIfTrue
		// Internally stored temperatures are always in C. Convert to F for In/Out.
		switch (slotTable[slotIndex].slotIdentifier) {
		case EMIT_TEMPERATURE_OFFSET:
			// Without +32
			if (configurationData.temperatureIsFarenheitIfTrue) {
				tempFloat = (9.0/5.0)*input;
				input = tempFloat;
			}
			break;
		case EMIT_TEMPERATURE:
			if (configurationData.temperatureIsFarenheitIfTrue) {
				tempFloat = ((9.0/5.0)*input)+32.0;
				input = tempFloat;
			}
			break;
		default:
			break;
		}

		if (slotTable[slotIndex].minimum != float_IGNORE) {
			ClampLow(input, slotTable[slotIndex].minimum);
		}
		if (slotTable[slotIndex].maximum != float_IGNORE) {
 			ClampHigh(input, slotTable[slotIndex].maximum);
		}
		input -= slotTable[slotIndex].offset;
		input /= slotTable[slotIndex].gain;
		returnValue = input;
	}
	return(returnValue);
}
	
//-------------------------------------------------------------------------------------------------
// Decodes incoming data packet that contains an SPN and a value that is written to a destination
// using the ioTable[] table.
//-------------------------------------------------------------------------------------------------
#ifdef __DEBUG
	volatile int dwhStop = 0;
#endif
ErrorCodeIntType decodeSPN(int length, pgnDataType *pData) {
	spnEnumType spn;
#ifdef FIND_PGN_AND_ZERO_TX_TIMER
	int pgnTableIndex;
#endif
	int ioTableIndex,
		spnMasterIndex,
		slotIndex,
		result;
	volatile dataUnionType dataUnion;
	U8 *pSource;
	float tempFloat = 0.0;
	#ifdef DEBUG_RX_SPNS
		U8 *pTemp;
	#endif
	
	if (length <= 2) {
		// The SPN is two bytes so if the length is 2 or less, then no data has been passed to this function.
		return(FAIL);
	}

	pSource = (U8 *)pData;

	spn = (spnEnumType)*pSource++;
	spn += (spnEnumType)*pSource++ * 256;

	//---------------------------------------------
	// Debugging
	//---------------------------------------------
	#ifdef __DEBUG
		dwhStop = 11;
		canData.rx.spn = spn;
		switch (spn) {
		case SPN_RUN_SIGNAL:
			spnList.runCounter++;
		case SPN_MINIMUM_EXHAUST_TEMPERATURE:
//		case SPN_LEFT_NARROWBAND_VOLTAGE_SETPOINT:
//		case SPN_RIGHT_NARROWBAND_VOLTAGE_SETPOINT:
//		case SPN_NUMBER_OF_TEETH_ON_FLYWHEEL:
//		case SPN_SETPOINT_OFFSET:
//		case SPN_ERROR_RELAY:
//		case SPN_ENABLE_AUTOCONTROL_MODE:
			// These removed from the debugging log only because they occur so frequently
			dwhStop = 99;
			break;
		case SPN_PRODUCTION_TEST_STATE_MASTER:
		case SPN_PRODUCTION_TEST_STATE_SLAVE:
			canData.rx.spn = spn;
			dwhStop = 111;
			break;
		#ifdef IGNORE_THESE
			case SPN_ERROR_RELAY:
				dwhStop = 444;
				break;
			case SPN_TEMPERATURE_CONFIG_C_OR_F:
				dwhStop = 555;
				break;
		//		case SPN_USE_POST_CATALYST_SENSOR:
		//		case SPN_EXHAUST_TEMPERATURE_PRECAT_OFFSET:
				dwhStop = 222;
				break;
			case SPN_PRODUCTION_TEST_SET_SENSOR_HEATER_PWM:
				dwhStop = 333;
				break;
			case SPN_PRODUCTION_TEST_SET_DPV_PHASE:
				canData.rx.spn = spn;
				dwhStop = 77;
				break;
			case SPN_PRODUCTION_TEST_STATE_MASTER:
			case SPN_PRODUCTION_TEST_STATE_SLAVE:
				canData.rx.spn = spn;
				dwhStop = 111;
				break;
			case SPN_PRODUCTION_TEST_SELECT_DPV:
			case SPN_PID_RESPONSE_TIME_LEFT_BANK_1:
			case SPN_PID_RESPONSE_TIME_RIGHT_BANK_2:
				canData.rx.spn = spn;
				dwhStop = 88;
				break;
			case SPN_AUTO_MANUAL_MODE:
				#ifdef __DEBUG
					if (operationalData.debug.enable) {
						operationalData.debug.breakpoint = 999;
					}
				#endif
				tempFloat = 88.8;	// Breakpoint
				break;
		#endif
		case SPN_O2_SENSOR_TYPE:
			spnList.sensorTypeCounter++;
		default:
		#ifdef DEBUG_RX_SPNS
			if (spnList.head >= MAX_NUMBER_OF_SPNS) {
				spnList.head = 0;
			}
			pTemp = pSource;
			spnList.store[spnList.head].spn		= spn;
			spnList.store[spnList.head].len		= length;
			spnList.store[spnList.head].data[0]	= *pTemp++;
			spnList.store[spnList.head].data[1]	= *pTemp++;
			spnList.store[spnList.head].data[2]	= *pTemp++;
			spnList.store[spnList.head].data[3]	= *pTemp++;
			spnList.store[spnList.head].data[4]	= *pTemp++;
			spnList.store[spnList.head].data[5]	= *pTemp++;
			spnList.store[spnList.head].data[6]	= *pTemp++;
			spnList.store[spnList.head].data[7]	= *pTemp++;
			spnList.head++;
			if (spnList.count < MAX_U8) {
				spnList.count++;
			}
		#endif
			break;
		} // switch
	#else
		#ifdef DEBUG_RX_SPNS
			canData.rx.spn = spn;
			switch (spn) {
			case SPN_RUN_SIGNAL:
				// This one removed only because it occurs so frequently
				spnList.runCounter++;
				break;
			case SPN_O2_SENSOR_TYPE:
				spnList.sensorTypeCounter++;
			default:
				if (spnList.head >= MAX_NUMBER_OF_SPNS) {
					spnList.head = 0;
				}
				pTemp = pSource;
				spnList.store[spnList.head].spn		= spn;
				spnList.store[spnList.head].len		= length;
				spnList.store[spnList.head].data[0]	= *pTemp++;
				spnList.store[spnList.head].data[1]	= *pTemp++;
				spnList.store[spnList.head].data[2]	= *pTemp++;
				spnList.store[spnList.head].data[3]	= *pTemp++;
				spnList.store[spnList.head].data[4]	= *pTemp++;
				spnList.store[spnList.head].data[5]	= *pTemp++;
				spnList.store[spnList.head].data[6]	= *pTemp++;
				spnList.store[spnList.head].data[7]	= *pTemp++;
				spnList.head++;
				if (spnList.count < MAX_U8) {
					spnList.count++;
				}
				break;
			} // switch
		#endif
	#endif

	//---------------------------------------------
	//---------------------------------------------

	length -= 2;

	//---------------------------------------
	// Find this spn in the master spn table
	//---------------------------------------
	result = getSPNMasterTableIndex(spn);
	if (!result) {
		// SPN not found in master spn table
		return(FAIL);
	}

	spnMasterIndex = result;

	switch (spn) {
	case SPN_MULTISETPOINT_TABLE_SETUP:
		// Special case where we don't care about the data or length within
		break;
	default:
		ioTableIndex = getSPNmoduleIndex(spn);
		if (ioTableIndex == FAIL) {
			// SPN not found in data table
			return(FAIL);	
		}

		// Find slot
		slotIndex = getSLOTindex(spnMasterTable[spnMasterIndex].slotIdentifier);
		if (slotIndex == FAIL) {
			// SPN not found in slotTable
			return(FAIL);
		}

		dataUnion.u32 = 0;
		switch (length) {
		case 1:
			dataUnion.u8 = *pSource++;

			if (tempFloat == 88.8) {
				if (dataUnion.u8 == 1) {
					tempFloat = 77.7;
				} else {
					tempFloat = 77.8;
				}
			} else {
				tempFloat = 77.6;
			}
			tempFloat = dataUnion.u8;
			tempFloat = SlotConvert_incoming(tempFloat, slotIndex);

			ETS_ENTER_CRITICAL;
				switch (ioTable[ioTableIndex].dataType) {
				case TYPE_U8:
					// Rely on float-to-integer conversion feature of the compiler
					dataUnion.u8 = tempFloat;
						*(U8 *)ioTable[ioTableIndex].pValue = dataUnion.u8;
					break;
				case TYPE_U16:
					// Rely on float-to-integer conversion feature of the compiler
					dataUnion.u16 = tempFloat;
						*(U8 *)ioTable[ioTableIndex].pValue = dataUnion.u16;
					break;
				case TYPE_U32:
					// Rely on float-to-integer conversion feature of the compiler
					dataUnion.u32 = tempFloat;
						*(U8 *)ioTable[ioTableIndex].pValue = dataUnion.u32;
					break;
				default:
					// Do nothing
					break;
				}
			ETS_EXIT_CRITICAL;
			break;
		case 2:
			dataUnion.u16 = *pSource++;
			dataUnion.u16 += (*pSource++)<<8;

			tempFloat = dataUnion.u16;
			tempFloat = SlotConvert_incoming(tempFloat, slotIndex);

		#if (MY_MODULE_TYPE == AFRC_LITE_TYPE) || (MY_MODULE_TYPE == AFRC_ADVANCED_TYPE)
			switch (spn) {
			// Special conversion for number of DPV setps
			case SPN_DPV_HOME_POSITION_STEPS:
			case SPN_DPV_DESIRED_POSITION_LEFT_BANK_1_STEPS:
			case SPN_DPV_DESIRED_POSITION_RIGHT_BANK_2_STEPS:
				processNumberOfSteps(spn, tempFloat);
				break;

			default:
		#endif
				ETS_ENTER_CRITICAL;
					switch (ioTable[ioTableIndex].dataType) {
					case TYPE_U8:
						// Rely on float-to-integer conversion feature of the compiler
						dataUnion.u8 = tempFloat;
						*(U8 *)ioTable[ioTableIndex].pValue = dataUnion.u8;
						break;
					case TYPE_U16:
						// Rely on float-to-integer conversion feature of the compiler
						dataUnion.u16 = tempFloat;
						*(U16 *)ioTable[ioTableIndex].pValue = dataUnion.u16;
						break;
					case TYPE_FLOAT:
						dataUnion.f = tempFloat;
						*(float *)ioTable[ioTableIndex].pValue = dataUnion.f;
						break;
					default:
						break;
					}
				ETS_EXIT_CRITICAL;
		#if (MY_MODULE_TYPE == AFRC_LITE_TYPE) || (MY_MODULE_TYPE == AFRC_ADVANCED_TYPE)
				break;
			}
		#endif
			break;
		case 4:
			dataUnion.u32 = *pSource++;
			dataUnion.u32 += (*pSource++)<<8;
			dataUnion.u32 += (*pSource++)<<16;
			dataUnion.u32 += (*pSource++)<<24;

			tempFloat = dataUnion.u32;
			tempFloat = SlotConvert_incoming(tempFloat, slotIndex);

			switch (spn) {
			case SPN_EMIT_SERIAL_NUMBER:
				// Special method to write serialNumber
				if (myNvm.writeSerialNumber(dataUnion.u32) != PASS) {
					// If it didn't work, restore to the nvm version
					dataUnion.u32 = myNvm.readSerialNumber();
					tempFloat = dataUnion.u32;
				}
				// Fall through on purpose
			default:
				ETS_ENTER_CRITICAL;
					switch (ioTable[ioTableIndex].dataType) {
					case TYPE_U8:
						// Rely on float-to-integer conversion feature of the compiler
						dataUnion.u8 = tempFloat;
						*(U8 *)ioTable[ioTableIndex].pValue = dataUnion.u8;
						break;
					case TYPE_U16:
						// Rely on float-to-integer conversion feature of the compiler
						dataUnion.u16 = tempFloat;
						*(U16 *)ioTable[ioTableIndex].pValue = dataUnion.u16;
						break;
					case TYPE_U32:
						// Rely on float-to-integer conversion feature of the compiler
						dataUnion.u32 = tempFloat;
						*(U32 *)ioTable[ioTableIndex].pValue = dataUnion.u32;
						break;
					case TYPE_FLOAT:
						dataUnion.f = tempFloat;
						*(float *)ioTable[ioTableIndex].pValue = dataUnion.f;
						break;
					default:
						break;
					}
				ETS_EXIT_CRITICAL;
				break;
			}
			break;
		default:
			return(FAIL);
			break;
		}
	} // switch (spn)

	// Decode specific SPN's
#if (MY_MODULE_TYPE == AFRC_LITE_TYPE) || (MY_MODULE_TYPE == AFRC_ADVANCED_TYPE)
	switch (spn) {
	case SPN_MULTISETPOINT_TABLE_SETUP:
		return(processMSTcommand(pSource));
	case SPN_AUTO_MANUAL_MODE:
		myCAN.sendPGN(AFRC_STATUS_1);
	case SPN_RUN_SIGNAL:
	case SPN_ERROR_RELAY:
		// Do nothing
		break;
	default:
		break;
	}
#else
	// Reset the datalog timer for any SPN that changes the configuration
	USBStatus.datalogTick = 0;
	USBStatus.log.timeSinceLastWrite_ms = 0;
	USBStatus.log.timeSinceLastWrite_seconds = 0;
	USBStatus.log.flags.requestFileUpdate = FALSE;
	USBStatus.configuration.timeSinceLastWrite_ms = 0;
	USBStatus.configuration.timeSinceLastWrite_seconds = 0;
	USBStatus.configuration.flags.requestFileUpdate = FALSE;
#endif

//#define FIND_PGN_AND_ZERO_TX_TIMER
#ifdef FIND_PGN_AND_ZERO_TX_TIMER
	//------------------------------------------------------------------------------------------------
	// 2013-05-01 Programmer's Note: This works but it has the potential to load the CAN bus.
	//------------------------------------------------------------------------------------------------

	//------------------------------------------------------------------------------------------------
	// Find the associated PGN that contains the SPN just received and reset the RX timer to cause an
	// immediate TX of the PGN after this function is completel. Only zero the timer if it is greater
	// than 250ms. To make it otherwise risks too many CAN transmits.
	//------------------------------------------------------------------------------------------------
	pgnTableIndex = getIndexOfPGNthatContainsSPN(spn);
	if (pgnTableIndex < 0) {
		// SPN not found in master spn table
		return(FAIL);
	}

	VALIDATE_PGN_TABLE_INDEX(pgnTableIndex);

	if (canData.pgnWaitTimeBeforeNextTransmit[pgnTableIndex] > 250) {
		canData.pgnWaitTimeBeforeNextTransmit[pgnTableIndex] = 0;
	}
#endif

	// Debugging
	canData.rx.rxFloat = tempFloat;

	return(PASS);
}

//-------------------------------------------------------------------------------------------------
// Decodes incoming data packet
//-------------------------------------------------------------------------------------------------
ErrorCodeIntType decodePGN(pgnEnumType pgn, int length, pgnDataType *pData) {
	spnEnumType spn;
	int i,
		spnIndex;
	U16 pgnTableIndex = 0;
	U8 *pSource;
	boolean result;
	ErrorCodeIntType returnValue = PASS;
	U16 PDUFormat;

	canData.flags.read++;

	PDUFormat = (pgn & 0xff00) >> 8;

	if (PDUFormat == PF_TRANSMIT_DATA) {
		returnValue = decodeSPN(length, pData);
	} else {
		//-----------------
		// Receiving a PGN
		//-----------------
		#ifdef ENABLE_NOX
			// Only accept these PGN's
			switch (pgn) {
			case PGN_AFTERTREATMENT_1_INTAKE_GAS_1:
			case PGN_AFTERTREATMENT_1_OUTLET_GAS_1:
				operationalData.noxSensor.rxCounter++;
				break;
			default:
				return(FAIL);
			}
		#else
			#ifndef PRODUCTION_TEST
				// Only accept these PGN's
				switch (pgn) {
				// Anything that contains SPN_EXHAUST_TEMPERATURE_PRECAT and SPN_EXHAUST_TEMPERATURE_POSTCAT
				case AFRC_TEMPERATURE:
				case EXHAUST_TEMPERATURE_C:
				case EMD_TEMPERATURE_1:
				case EXHAUST_TEMPERATURE_PRECAT_CALIBRATE:
					break;
				default:
					return(FAIL);
				}
			#endif
		#endif

		// Find the PGN in the pgn master table
		result = FALSE;
		for (i=0; (i<NUMBER_OF_PGNS_IN_MASTER_TABLE) && (result != TRUE); i++) {
			if (pgn == pgnMasterTable[i].pgn) {
				pgnTableIndex = i;
				result = TRUE;
				break;
			}
		}

		/// PGN was not in the master table
		if (result != TRUE) {
			return(FAIL);
		}

		pSource = (U8 *)pData;

		// Loop through list of SPNs that are within this PGN
		result = FALSE;
		for (spnIndex=0; (spnIndex<MAX_SPNS_PER_PGN) && !result; spnIndex++) {
			spn = pgnMasterTable[pgnTableIndex].spn[spnIndex];

#ifdef __DEBUG
	volatile int dwhNom;
	dwhNom = 1234;
#endif
			if (spn == SPN_INVALID) {
				result = TRUE;
				break;
			} else {
				returnValue = ioTableConvert_incoming(pSource, spn);

				// DWH TBD - is this correct?  If it fails then it is going to fail forever.  Or not?  More thought is needed.
				if (returnValue > 0) {
					pSource += returnValue;
				}
			}
		}
	} // if !PDUFormat

	return(returnValue);
}

//=================================================================================================
pgnDataType decodeBuffer;
ErrorCodeIntType testLoadCANDataIntoDecodeBuffer(spnEnumType spn, U8 *pSource, int length) {
	int i;
	U8 *pDestination;

	pDestination = (U8 *)decodeBuffer;

	*pDestination++ = spn>>8;
	*pDestination++ = spn & 0xff;

	for (i=0; i<length; i++) {
		*pDestination++ = *pSource++;
	}

	decodeSPN(i+2, &decodeBuffer);

	return(PASS);
}

//=================================================================================================
static ErrorCodeIntType _writeFloatToIOtable(int ioTableIndex, float value) {
	dataUnionType dataUnion;

	switch (ioTable[ioTableIndex].dataType) {
	case TYPE_U8:
		// Rely on float-to-integer conversion feature of the compiler
		dataUnion.u8 = value;
		*(U8 *)ioTable[ioTableIndex].pValue = dataUnion.u8;
		break;
	case TYPE_U16:
		// Rely on float-to-integer conversion feature of the compiler
		dataUnion.u16 = value;
		*(U16 *)ioTable[ioTableIndex].pValue = dataUnion.u16;
		break;
	case TYPE_U32:
		// Rely on float-to-integer conversion feature of the compiler
		dataUnion.u32 = value;
		*(U32 *)ioTable[ioTableIndex].pValue = dataUnion.u32;
		break;
	case TYPE_FLOAT:
		dataUnion.f = value;
		*(float *)ioTable[ioTableIndex].pValue = dataUnion.f;
		break;
	default:
		return(FAIL);
	}
	return(PASS);
}

//-------------------------------------------------------------------------------------------------
static boolean _isUsableSPN(spnEnumType spn) {
#ifdef PRODUCTION_TEST
	return(TRUE);
#else
	switch (spn) {
	case SPN_INVALID:
		return(FALSE);

	//==========================================================
	// Special Case: For PGN's that contain exhaust temperature,
	// don't accept non-Pre/Post Cat SPN's and don't accept if
	// the on-board TC is attached.
	//==========================================================
	case SPN_EXHAUST_TEMPERATURE_PRECAT:
		if (operationalData.exhaustTemperatureRxCounter[TC_PRECAT] < 10) {
			operationalData.exhaustTemperatureRxCounter[TC_PRECAT]++;
		}
		return(operationalData.OKtoUseRemotePrePostCatTemperature[TC_PRECAT]);
	case SPN_EXHAUST_TEMPERATURE_POSTCAT:
		if (operationalData.exhaustTemperatureRxCounter[TC_POSTCAT] < 10) {
			operationalData.exhaustTemperatureRxCounter[TC_POSTCAT]++;
		}
		return(operationalData.OKtoUseRemotePrePostCatTemperature[TC_POSTCAT]);

	// Not allowed SPN's within AFRC_TEMPERATURE
	case SPN_AUXILARY_TEMPERATURE_1:
	case SPN_AMBIENT_TEMPERATURE_THERMISTOR:
		return(FALSE);

	// Not allowed SPN's within EXHAUST_TEMPERATURE_C
	case SPN_EXHAUST_DIFFERENTIAL_TEMPERATURE:
		return(FALSE);

	// Not allowed SPN's within EMD_TEMPERATURE_1
	case SPN_FUEL_TEMPERATURE:
	case SPN_AMBIENT_TEMPERATURE:
		return(FALSE);

	// 2013-01-28 Added
	case SPN_RUN_SIGNAL:
		return(TRUE);

	default:
		break;
	}
	return(TRUE);
#endif
}

//-------------------------------------------------------------------------------------------------
ErrorCodeIntType getIndexOfPGNthatContainsSPN(spnEnumType spn) {
	int spnIndex;
	int pgnTableIndex;
	pgnEnumType pgn = 0;
	boolean result = FALSE;

	// Step 1 - find pgn that is associated with this spn
	for (pgnTableIndex=0; pgnTableIndex<NUMBER_OF_PGNS_IN_MASTER_TABLE; pgnTableIndex++) {
		for (spnIndex=0; spnIndex<MAX_SPNS_PER_PGN; spnIndex++) {
			if (spn == pgnMasterTable[pgnTableIndex].spn[spnIndex]) {
				result = TRUE;
				pgn = pgnMasterTable[pgnTableIndex].pgn;
				break;
			}
		}
	}

	// Step 2 - find pgn in this module's supported PGN table
	for (pgnTableIndex=0; pgnTableIndex<NUMBER_OF_PGNS_SUPPORTED_BY_THIS_MODULE; pgnTableIndex++) {
		if (pgn == supportedPGNS[pgnTableIndex].pgn) {
			return(pgnTableIndex);
		}
	}

	return(FAIL);
}

// Last Line

