/**********************************************************************
* This file was created from CANFunctions.h in the Microchip Explorer16
* library.
************************************************************************/

#ifndef MY_CAN_H
#define MY_CAN_H

//=================================================================================================
// Configuration #defines
//=================================================================================================
#define VALIDATE_PGN_TABLE_INDEX(a)	if ((a>=MAX_PGNS_SUPPORTED_PER_MODULE) ||	\
										(a>=NUMBER_OF_PGNS_SUPPORTED_BY_THIS_MODULE)) {a=0;}

#define PUT_RX_UPDATE_INSIDE_INTERRUPT

#ifdef PRODUCTION_TEST
	#define CAN_BLOCK_TIME_MS	5
	#define MAX_PRESENCE_COUNT	25
	#define CAN_STARTUP_TIME_MS	500
#else
	#ifdef USE_SLOW_UPDATE_RATE
		#define CAN_BLOCK_TIME_MS		5	// 2 is lowest value tested
	#else
		#if defined(MAXIMUM_STRESS_TASK_SCHEDULING) || defined(STRESS_TASK_SCHEDULING) || defined(USE_50PCT_BUS_LOADING)
			#define CAN_BLOCK_TIME_MS	2
		#else
			#define CAN_BLOCK_TIME_MS	5	// 2 is lowest value tested
		#endif
	#endif
	#define CAN_STARTUP_TIME_MS	5000
#endif

//CAN Operating Mode doesn't work quite right
#define IGNORE_RETRY_FAILURE
//#define FLUSH_TX_BUFFER_IF_NONE_AVAILABLE
#ifdef PRODUCTION_TEST
	#define ACCEPT_ALL_MESSAGES	// Works all the time but allows too much traffic
#else
	#ifdef ENABLE_NOX
		#define ACCEPT_ALL_MESSAGES	// Works all the time but allows too much traffic
	#endif
	#define ACCEPT_ANY_PREPOST_TC
#endif
#ifdef CRAPOLA	// 2013-10-24 Testing
	#ifndef ACCEPT_ALL_MESSAGES
			#define ACCEPT_ALL_MESSAGES
	#endif
#endif

//=================================================================================================

#define SPN_GET_U8_VALUE(a)		*(U8 *)ioTable[a].pValue
#define SPN_GET_U16_VALUE(a)	*(U16 *)ioTable[a].pValue
#define SPN_GET_U32_VALUE(a)	*(U32 *)ioTable[a].pValue
#define SPN_GET_U64_VALUE(a)	*(U64 *)ioTable[a].pValue

#define RX_INCREMENT_CAN_HEAD_FROM_ISR							\
	canData.rx.head++;											\
	if (canData.rx.head >= SIZE_OF_CAN_MESSAGE_RX_QUEUE) {		\
		canData.rx.head = 0;									\
	}															\
	if (canData.rx.tail == canData.rx.head) {					\
		canData.rx.tail = canData.rx.head + 1;					\
		if (canData.rx.tail >= SIZE_OF_CAN_MESSAGE_RX_QUEUE) {	\
			canData.rx.tail = 0;								\
		}														\
	}

#define TX_INCREMENT_CAN_HEAD									\
	canData.tx.head++;											\
	if (canData.tx.head >= SIZE_OF_CAN_MESSAGE_TX_QUEUE) {		\
		canData.tx.head = 0;									\
	}															\
	if (canData.tx.tail == canData.tx.head) {					\
		canData.tx.tail = canData.tx.head + 1;					\
		if (canData.tx.tail >= SIZE_OF_CAN_MESSAGE_TX_QUEUE) {	\
			canData.tx.tail = 0;								\
		}														\
	}

#define TX_INCREMENT_CAN_TAIL									\
	canData.tx.tail++;											\
	if (canData.tx.tail >= SIZE_OF_CAN_MESSAGE_TX_QUEUE) {		\
		canData.tx.tail = 0;									\
	}

#define RX_INCREMENT_CAN_HEAD									\
	INTEnable(INT_SOURCE_CAN(MY_CAN_MODULE), INT_DISABLED);		\
	canData.rx.head++;											\
	if (canData.rx.head >= SIZE_OF_CAN_MESSAGE_RX_QUEUE) {		\
		canData.rx.head = 0;									\
	}															\
	if (canData.rx.tail == canData.rx.head) {					\
		canData.rx.tail = canData.rx.head + 1;					\
		if (canData.rx.tail >= SIZE_OF_CAN_MESSAGE_RX_QUEUE) {	\
			canData.rx.tail = 0;								\
		}														\
	}															\
	INTEnable(INT_SOURCE_CAN(MY_CAN_MODULE), INT_ENABLED);

#define RX_INCREMENT_CAN_TAIL									\
	INTEnable(INT_SOURCE_CAN(MY_CAN_MODULE), INT_DISABLED);		\
	canData.rx.tail++;											\
	if (canData.rx.tail >= SIZE_OF_CAN_MESSAGE_RX_QUEUE) {		\
		canData.rx.tail = 0;									\
	}															\
	INTEnable(INT_SOURCE_CAN(MY_CAN_MODULE), INT_ENABLED);

#ifdef DEBUG_RX_SPNS
	#define MAX_NUMBER_OF_SPNS	64
	#define SPN_DATA_SIZE		8	// bytes
	typedef struct {
		U16 head;
		U8 count;
		struct {
			spnEnumType spn;
			U16 len;
			U8 data[SPN_DATA_SIZE];
		} store[MAX_NUMBER_OF_SPNS];
		U16 runCounter;
		U16 sensorTypeCounter;
	} spnListType;

	#define MAX_NUMBER_OF_MST_SPNS	16
	#define SPN_DATA_SIZE			8	// bytes
	typedef struct {
		spnEnumType spn;
		U16 head;
		U8 count;
		mstSetupType mst[MAX_NUMBER_OF_MST_SPNS];
	} mstListType;
#endif

#ifdef GLOBAL
int _myCANindex;
	#ifdef DEBUG_RX_SPNS
		spnListType spnList;
	#endif
#else
	extern int _myCANindex;
	#ifdef DEBUG_RX_SPNS
		extern spnListType spnList;
	#endif
#endif

#ifdef HOLD_THIS
			/* Hold everything while this change catches up */
			systemData.operationalState = OS_CONFIGURATION_CHANGE_TIMEOUT;
			systemData.timer.startup_ms = 0;
#endif

//-------------------------------------------------------------------------------------------------
// CAN Addressing
//-------------------------------------------------------------------------------------------------
// Programmer's Note: If anything other than 4 is used here, the CAN Filtering must be changed
// to mask on the new value.
#define MAX_NUMBER_OF_SAME_MODULE_TYPES_ON_BUS	4

#define MAX_SPN_DATA_SIZE	4	// bytes

// Note: There cannot be more than 239/4 (59) different module types because the address must fit within
// a single byte.

// MY_ADDRESS is a value from 0-3.  It is added to MY_MODULE_TYPE to determine the module's Source Address
#define MY_BASE_ADDRESS			0		// Will be supplemented by the address configuration switches
#define MY_SOURCE_ADDRESS		((MY_MODULE_TYPE*MAX_NUMBER_OF_SAME_MODULE_TYPES_ON_BUS) + MY_BASE_ADDRESS)
#define CAN_GLOBAL_ADDRESS		0xFF	// All modules must recognize data sent to this address
#define UNINOX_CAN_ADDRESS		0x3D

#ifdef USE_PRODUCTION_TEST_HW
	#define CAN_ACTIVITY_TIMEOUT_MS	1000
#else
	#define CAN_ACTIVITY_TIMEOUT_MS	12500	// CAN LED blinks slowly after this timeout
#endif
#if (CAN_ACTIVITY_TIMEOUT_MS >= MAX_S16)
	#error CAN Activity Timeout is too high.  It must be less than MAX_S16.
#endif

#define CAN_STARTUP_TIMEOUT_MS	1000

//-------------------------------------------------------------------------------------------------
// Hardware Setup
//-------------------------------------------------------------------------------------------------
#define CAN_BUS_SPEED 250000

// Priority - 000 = Highest, 111 = Lowest
// Priorities may be added as long as the number is less than 7
typedef enum {
	SYSTEM_PRIORITY,			// Absolute priority over all other traffic on the network
	IMPORTANT_PRIORITY,			// Non-EIM modules should consider this as their own highest priority
	OPERATIONAL_DATA_PRIORITY,	// Operational data such as exhaust temp, O2 readings.
	CONFIGURATION_PRIORITY		// Used for data that is not time critical
} priorityEnumType;

#define SIZE_OF_CAN_MESSAGE_RX_QUEUE	64
#define SIZE_OF_CAN_MESSAGE_TX_QUEUE	64

typedef struct {
	ErrorCodeIntType (*open)(void);
	ErrorCodeIntType (*read)(void);
	ErrorCodeIntType (*write)(U8, spnEnumType, dataUnionType);
	ErrorCodeIntType (*writeMST)(U8, spnEnumType, U8 *);
	ErrorCodeIntType (*sendPGN)(pgnEnumType);
} canType;

typedef struct {
	struct {
		int read;
		int write;
		int sendCANoverRS232;	// For development purposes
	} flags;
	int errorCounter;
	int failReason;
	xTaskHandle xHandle;
	U16 startupTimer_ms;
	int pgnWaitTimeBeforeNextTransmit[MAX_PGNS_SUPPORTED_PER_MODULE];	// Must be int!
	struct {
		xSemaphoreHandle semaphore;
		float rxFloat;			// Last value received - for debugging via serial port
		spnEnumType spn;
		U8 head;
		U8 tail;
		U16 counter;
		U16 errorCounter;
		U16 timeSinceLastRx_ms;
		U16 PDUFormat;
		U16 PDUSpecific;
		U8 sourceAddress;
	} rx;
	struct {
		spnEnumType spn;
		U8 head;
		U8 tail;
		U16 counter;
	} tx;
} canDataType;

#define CAN_STRUCT_DEFAULTS		\
{								\
	_open,						\
	_read,						\
	_write,						\
	_writeMST,					\
	_sendPGN,					\
}

typedef struct {
	void (*reset)(U8);
} monitorType;

#define MONITOR_DEFAULTS		\
{								\
	_monitorReset,				\
}

// Public Data and Functions
#ifdef GLOBAL
	CANRxMessageBuffer CANRxBuffer[SIZE_OF_CAN_MESSAGE_RX_QUEUE];
	CANTxMessageBuffer CANTxBuffer[SIZE_OF_CAN_MESSAGE_TX_QUEUE];
	canDataType canData;
#else
	extern CANRxMessageBuffer CANRxBuffer[SIZE_OF_CAN_MESSAGE_RX_QUEUE];
	extern CANTxMessageBuffer CANTxBuffer[SIZE_OF_CAN_MESSAGE_TX_QUEUE];
	extern const monitorType monitor;
	extern canDataType canData;
#endif
extern const canType myCAN;

extern ErrorCodeIntType processMSTcommand(U8 *);
#if (MY_MODULE_TYPE == AFRC_LITE_TYPE) || (MY_MODULE_TYPE == AFRC_ADVANCED_TYPE)
	extern ErrorCodeIntType processNumberOfSteps(spnEnumType, float);
#endif
extern spnEnumType getSpnFromSensorEnumType(sensorEnumType);
extern float SlotConvert_incoming(float, int);
extern float SlotConvert_outgoing(float, int);
extern ErrorCodeIntType ioTableConvert_outgoing(U8 *, spnEnumType);
extern ErrorCodeIntType ioTableConvert_incoming(U8 *, spnEnumType);
ErrorCodeIntType decodePGN(pgnEnumType, int, pgnDataType *);
ErrorCodeIntType decodeSPN(int, pgnDataType *);
ErrorCodeIntType testLoadCANDataIntoDecodeBuffer(spnEnumType, U8 *, int);

#endif
