//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
// PGN.H
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
#ifndef _PGN_H
#define _PGN_H

#include "SPN.h"

//=================================================================================================
//=================================================================================================
// PGN Table
//=================================================================================================
//=================================================================================================

//----------------------------------------------------------------------
// Configuration PGN's
//----------------------------------------------------------------------
// These PGNs are sent to a specific node on the bus.  The proprietary
// PDU Format is 239.

//----------------------------------------------------------------------
// Broadcast PGN's
//----------------------------------------------------------------------
/*
   These PGNs are broadcast with the potential to be received by any
   node on the bus.
*/
// Reference the CAN Protocol document for the definition of these PGNs
// Proprietary PGNs - are PDU2 format and start with PDU Format = 255.
// Ref SAEJ1939/21, 5.3.2: Proprietary PGN range covers 65280 to 65535
//----------------------------------------------------------------------
typedef enum {
	PGN_INVALID							= 0,
	PGN_TACHOGRAPH						= 65132,
	PGN_VEHICLE_SPEED					= 65256,
	EXHAUST_TEMPERATURE_C				= 65280,
	MY_USB_STATUS						= 65281,
	WIDEBAND_O2_SENSOR_BANK_1			= 65282,
	WIDEBAND_O2_SENSOR_BANK_2			= 65283,
	EMD_TEMPERATURE_1					= 65284,
	EMD_TEMPERATURE_2					= 65285,
	ANALOG_VOLTS_1						= 65286,
	ANALOG_VOLTS_2						= 65287,
	EMD_MODULE_IDENTIFICATION			= 65288,
	AFRC_LITE_MODULE_IDENTIFICATION		= 65289,
	AFRC_ADVANCED_MODULE_IDENTIFICATION	= 65290,
	EXHAUST_TEMPERATURE_F				= 65291,
	AFRC_LITE_PID_INFORMATION			= 65292,
	WIDEBAND_NERNST_GAINS_BANK_1		= 65293,
	WIDEBAND_O2_GAINS_BANK_1			= 65294,
	NARROWBAND_O2_GAINS_BANK_1			= 65295,
	NARROWBAND_O2_SENSOR_BANK_1			= 65296,
	NARROWBAND_O2_SENSOR_BANK_2			= 65297,
	AFRC_STATUS_1						= 65298,
	DPV_POSITION_STEPS					= 65299,
	DPV_POSITION						= 65300,
	AFRC_LITE_TASK_COUNTERS_1			= 65301,
	AFRC_LITE_TASK_COUNTERS_2			= 65302,
	WB_HEATER_PID_MONITOR_BANK_1		= 65303,
	WB_LAMBDA_PID_MONITOR_BANK_1		= 65304,
	WB_HEATER_PID_MONITOR_BANK_2		= 65305,
	WB_LAMBDA_PID_MONITOR_BANK_2		= 65306,
	WB_LAMBDA_POST_CATALYST				= 65307,
	WB_HEATER_PID_MONITOR_POSTCAT		= 65308,
	ECU_RUN_TIME						= 65309,
	ENGINE_RUN_TIME						= 65310,
	ANALOG_CURRENT_1					= 65311,
	ANALOG_CURRENT_2					= 65312,
	LED_INDICATORS						= 65313,
	AFRC_TEMPERATURE					= 65314,
	MANIFOLD_PRESSURE					= 65315,
	O2_SENSOR_TYPE						= 65316,
	AFRC_CONFIGURATION_1				= 65317,
	NARROWBAND_O2_SENSOR_POSTCAT		= 65318,
	WIDEBAND_O2_SENSOR_POSTCAT			= 65319,
	WIDEBAND_NERNST_GAINS_BANK_2		= 65320,
	WIDEBAND_NERNST_GAINS_POSTCAT		= 65321,
	WIDEBAND_O2_GAINS_BANK_2			= 65322,
	NARROWBAND_O2_GAINS_BANK_2			= 65323,
	WIDEBAND_O2_GAINS_POSTCAT			= 65324,
	NARROWBAND_O2_GAINS_POSTCAT			= 65325,

	MULTISETPOINT_TABLE_CONFIGURATION	= 65326,
	MULTISETPOINT_TABLE_ROW_0,
	MULTISETPOINT_TABLE_ROW_1,
	MULTISETPOINT_TABLE_ROW_2,
	MULTISETPOINT_TABLE_ROW_3,
	MULTISETPOINT_TABLE_ROW_4,
	MULTISETPOINT_TABLE_SETUP,			// 65332

	EXHAUST_TEMPERATURE_PRECAT_CALIBRATE,// 65333
	EXHAUST_TEMPERATURE_POSTCAT_CALIBRATE,// 65334
	REGULATORY_AVERAGES,				// 65335
	AFRC_CONTROL_STATUS,				// 65336
	EMD_CONTROL_STATUS,					// 65337
	AFRC_STATUS_2,						// 65338
	RAW_VOLTAGE_LEFT_BANK_1,			// 65339
	RAW_VOLTAGE_RIGHT_BANK_2,			// 65340
	RAW_VOLTAGE_POSTCAT,				// 65341
	DATATAG_STATUS,						// 65342
	EMD_CONTROL_STATUS_2,				// 65343
	PRODUCTION_SLAVE,					// 65344
	EMD_ANALOG_CONFIGURATION,			// 65345
	AFRC_AUTOCONTROL,					// 65346
	PRODUCTION_MASTER,					// 65347
	PGN_XY_STRING,						// 65348
	EMD_CONTROL_STATUS_3,				// 65349
	
	//-----------------------------------------------
	// Standard J1939 PGNs
	//-----------------------------------------------
	PGN_ACKNOWLEDGMENT					= 59392,		// PF=232, Has destination address in it
	PGN_TRANSMIT_DATA					= 61440,		// PF=240, Has destination address in it
	PGN_AFTERTREATMENT_1_INTAKE_GAS_1	= 61454,		// Aftertreatment 1 Intake Gas 1 AT1IG1
	PGN_AFTERTREATMENT_1_OUTLET_GAS_1	= 61455,		// Aftertreatment 1 Outlet Gas 1 AT1OG1
	PGN_EXHAUST_TEMPERATURE				= 65031,
	PGN_ENGINE_TEMPERATURE				= 65262,
	ELECTRONIC_ENGINE_CONTROLLER_1		= 61444,
	ENGINE_THROTTLE_FUEL_ACTUATOR_CONTROL_COMMAND = 61466,
	TIME_AND_DATE						= 65254,
	TIME_AND_DATE_ADJUST				= 54528,
	ELECTRICAL_POWER_1					= 64925,
	ELECTRICAL_POWER_2					= 64924,
	VEHICLE_ELECTRICAL_POWER_1			= 65271,
	ELECTRONIC_ENGINE_CONTROLLER_3		= 65247,

	CLEAR_ALL_DTCS						= 65235,		// Clears all DTC's on all modules on the CAN bus
	ACTIVE_DTC							= 65236
} pgnEnumType;

#define PF_ACKNOWLEDGMENT	232
#define PF_REQUEST_PGN		234	// DA,SA with PGN in data field
#define PF_TRANSMIT_DATA	239	//240

//-----------------------------------------------
// Used in a PGN where a particular SPN is not used
//-----------------------------------------------
#define UNUSED_VALUE_8_BITS				0xFF
#define UNUSED_VALUE_16_BITS			0xFFFF
#define UNUSED_VALUE_32_BITS			0xFFFFFFFF
#define FLOAT_CONVERT_TO_16BITS			0.05

//-------------------------------------------------------------------------------------------------
// PGN Table - Assigns SPN's to each PGN
//-------------------------------------------------------------------------------------------------

// 2011/10/17 This is important to keep pgnIndex from incrementing too far in the case of an incomplete table
#define MAX_PGNS_SUPPORTED_PER_MODULE		64

#define MAX_SPNS_PER_PGN	8
typedef struct {
	pgnEnumType pgn;
	spnEnumType	spn[MAX_SPNS_PER_PGN];
} pgnTableType;

#ifdef GLOBAL
	const pgnTableType pgnMasterTable[] = {
		{AFRC_TEMPERATURE,			{SPN_EXHAUST_TEMPERATURE_PRECAT,
									 SPN_EXHAUST_TEMPERATURE_POSTCAT,
									 SPN_AUXILARY_TEMPERATURE_1,
									 SPN_AMBIENT_TEMPERATURE_THERMISTOR}},
		{AFRC_STATUS_1,				{SPN_AUTO_MANUAL_MODE,					// Byte 1
									 SPN_RUN_SIGNAL,						// Byte 2
									 SPN_USE_POST_CATALYST_SENSOR,			// Byte 3
									 SPN_ERROR_RELAY,						// Byte 4
									 SPN_ENGINE_SPEED,						// Byte 5-6
									 SPN_NUMBER_OF_TEETH_ON_FLYWHEEL}},		// Byte 7-8
		{AFRC_STATUS_2,				{SPN_AUTOSTART_DELAY,					// 2 BYTES
									 SPN_BATTERY_POTENTIAL,					// 2 BYTES
									 SPN_ENABLE_AUTOCONTROL_MODE,			// 1 byte
									 SPN_ANALOG_1_IS_4_20MA}},				// 1 byte
		{EXHAUST_TEMPERATURE_C,		{SPN_EXHAUST_TEMPERATURE_PRECAT,
									 SPN_EXHAUST_TEMPERATURE_POSTCAT,
									 SPN_EXHAUST_DIFFERENTIAL_TEMPERATURE}},
		{WIDEBAND_O2_SENSOR_BANK_1,	{SPN_LEFT_WIDEBAND_LAMBDA_SETPOINT,
									 SPN_WIDEBAND_LAMBDA_LEFT_BANK_1,
									 SPN_WIDEBAND_NERNST_CELL_RESISTANCE_LEFT_BANK_1,
									 SPN_SENSOR_STATUS_LEFT_BANK_1_1,
									 SPN_SENSOR_STATUS_LEFT_BANK_1_2}},
		{WIDEBAND_O2_SENSOR_BANK_2,	{SPN_RIGHT_WIDEBAND_LAMBDA_SETPOINT,
									 SPN_WIDEBAND_LAMBDA_RIGHT_BANK_2,
									 SPN_WIDEBAND_NERNST_CELL_RESISTANCE_RIGHT_BANK_2,
									 SPN_SENSOR_STATUS_RIGHT_BANK_2_1,
									 SPN_SENSOR_STATUS_RIGHT_BANK_2_2}},
		{WIDEBAND_O2_SENSOR_POSTCAT,{SPN_POSTCAT_WIDEBAND_LAMBDA_SETPOINT,
									 SPN_WIDEBAND_LAMBDA_POSTCAT,
									 SPN_WIDEBAND_NERNST_CELL_RESISTANCE_POSTCAT,
									 SPN_SENSOR_STATUS_POSTCAT_1,
									 SPN_SENSOR_STATUS_POSTCAT_2}},
		{NARROWBAND_O2_SENSOR_BANK_1,{SPN_LEFT_NARROWBAND_VOLTAGE_SETPOINT,
									 SPN_NARROWBAND_VOLTAGE_LEFT_BANK_1,
									 SPN_SENSOR_STATUS_LEFT_BANK_1_1,
									 SPN_SENSOR_STATUS_LEFT_BANK_1_2}},
		{NARROWBAND_O2_SENSOR_BANK_2,{SPN_RIGHT_NARROWBAND_VOLTAGE_SETPOINT,
									 SPN_NARROWBAND_VOLTAGE_RIGHT_BANK_2,
									 SPN_SENSOR_STATUS_RIGHT_BANK_2_1,
									 SPN_SENSOR_STATUS_RIGHT_BANK_2_2}},
		{NARROWBAND_O2_SENSOR_POSTCAT,{SPN_POSTCAT_NARROWBAND_VOLTAGE_SETPOINT,
									 SPN_NARROWBAND_VOLTAGE_POSTCAT,
									 SPN_SENSOR_STATUS_POSTCAT_1,
									 SPN_SENSOR_STATUS_POSTCAT_2}},
		{EMD_TEMPERATURE_1,			{SPN_EXHAUST_TEMPERATURE_PRECAT,
									 SPN_EXHAUST_TEMPERATURE_POSTCAT,
									 SPN_FUEL_TEMPERATURE,
									 SPN_AMBIENT_TEMPERATURE}},
		{EMD_TEMPERATURE_2,			{SPN_AUXILARY_TEMPERATURE_1,
									 SPN_AUXILARY_TEMPERATURE_2,
								 	 SPN_AMBIENT_TEMPERATURE_THERMISTOR,
									 SPN_FUEL_TEMPERATURE_THERMISTOR}},
		{ANALOG_VOLTS_1,			{SPN_ANALOG_VOLTAGE_1,
									 SPN_ANALOG_VOLTAGE_2,
									 SPN_ANALOG_VOLTAGE_3,
									 SPN_ANALOG_VOLTAGE_4}},
		{ANALOG_VOLTS_2,			{SPN_ANALOG_VOLTAGE_5,
									 SPN_ANALOG_VOLTAGE_6,
									 SPN_ANALOG_VOLTAGE_7,
									 SPN_ANALOG_VOLTAGE_8}},
		{ANALOG_CURRENT_1,			{SPN_ANALOG_CURRENT_1,
									 SPN_ANALOG_CURRENT_2,
									 SPN_ANALOG_CURRENT_3,
									 SPN_ANALOG_CURRENT_4}},
		{ANALOG_CURRENT_2,			{SPN_ANALOG_CURRENT_5,
									 SPN_ANALOG_CURRENT_6,
									 SPN_ANALOG_CURRENT_7,
									 SPN_ANALOG_CURRENT_8}},
		{ECU_RUN_TIME,				{SPN_ECU_UPTIME,
									 SPN_ECU_RUNTIME}},
		{ENGINE_RUN_TIME,			{SPN_ENGINE_UPTIME,
									 SPN_ENGINE_RUNTIME}},
		{LED_INDICATORS,			{SPN_LED_INDICATORS}},
		{DPV_POSITION_STEPS,		{SPN_DPV_DESIRED_POSITION_LEFT_BANK_1_STEPS,
									 SPN_DPV_ACTUAL_POSITION_LEFT_BANK_1_STEPS,
									 SPN_DPV_DESIRED_POSITION_RIGHT_BANK_2_STEPS,
									 SPN_DPV_ACTUAL_POSITION_RIGHT_BANK_2_STEPS}},
		{DPV_POSITION,				{SPN_DPV_DESIRED_POSITION_LEFT_BANK_1,
									 SPN_DPV_ACTUAL_POSITION_LEFT_BANK_1,
									 SPN_DPV_DESIRED_POSITION_RIGHT_BANK_2,
									 SPN_DPV_ACTUAL_POSITION_RIGHT_BANK_2}},

		// Left bank 1
		{WIDEBAND_NERNST_GAINS_BANK_1,{SPN_LEFT_NERNST_KP,
									 SPN_LEFT_NERNST_KI,
									 SPN_LEFT_NERNST_KD}},
		{WIDEBAND_O2_GAINS_BANK_1,	{SPN_LEFT_WIDEBAND_KP,
									 SPN_LEFT_WIDEBAND_KI,
									 SPN_LEFT_WIDEBAND_KD,
									 SPN_PID_RESPONSE_TIME_LEFT_BANK_1}},
		{NARROWBAND_O2_GAINS_BANK_1,{SPN_LEFT_NARROWBAND_KP,
									 SPN_LEFT_NARROWBAND_KI,
									 SPN_LEFT_NARROWBAND_KD,
									 SPN_PID_RESPONSE_TIME_LEFT_BANK_1}},
		// Right bank 2
		{WIDEBAND_NERNST_GAINS_BANK_2,{SPN_RIGHT_NERNST_KP,
									 SPN_RIGHT_NERNST_KI,
									 SPN_RIGHT_NERNST_KD}},
		{WIDEBAND_O2_GAINS_BANK_2,	{SPN_RIGHT_WIDEBAND_KP,
									 SPN_RIGHT_WIDEBAND_KI,
									 SPN_RIGHT_WIDEBAND_KD,
									 SPN_PID_RESPONSE_TIME_RIGHT_BANK_2}},
		{NARROWBAND_O2_GAINS_BANK_2,{SPN_RIGHT_NARROWBAND_KP,
									 SPN_RIGHT_NARROWBAND_KI,
									 SPN_RIGHT_NARROWBAND_KD,
									 SPN_PID_RESPONSE_TIME_RIGHT_BANK_2}},
		// Postcat
		{WIDEBAND_NERNST_GAINS_POSTCAT,{SPN_POSTCAT_NERNST_KP,
									 SPN_POSTCAT_NERNST_KI,
									 SPN_POSTCAT_NERNST_KD}},
		{WIDEBAND_O2_GAINS_POSTCAT,	{SPN_POSTCAT_WIDEBAND_KP,
									 SPN_POSTCAT_WIDEBAND_KI,
									 SPN_POSTCAT_WIDEBAND_KD}},
		{NARROWBAND_O2_GAINS_POSTCAT,{SPN_POSTCAT_NARROWBAND_KP,
									 SPN_POSTCAT_NARROWBAND_KI,
									 SPN_POSTCAT_NARROWBAND_KD}},

		// Everything else
		{AFRC_CONTROL_STATUS,		{SPN_AFRC_CONTROL_STATUS,			// 1 byte
									 SPN_AFRC_CONTROL_STATUS_BANK_1,	// 1 byte
									 SPN_AFRC_CONTROL_STATUS_BANK_2,	// 1 byte
									 SPN_AFRC_CONTROL_STATUS_POSTCAT,	// 1 byte
									 SPN_DISCRETE_INPUT,				// 1 byte
									 SPN_OIL_PRESSURE_SWITCH}},			// 1 BYTE

		{EMD_CONTROL_STATUS,		{SPN_DISCRETE_INPUT,				// 1 byte
									 SPN_OIL_PRESSURE_SWITCH,			// 1 byte
									 SPN_CAT_PRESSURE_CURRENT,			// 2 bytes
									 SPN_FUELFLOW_CURRENT,				// 2 bytes
									 SPN_RUN_SIGNAL}},					// 1 bytes
		{EMD_CONTROL_STATUS_2,		{SPN_ERROR_RELAY,					// 1 byte
									 SPN_ENGINE_SPEED,					// 2 bytes
									 SPN_EXHAUST_DIFFERENTIAL_TEMPERATURE,// 2 bytes
									 SPN_BATTERY_POTENTIAL}},			// 2 bytes
		{EMD_CONTROL_STATUS_3,		{SPN_FREQUENCY}},					// 2 bytes

		{MANIFOLD_PRESSURE,			{SPN_MANIFOLD_PRESSURE_LEFT_BANK_1,		// 2 bytes
									 SPN_MANIFOLD_PRESSURE_RIGHT_BANK_2}},	// 2 bytes
		{O2_SENSOR_TYPE,			{SPN_O2_SENSOR_TYPE,								// 1 byte
									 SPN_O2_SENSOR_WARMUP_TIME_S,						// 2 bytes
									 SPN_O2_SENSOR_CLOSED_LOOP_OPERATION_LEFT_BANK_1,	// 1 byte
									 SPN_O2_SENSOR_CLOSED_LOOP_OPERATION_RIGHT_BANK_2}},// 1 byte
		{AFRC_CONFIGURATION_1,		{SPN_STARTUP_WAIT_TIME,				// 2 bytes
									 SPN_DPV_HOME_POSITION,				// 2 bytes
									 SPN_SINGLE_OR_DUAL_BANK,			// 1 byte
									 SPN_CALIBRATE_DPV_HOME_POSITION,	// 1 byte
									 SPN_HOME_POSITION_CALIBRATION_COUNTER}},// 2 bytes
		{WB_HEATER_PID_MONITOR_BANK_1,{SPN_WIDEBAND_NERNST_CELL_RESISTANCE_LEFT_BANK_1,
									 SPN_HEATER_DUTY_CYCLE_BANK_1,
									 SPN_HEATER_CONTROL_TIME_BANK_1,
									 SPN_HEATER_PID_STATE_BANK_1,
									 SPN_HEATER_PID_ENABLED_BANK_1}},
		{WB_HEATER_PID_MONITOR_BANK_2,{SPN_WIDEBAND_NERNST_CELL_RESISTANCE_RIGHT_BANK_2,
									 SPN_HEATER_DUTY_CYCLE_BANK_2,
									 SPN_HEATER_CONTROL_TIME_BANK_2,
									 SPN_HEATER_PID_STATE_BANK_2,
									 SPN_HEATER_PID_ENABLED_BANK_2}},
		{WB_HEATER_PID_MONITOR_POSTCAT,{SPN_WIDEBAND_NERNST_CELL_RESISTANCE_POSTCAT,
									 SPN_HEATER_DUTY_CYCLE_POSTCAT,
									 SPN_HEATER_CONTROL_TIME_POSTCAT,
									 SPN_HEATER_PID_STATE_POSTCAT,
									 SPN_HEATER_PID_ENABLED_POSTCAT}},
		{ACTIVE_DTC,				{SPN_DTC_STATUS_LEDS,
									 SPN_DTC_SPN}},
		{MULTISETPOINT_TABLE_CONFIGURATION,	{SPN_MULTISETPOINT_TABLE_CONFIG,
											 SPN_MULTISETPOINT_TABLE_LEFT_OR_RIGHT,
											 SPN_MULTISETPOINT_TABLE_SOURCE}},
		{MULTISETPOINT_TABLE_ROW_0,			{SPN_MULTISETPOINT_TABLE_ROW_0_ENABLED,
											 SPN_MULTISETPOINT_TABLE_ENTRY_0_INPUT,
											 SPN_MULTISETPOINT_TABLE_ENTRY_0_OFFSET}},
		{MULTISETPOINT_TABLE_ROW_1,			{SPN_MULTISETPOINT_TABLE_ROW_1_ENABLED,
											 SPN_MULTISETPOINT_TABLE_ENTRY_1_INPUT,
											 SPN_MULTISETPOINT_TABLE_ENTRY_1_OFFSET}},
		{MULTISETPOINT_TABLE_ROW_2,			{SPN_MULTISETPOINT_TABLE_ROW_2_ENABLED,
											 SPN_MULTISETPOINT_TABLE_ENTRY_2_INPUT,
											 SPN_MULTISETPOINT_TABLE_ENTRY_2_OFFSET}},
		{MULTISETPOINT_TABLE_ROW_3,			{SPN_MULTISETPOINT_TABLE_ROW_3_ENABLED,
											 SPN_MULTISETPOINT_TABLE_ENTRY_3_INPUT,
											 SPN_MULTISETPOINT_TABLE_ENTRY_3_OFFSET}},
		{MULTISETPOINT_TABLE_ROW_4,			{SPN_MULTISETPOINT_TABLE_ROW_4_ENABLED,
											 SPN_MULTISETPOINT_TABLE_ENTRY_4_INPUT,
											 SPN_MULTISETPOINT_TABLE_ENTRY_4_OFFSET}},
		{MULTISETPOINT_TABLE_SETUP,			{SPN_MULTISETPOINT_TABLE_SETUP,				// 6 bytes
											 SPN_SETPOINT_OFFSET}},						// 2 bytes
		{EXHAUST_TEMPERATURE_PRECAT_CALIBRATE,	{SPN_EXHAUST_TEMPERATURE_PRECAT_GAIN,	// 2 bytes
												 SPN_EXHAUST_TEMPERATURE_PRECAT_OFFSET,	// 2 bytes
												 SPN_TEMPERATURE_CONFIG_C_OR_F}},		// 1 byte
		{EXHAUST_TEMPERATURE_POSTCAT_CALIBRATE,	{SPN_EXHAUST_TEMPERATURE_POSTCAT_GAIN,	// 2 bytes
												 SPN_EXHAUST_TEMPERATURE_POSTCAT_OFFSET,// 2 bytes
												 SPN_MINIMUM_EXHAUST_TEMPERATURE,		// 2 bytes
												 SPN_MAXIMUM_EXHAUST_TEMPERATURE}},		// 2 bytes
		{REGULATORY_AVERAGES,				{SPN_REG_PRECAT_TEMPERATURE_4_HOUR_AVERAGE,			// 2 bytes
											 SPN_REG_DIFFERENTIAL_PRESSURE_15_MINUTE_AVERAGE}},	// 2 bytes
		{RAW_VOLTAGE_LEFT_BANK_1,			{SPN_RAW_ADC_NERNST_1,			// 2 bytes
											 SPN_RAW_ADC_WIDEBAND_1,		// 2 bytes
											 SPN_RAW_ADC_NARROWBAND_1}},	// 2 bytes
		{RAW_VOLTAGE_RIGHT_BANK_2,			{SPN_RAW_ADC_NERNST_2,			// 2 bytes
											 SPN_RAW_ADC_WIDEBAND_2,		// 2 bytes
											 SPN_RAW_ADC_NARROWBAND_2}},	// 2 bytes
		{RAW_VOLTAGE_POSTCAT,				{SPN_RAW_ADC_NERNST_POSTCAT,	// 2 bytes
											 SPN_RAW_ADC_WIDEBAND_POSTCAT,	// 2 bytes
											 SPN_RAW_ADC_NARROWBAND_POSTCAT}},// 2 bytes
		{DATATAG_STATUS,					{SPN_DATATAG_STATUS_ENGINE,
											 SPN_DATATAG_STATUS_COOLER,
											 SPN_DATATAG_STATUS_COMPRESSOR}},
		{PRODUCTION_SLAVE,					{SPN_PRODUCTION_TEST_STATE_SLAVE,		// 1 byte - DUT sends this
											 SPN_PRODUCTION_OK_TO_CALIBRATE,		// 1 byte
											 SPN_PRODUCTION_TARGET_TEMPERATURE,		// 2 bytes
											 SPN_PRODUCTION_CALIBRATED}},			// 1 byte
		{PRODUCTION_MASTER,					{SPN_PRODUCTION_TEST_STATE_MASTER}},	// 1 byte - Test Fixture sends this
		{PGN_AFTERTREATMENT_1_INTAKE_GAS_1,	{SPN_NOX,						// 2 bytes
											 SPN_O2,						// 2 bytes
											 SPN_NOX_SENSOR_STATUS,			// 1 byte
											 SPN_NOX_HEATER,				// 1 byte
											 SPN_NOX_ERROR,					// 1 byte
											 SPN_O2_ERROR}},				// 1 byte
		{PGN_AFTERTREATMENT_1_OUTLET_GAS_1,	{SPN_NOX,						// 2 bytes
											 SPN_O2,						// 2 bytes
											 SPN_NOX_SENSOR_STATUS,			// 1 byte
											 SPN_NOX_HEATER,				// 1 byte
											 SPN_NOX_ERROR,					// 1 byte
											 SPN_O2_ERROR}},				// 1 byte
		{ELECTRONIC_ENGINE_CONTROLLER_3,	{SPN_UNUSED_16,					// 2 bytes
											 SPN_UNUSED_16,					// 2 bytes
											 SPN_UNUSED_16,					// 2 bytes
											 SPN_UNUSED_8,					// 1 byte
											 SPN_DEW_POINT}},				// 1 byte
		{EMD_ANALOG_CONFIGURATION,			{SPN_ANALOG_1_IS_4_20MA,		// 1 byte
											 SPN_ANALOG_2_IS_4_20MA,		// 1 byte
											 SPN_ANALOG_3_IS_4_20MA,		// 1 byte
											 SPN_ANALOG_4_IS_4_20MA,		// 1 byte
											 SPN_ANALOG_5_IS_4_20MA}},		// 1 byte
		{AFRC_AUTOCONTROL,					{SPN_AUTOCONTROL_MINIMUM_POSTCAT_VOLTS,	// 2 bytes
											 SPN_AUTOCONTROL_TARGET_AQUISITION_TIME,// 2 bytes
											 SPN_AUTOCONTROL_TARGET_HOLD_TIME,		// 2 bytes
											 SPN_AUTOCONTROL_ADJUSTMENT}},			// 2 bytes
		{PGN_TACHOGRAPH,					{SPN_UNUSED_16,
											 SPN_UNUSED_16,
											 SPN_OUTPUT_SHAFT_SPEED,
											 SPN_TACHOGRAPH_VEHICLE_SPEED}},
		{PGN_VEHICLE_SPEED,					{SPN_COMPASS_BEARING,
											 SPN_NAVIGATION_BASED_VEHICLE_SPEED,
											 SPN_PITCH,
											 SPN_ALTITUDE}},
		{PGN_XY_STRING,						{SPN_X, SPN_Y, SPN_STRING}},
	};
	const int NUMBER_OF_PGNS_IN_MASTER_TABLE = sizeof(pgnMasterTable)/sizeof(pgnTableType);
#else
	extern const pgnTableType pgnMasterTable[];
	extern const int NUMBER_OF_PGNS_IN_MASTER_TABLE;
#endif	// GLOBAL

//-----------------------------------------------
// Transmit rates
//-----------------------------------------------
#define NEVER_SEND_UPDATE_RATE				0		// PGN is supported but not in the periodic transmit
#define NOX_UPDATE_RATE						50

//#define USE_SLOW_UPDATE_RATE
#ifdef USE_SLOW_UPDATE_RATE
	#define VERY_FAST_OPERATIONAL_UPDATE_RATE	500
	#define FAST_OPERATIONAL_UPDATE_RATE		500
	#define SLOW_OPERATIONAL_UPDATE_RATE		500
	#define STATUS_UPDATE_RATE					1000
	#define MST_UPDATE_RATE						500
#else
	//#define	USE_50PCT_BUS_LOADING
	#ifdef USE_50PCT_BUS_LOADING
		#define VERY_FAST_OPERATIONAL_UPDATE_RATE	10
		#define FAST_OPERATIONAL_UPDATE_RATE		10
		#define SLOW_OPERATIONAL_UPDATE_RATE		10
		#define STATUS_UPDATE_RATE					10
		#define MST_UPDATE_RATE						10
	#else
		#ifdef MAXIMUM_STRESS_TASK_SCHEDULING
			#define VERY_FAST_OPERATIONAL_UPDATE_RATE	10
			#define FAST_OPERATIONAL_UPDATE_RATE		25
			#define SLOW_OPERATIONAL_UPDATE_RATE		50
			#define STATUS_UPDATE_RATE					100
			#define MST_UPDATE_RATE						50
		#else
			#ifdef STRESS_TASK_SCHEDULING
				#define VERY_FAST_OPERATIONAL_UPDATE_RATE	25
				#define FAST_OPERATIONAL_UPDATE_RATE		50
				#define SLOW_OPERATIONAL_UPDATE_RATE		100
				#define STATUS_UPDATE_RATE					500
			#else
				#ifdef PRODUCTION_TEST
					#if (MY_MODULE_TYPE == PRODUCTION_TEST_TYPE)
						#define VERY_FAST_OPERATIONAL_UPDATE_RATE	250
						#define FAST_OPERATIONAL_UPDATE_RATE		250
						#define SLOW_OPERATIONAL_UPDATE_RATE		100
						#define STATUS_UPDATE_RATE					250
						#define MST_UPDATE_RATE						500
					#else
						#define VERY_FAST_OPERATIONAL_UPDATE_RATE	50
						#define FAST_OPERATIONAL_UPDATE_RATE		100
						#define SLOW_OPERATIONAL_UPDATE_RATE		100
						#define STATUS_UPDATE_RATE					250
						#define MST_UPDATE_RATE						500
					#endif
				#else
					//#define USE_350MS_EIM_UPDATE_RATES
					#ifdef USE_350MS_EIM_UPDATE_RATES
						// If the EIM is updating its display only every 350ms, then 1.) there's no use in this device
						// sending data faster than 250ms, and 2.) all data needs to be sent more frequenly than 350ms.
						#define VERY_FAST_OPERATIONAL_UPDATE_RATE	250
						#define FAST_OPERATIONAL_UPDATE_RATE		250
						#define SLOW_OPERATIONAL_UPDATE_RATE		250
						#define STATUS_UPDATE_RATE					250
						#define MST_UPDATE_RATE						250
					#else
						#define VERY_FAST_OPERATIONAL_UPDATE_RATE	50
						#define FAST_OPERATIONAL_UPDATE_RATE		100
						#define SLOW_OPERATIONAL_UPDATE_RATE		250
						#define STATUS_UPDATE_RATE					1000
					#endif
				#endif
			#endif
		#endif
	#endif
#endif

typedef struct {
	char *id;
	pgnEnumType pgn;
	U16	transmitRate;
} supportedPGNtype;

#ifdef __PIC32MX
	#ifndef J1939_DATA_LENGTH
		#define J1939_DATA_LENGTH	8
	#endif
	typedef U8 pgnDataType[J1939_DATA_LENGTH];
#endif

#define PGN_UPDATE_DATA(a,b)	{#a, a,	b}

#ifdef GLOBAL
	#ifndef MST_UPDATE_RATE
		#define MST_UPDATE_RATE	STATUS_UPDATE_RATE
	#endif
	const supportedPGNtype supportedPGNS_AFRC_LITE[] = {
			PGN_UPDATE_DATA(AFRC_LITE_MODULE_IDENTIFICATION,	STATUS_UPDATE_RATE),
			PGN_UPDATE_DATA(AFRC_CONTROL_STATUS,				SLOW_OPERATIONAL_UPDATE_RATE),
			PGN_UPDATE_DATA(AFRC_STATUS_1,						SLOW_OPERATIONAL_UPDATE_RATE),
			PGN_UPDATE_DATA(AFRC_STATUS_2,						SLOW_OPERATIONAL_UPDATE_RATE),
			PGN_UPDATE_DATA(AFRC_CONFIGURATION_1,				STATUS_UPDATE_RATE),
			PGN_UPDATE_DATA(ECU_RUN_TIME,						STATUS_UPDATE_RATE),
			PGN_UPDATE_DATA(O2_SENSOR_TYPE,						STATUS_UPDATE_RATE),
			PGN_UPDATE_DATA(WIDEBAND_O2_GAINS_BANK_1,			STATUS_UPDATE_RATE),
			PGN_UPDATE_DATA(NARROWBAND_O2_GAINS_BANK_1,			STATUS_UPDATE_RATE),
			PGN_UPDATE_DATA(WIDEBAND_NERNST_GAINS_BANK_1,		STATUS_UPDATE_RATE),
			PGN_UPDATE_DATA(WIDEBAND_O2_SENSOR_BANK_1,			FAST_OPERATIONAL_UPDATE_RATE),
			PGN_UPDATE_DATA(NARROWBAND_O2_SENSOR_BANK_1,		FAST_OPERATIONAL_UPDATE_RATE),
			PGN_UPDATE_DATA(AFRC_TEMPERATURE,					SLOW_OPERATIONAL_UPDATE_RATE),
			PGN_UPDATE_DATA(EXHAUST_TEMPERATURE_C,				SLOW_OPERATIONAL_UPDATE_RATE),
			PGN_UPDATE_DATA(MANIFOLD_PRESSURE,					SLOW_OPERATIONAL_UPDATE_RATE),
			PGN_UPDATE_DATA(DPV_POSITION,						FAST_OPERATIONAL_UPDATE_RATE),
			PGN_UPDATE_DATA(DPV_POSITION_STEPS,					FAST_OPERATIONAL_UPDATE_RATE),
			PGN_UPDATE_DATA(WB_HEATER_PID_MONITOR_BANK_1,		SLOW_OPERATIONAL_UPDATE_RATE),
			PGN_UPDATE_DATA(ANALOG_VOLTS_1,						SLOW_OPERATIONAL_UPDATE_RATE),
			PGN_UPDATE_DATA(ANALOG_CURRENT_1,					SLOW_OPERATIONAL_UPDATE_RATE),
			PGN_UPDATE_DATA(MULTISETPOINT_TABLE_CONFIGURATION,	MST_UPDATE_RATE),
			PGN_UPDATE_DATA(MULTISETPOINT_TABLE_ROW_0,			MST_UPDATE_RATE),
			PGN_UPDATE_DATA(MULTISETPOINT_TABLE_ROW_1,			MST_UPDATE_RATE),
			PGN_UPDATE_DATA(MULTISETPOINT_TABLE_ROW_2,			MST_UPDATE_RATE),
			PGN_UPDATE_DATA(MULTISETPOINT_TABLE_ROW_3,			MST_UPDATE_RATE),
			PGN_UPDATE_DATA(MULTISETPOINT_TABLE_ROW_4,			MST_UPDATE_RATE),
			PGN_UPDATE_DATA(EXHAUST_TEMPERATURE_PRECAT_CALIBRATE,	STATUS_UPDATE_RATE),
			PGN_UPDATE_DATA(EXHAUST_TEMPERATURE_POSTCAT_CALIBRATE,	STATUS_UPDATE_RATE),
			PGN_UPDATE_DATA(MULTISETPOINT_TABLE_SETUP,				NEVER_SEND_UPDATE_RATE),
			PGN_UPDATE_DATA(RAW_VOLTAGE_LEFT_BANK_1,			STATUS_UPDATE_RATE),
			#ifdef PRODUCTION_TEST
				PGN_UPDATE_DATA(PRODUCTION_SLAVE,				SLOW_OPERATIONAL_UPDATE_RATE),
			#endif
			#ifdef USE_SIMULATED_DATA
				// Try to mess up the EIM by putting something unexpected on the CAN bus
				PGN_UPDATE_DATA(PGN_TACHOGRAPH,					50),
				PGN_UPDATE_DATA(PGN_VEHICLE_SPEED,				50),
			#endif
	};
	const supportedPGNtype supportedPGNS_AFRC_ADV[] = {
			PGN_UPDATE_DATA(AFRC_ADVANCED_MODULE_IDENTIFICATION,STATUS_UPDATE_RATE),
			PGN_UPDATE_DATA(AFRC_CONTROL_STATUS,				SLOW_OPERATIONAL_UPDATE_RATE),
			PGN_UPDATE_DATA(AFRC_STATUS_1,						SLOW_OPERATIONAL_UPDATE_RATE),
			PGN_UPDATE_DATA(AFRC_STATUS_2,						SLOW_OPERATIONAL_UPDATE_RATE),
			PGN_UPDATE_DATA(AFRC_CONFIGURATION_1,				STATUS_UPDATE_RATE),
			PGN_UPDATE_DATA(ECU_RUN_TIME,						STATUS_UPDATE_RATE),
			PGN_UPDATE_DATA(O2_SENSOR_TYPE,						STATUS_UPDATE_RATE),
			PGN_UPDATE_DATA(WIDEBAND_O2_GAINS_BANK_1,			STATUS_UPDATE_RATE),
			PGN_UPDATE_DATA(NARROWBAND_O2_GAINS_BANK_1,			STATUS_UPDATE_RATE),
			PGN_UPDATE_DATA(WIDEBAND_NERNST_GAINS_BANK_1,		STATUS_UPDATE_RATE),
			PGN_UPDATE_DATA(WIDEBAND_O2_GAINS_BANK_2,			STATUS_UPDATE_RATE),
			PGN_UPDATE_DATA(NARROWBAND_O2_GAINS_BANK_2,			STATUS_UPDATE_RATE),
			PGN_UPDATE_DATA(WIDEBAND_NERNST_GAINS_BANK_2,		STATUS_UPDATE_RATE),
			PGN_UPDATE_DATA(WIDEBAND_O2_GAINS_POSTCAT,			STATUS_UPDATE_RATE),
			PGN_UPDATE_DATA(NARROWBAND_O2_GAINS_POSTCAT,		STATUS_UPDATE_RATE),
			PGN_UPDATE_DATA(WIDEBAND_NERNST_GAINS_POSTCAT,		STATUS_UPDATE_RATE),
			PGN_UPDATE_DATA(WIDEBAND_O2_SENSOR_BANK_1,			FAST_OPERATIONAL_UPDATE_RATE),
			PGN_UPDATE_DATA(WIDEBAND_O2_SENSOR_BANK_2,			FAST_OPERATIONAL_UPDATE_RATE),
			PGN_UPDATE_DATA(WIDEBAND_O2_SENSOR_POSTCAT,			FAST_OPERATIONAL_UPDATE_RATE),
			PGN_UPDATE_DATA(NARROWBAND_O2_SENSOR_BANK_1,		FAST_OPERATIONAL_UPDATE_RATE),
			PGN_UPDATE_DATA(NARROWBAND_O2_SENSOR_BANK_2,		FAST_OPERATIONAL_UPDATE_RATE),
			PGN_UPDATE_DATA(NARROWBAND_O2_SENSOR_POSTCAT,		FAST_OPERATIONAL_UPDATE_RATE),
			PGN_UPDATE_DATA(AFRC_TEMPERATURE,					SLOW_OPERATIONAL_UPDATE_RATE),
			PGN_UPDATE_DATA(EXHAUST_TEMPERATURE_C,				SLOW_OPERATIONAL_UPDATE_RATE),
			PGN_UPDATE_DATA(MANIFOLD_PRESSURE,					SLOW_OPERATIONAL_UPDATE_RATE),
			PGN_UPDATE_DATA(DPV_POSITION,						FAST_OPERATIONAL_UPDATE_RATE),
			PGN_UPDATE_DATA(DPV_POSITION_STEPS,					FAST_OPERATIONAL_UPDATE_RATE),
			PGN_UPDATE_DATA(WB_HEATER_PID_MONITOR_BANK_1,		SLOW_OPERATIONAL_UPDATE_RATE),
			PGN_UPDATE_DATA(WB_HEATER_PID_MONITOR_BANK_2,		SLOW_OPERATIONAL_UPDATE_RATE),
			PGN_UPDATE_DATA(WB_HEATER_PID_MONITOR_POSTCAT,		SLOW_OPERATIONAL_UPDATE_RATE),
			PGN_UPDATE_DATA(ANALOG_VOLTS_1,						SLOW_OPERATIONAL_UPDATE_RATE),
			PGN_UPDATE_DATA(ANALOG_CURRENT_1,					SLOW_OPERATIONAL_UPDATE_RATE),
			PGN_UPDATE_DATA(MULTISETPOINT_TABLE_CONFIGURATION,	MST_UPDATE_RATE),
			PGN_UPDATE_DATA(MULTISETPOINT_TABLE_ROW_0,			MST_UPDATE_RATE),
			PGN_UPDATE_DATA(MULTISETPOINT_TABLE_ROW_1,			MST_UPDATE_RATE),
			PGN_UPDATE_DATA(MULTISETPOINT_TABLE_ROW_2,			MST_UPDATE_RATE),
			PGN_UPDATE_DATA(MULTISETPOINT_TABLE_ROW_3,			MST_UPDATE_RATE),
			PGN_UPDATE_DATA(MULTISETPOINT_TABLE_ROW_4,			MST_UPDATE_RATE),
			PGN_UPDATE_DATA(EXHAUST_TEMPERATURE_PRECAT_CALIBRATE,	STATUS_UPDATE_RATE),
			PGN_UPDATE_DATA(EXHAUST_TEMPERATURE_POSTCAT_CALIBRATE,	STATUS_UPDATE_RATE),
			PGN_UPDATE_DATA(MULTISETPOINT_TABLE_SETUP,				NEVER_SEND_UPDATE_RATE),
			PGN_UPDATE_DATA(RAW_VOLTAGE_LEFT_BANK_1,			STATUS_UPDATE_RATE),
			PGN_UPDATE_DATA(RAW_VOLTAGE_RIGHT_BANK_2,			STATUS_UPDATE_RATE),
			PGN_UPDATE_DATA(RAW_VOLTAGE_POSTCAT,				STATUS_UPDATE_RATE),
			PGN_UPDATE_DATA(AFRC_AUTOCONTROL,					STATUS_UPDATE_RATE),
			#ifdef PRODUCTION_TEST
				PGN_UPDATE_DATA(PRODUCTION_SLAVE,				SLOW_OPERATIONAL_UPDATE_RATE),
			#endif
			PGN_UPDATE_DATA(PGN_XY_STRING,						NEVER_SEND_UPDATE_RATE),
	};
	const supportedPGNtype supportedPGNS_EMD[] = {
			PGN_UPDATE_DATA(EMD_MODULE_IDENTIFICATION,				STATUS_UPDATE_RATE),
			PGN_UPDATE_DATA(EMD_TEMPERATURE_1,						SLOW_OPERATIONAL_UPDATE_RATE),
			PGN_UPDATE_DATA(EMD_TEMPERATURE_2,						SLOW_OPERATIONAL_UPDATE_RATE),
			PGN_UPDATE_DATA(EMD_CONTROL_STATUS,						SLOW_OPERATIONAL_UPDATE_RATE),
			PGN_UPDATE_DATA(EMD_CONTROL_STATUS_2,					SLOW_OPERATIONAL_UPDATE_RATE),
			PGN_UPDATE_DATA(EMD_CONTROL_STATUS_3,					SLOW_OPERATIONAL_UPDATE_RATE),
			PGN_UPDATE_DATA(ANALOG_VOLTS_1,							SLOW_OPERATIONAL_UPDATE_RATE),
			PGN_UPDATE_DATA(ANALOG_VOLTS_2,							SLOW_OPERATIONAL_UPDATE_RATE),
			PGN_UPDATE_DATA(ANALOG_CURRENT_1,						SLOW_OPERATIONAL_UPDATE_RATE),
			PGN_UPDATE_DATA(ANALOG_CURRENT_2,						SLOW_OPERATIONAL_UPDATE_RATE),
			PGN_UPDATE_DATA(ECU_RUN_TIME,							STATUS_UPDATE_RATE),
			PGN_UPDATE_DATA(ENGINE_RUN_TIME,						STATUS_UPDATE_RATE),
			PGN_UPDATE_DATA(REGULATORY_AVERAGES,					STATUS_UPDATE_RATE),
			PGN_UPDATE_DATA(EXHAUST_TEMPERATURE_PRECAT_CALIBRATE,	STATUS_UPDATE_RATE),
			PGN_UPDATE_DATA(EXHAUST_TEMPERATURE_POSTCAT_CALIBRATE,	STATUS_UPDATE_RATE),
			PGN_UPDATE_DATA(DATATAG_STATUS,							STATUS_UPDATE_RATE),
			#ifdef ENABLE_NOX
				PGN_UPDATE_DATA(ELECTRONIC_ENGINE_CONTROLLER_3,		NOX_UPDATE_RATE),
				SPN_FREQUENCY
			#endif
			#ifdef PRODUCTION_TEST
				PGN_UPDATE_DATA(PRODUCTION_SLAVE,					SLOW_OPERATIONAL_UPDATE_RATE),
			#endif
	};
	const supportedPGNtype supportedPGNS_TEST_FIXTURE[] = {
			PGN_UPDATE_DATA(PRODUCTION_MASTER,						FAST_OPERATIONAL_UPDATE_RATE),
	};

	const int NUMBER_OF_PGNS_SUPPORTED_BY_AFRC_LITE = sizeof(supportedPGNS_AFRC_LITE)/sizeof(supportedPGNtype);
	const int NUMBER_OF_PGNS_SUPPORTED_BY_AFRC_ADV = sizeof(supportedPGNS_AFRC_ADV)/sizeof(supportedPGNtype);
	const int NUMBER_OF_PGNS_SUPPORTED_BY_EMD = sizeof(supportedPGNS_EMD)/sizeof(supportedPGNtype);
	const int NUMBER_OF_PGNS_SUPPORTED_BY_TEST_FIXTURE = sizeof(supportedPGNS_TEST_FIXTURE)/sizeof(supportedPGNtype);
#else
	extern const int NUMBER_OF_PGNS_SUPPORTED_BY_AFRC_LITE;
	extern const int NUMBER_OF_PGNS_SUPPORTED_BY_AFRC_ADV;
	extern const int NUMBER_OF_PGNS_SUPPORTED_BY_EMD;
	extern const int NUMBER_OF_PGNS_SUPPORTED_BY_TEST_FIXTURE;

	extern const supportedPGNtype supportedPGNS_AFRC_LITE[];
	extern const supportedPGNtype supportedPGNS_AFRC_ADV[];
	extern const supportedPGNtype supportedPGNS_EMD[];
	extern const supportedPGNtype supportedPGNS_TEST_FIXTURE[];
#endif	// GLOBAL

#if (MY_MODULE_TYPE == EMD_TYPE)
	#define NUMBER_OF_PGNS_SUPPORTED_BY_THIS_MODULE	NUMBER_OF_PGNS_SUPPORTED_BY_EMD
	#define supportedPGNS							supportedPGNS_EMD
#elif (MY_MODULE_TYPE == AFRC_LITE_TYPE)
	#define NUMBER_OF_PGNS_SUPPORTED_BY_THIS_MODULE NUMBER_OF_PGNS_SUPPORTED_BY_AFRC_LITE
	#define supportedPGNS							supportedPGNS_AFRC_LITE
#elif (MY_MODULE_TYPE == AFRC_ADVANCED_TYPE)
	#define NUMBER_OF_PGNS_SUPPORTED_BY_THIS_MODULE NUMBER_OF_PGNS_SUPPORTED_BY_AFRC_ADV
	#define supportedPGNS							supportedPGNS_AFRC_ADV
#elif (MY_MODULE_TYPE == EIM_PRODUCTION_TEST_TYPE) || (MY_MODULE_TYPE == PRODUCTION_TEST_TYPE)
	#define NUMBER_OF_PGNS_SUPPORTED_BY_THIS_MODULE NUMBER_OF_PGNS_SUPPORTED_BY_TEST_FIXTURE
	#define supportedPGNS							supportedPGNS_TEST_FIXTURE
#endif

#endif	// _PGN_H

// Last Line

