//-------------------------------------------------------------------------------------------------
// IO Table
// $Header: https://togwotee/ets/svn/ETS_Libraries/trunk/IO%20Table.c 1171 2014-07-09 16:39:15Z  $
//-------------------------------------------------------------------------------------------------
#include "environ.h"

// Placeholder variables
const U8 placeholderU8 = 0xff;
const U16 placeholderU16 = 0xffff;
const float placeholderFloat = 0.0;

// Compatibility macros
#define SPN_LAMBDA_LEFT_BANK_1	SPN_NARROWBAND_VOLTAGE_LEFT_BANK_1
#define SPN_LAMBDA_RIGHT_BANK_2	SPN_NARROWBAND_VOLTAGE_RIGHT_BANK_2
#define SPN_LAMBDA_POSTCAT		SPN_NARROWBAND_VOLTAGE_POSTCAT

#if (MY_MODULE_TYPE == EMD_TYPE)
	const ioTableType ioTable[] = {
		{SPN_BATTERY_POTENTIAL,							MB_INVALID,								TYPE_FLOAT,	(dataUnionType *)&adcData[BATTERY_VOLTAGE].volts},

		{SPN_EXHAUST_TEMPERATURE_PRECAT,				MB_EXHAUST_TEMPERATURE_PRECAT,			TYPE_FLOAT,	(dataUnionType *)&operationalData.temperatureC[TC_PRECAT]},
		{SPN_EXHAUST_TEMPERATURE_POSTCAT,				MB_EXHAUST_TEMPERATURE_POSTCAT,			TYPE_FLOAT,	(dataUnionType *)&operationalData.temperatureC[TC_POSTCAT]},
		{SPN_EXHAUST_DIFFERENTIAL_TEMPERATURE,			MB_EXHAUST_DIFFERENTIAL_TEMPERATURE,	TYPE_FLOAT,	(dataUnionType *)&operationalData.differentialTemperatureC},
		{SPN_AUXILARY_TEMPERATURE_1,					MB_AUXILARY_TEMPERATURE_1,				TYPE_FLOAT,	(dataUnionType *)&operationalData.temperatureC[TC_SPARE_0]},
		{SPN_AUXILARY_TEMPERATURE_2,					MB_AUXILARY_TEMPERATURE_2,				TYPE_FLOAT,	(dataUnionType *)&operationalData.temperatureC[TC_SPARE_1]},
		{SPN_AMBIENT_TEMPERATURE,						MB_AMBIENT_TEMPERATURE,					TYPE_FLOAT,	(dataUnionType *)&operationalData.temperatureC[TC_AMBIENT]},
		{SPN_FUEL_TEMPERATURE,							MB_FUEL_TEMPERATURE,					TYPE_FLOAT,	(dataUnionType *)&operationalData.temperatureC[TC_FUEL]},
		{SPN_AMBIENT_TEMPERATURE_THERMISTOR,			MB_AMBIENT_TEMPERATURE_THERMISTOR,		TYPE_FLOAT,	(dataUnionType *)&operationalData.temperatureC[TM_AMBIENT]},
		{SPN_FUEL_TEMPERATURE_THERMISTOR,				MB_FUEL_TEMPERATURE_THERMISTOR,			TYPE_FLOAT,	(dataUnionType *)&operationalData.temperatureC[TM_FUEL]},
	#ifdef DISABLE_DATATAG
		{SPN_ENGINE_UPTIME,								MB_ENGINE_UPTIME,	TYPE_FLOAT, (dataUnionType *)&operationalData.engine.runtime},
		{SPN_ENGINE_RUNTIME,							MB_ENGINE_RUNTIME,	TYPE_FLOAT, (dataUnionType *)&runtimeData.engine.totalRuntime},
	#else
		{SPN_ENGINE_UPTIME,								MB_ENGINE_UPTIME,	TYPE_FLOAT, (dataUnionType *)&operationalData.engine.runtime},
		{SPN_ENGINE_RUNTIME,							MB_ENGINE_RUNTIME,	TYPE_FLOAT, (dataUnionType *)&runtimeData.engine.totalRuntime},
//		{SPN_ENGINE_UPTIME,								MB_ENGINE_UPTIME,	TYPE_FLOAT, (dataUnionType *)&engineDatatag.runTime},
//		{SPN_ENGINE_RUNTIME,							MB_ENGINE_RUNTIME,	TYPE_FLOAT, (dataUnionType *)&engineDatatag.totalRuntime},
	#endif
		{SPN_ANALOG_VOLTAGE_1,							MB_INVALID,					TYPE_FLOAT,	(dataUnionType *)&adcData[ANALOG_1].volts},
		{SPN_ANALOG_CURRENT_1,							MB_INVALID,					TYPE_FLOAT,	(dataUnionType *)&adcData[ANALOG_1].current},
		{SPN_ANALOG_VOLTAGE_2,							MB_INVALID,					TYPE_FLOAT,	(dataUnionType *)&adcData[ANALOG_2].volts},
		{SPN_ANALOG_CURRENT_2,							MB_INVALID,					TYPE_FLOAT,	(dataUnionType *)&adcData[ANALOG_2].current},
		{SPN_ANALOG_VOLTAGE_3,							MB_INVALID,					TYPE_FLOAT,	(dataUnionType *)&adcData[ANALOG_3].volts},
		{SPN_ANALOG_CURRENT_3,							MB_INVALID,					TYPE_FLOAT,	(dataUnionType *)&adcData[ANALOG_3].current},
		{SPN_ANALOG_VOLTAGE_4,							MB_INVALID,					TYPE_FLOAT,	(dataUnionType *)&adcData[ANALOG_4].volts},
		{SPN_ANALOG_CURRENT_4,							MB_INVALID,					TYPE_FLOAT,	(dataUnionType *)&adcData[ANALOG_4].current},
		{SPN_ANALOG_VOLTAGE_5,							MB_INVALID,					TYPE_FLOAT,	(dataUnionType *)&adcData[ANALOG_5].volts},
		{SPN_ANALOG_CURRENT_5,							MB_INVALID,					TYPE_FLOAT,	(dataUnionType *)&adcData[ANALOG_5].current},

		{SPN_CAT_PRESSURE_VOLTAGE,						MB_INVALID,					TYPE_FLOAT,	(dataUnionType *)&adcData[CAT_PRESSURE].volts},
		{SPN_CAT_PRESSURE_CURRENT,						MB_CAT_PRESSURE_CURRENT,	TYPE_FLOAT,	(dataUnionType *)&adcData[CAT_PRESSURE].current},
		{SPN_FUELFLOW_VOLTAGE,							MB_INVALID,					TYPE_FLOAT,	(dataUnionType *)&adcData[FUELFLOW].volts},
		{SPN_FUELFLOW_CURRENT,							MB_FUELFLOW_CURRENT,		TYPE_FLOAT,	(dataUnionType *)&adcData[FUELFLOW].current},
		{SPN_ANALOG_1_IS_4_20MA,						MB_INVALID,					TYPE_U8,	(dataUnionType *)&configurationData.adc[ANALOG_1].inputIsCurrent},
		{SPN_ANALOG_2_IS_4_20MA,						MB_INVALID,					TYPE_U8,	(dataUnionType *)&configurationData.adc[ANALOG_2].inputIsCurrent},
		{SPN_ANALOG_3_IS_4_20MA,						MB_INVALID,					TYPE_U8,	(dataUnionType *)&configurationData.adc[ANALOG_3].inputIsCurrent},
		{SPN_ANALOG_4_IS_4_20MA,						MB_INVALID,					TYPE_U8,	(dataUnionType *)&configurationData.adc[ANALOG_4].inputIsCurrent},
		{SPN_ANALOG_5_IS_4_20MA,						MB_INVALID,					TYPE_U8,	(dataUnionType *)&configurationData.adc[ANALOG_5].inputIsCurrent},

		{SPN_ERROR_RELAY,								MB_INVALID,					TYPE_U8,	(dataUnionType *)&operationalData.errorRelayIsClosed},
		{SPN_DISCRETE_INPUT,							MB_INVALID,					TYPE_U8,	(dataUnionType *)&operationalData.DI1switchIsClosed},
		{SPN_OIL_PRESSURE_SWITCH,						MB_INVALID,					TYPE_U8,	(dataUnionType *)&operationalData.oilPressureSwitchIsClosed},
		{SPN_REG_PRECAT_TEMPERATURE_4_HOUR_AVERAGE,		MB_REG_PRECAT_TEMPERATURE_4_HOUR_AVERAGE,	TYPE_FLOAT,	(dataUnionType *)&regulatoryDatalog[RLOG_TC_PRECAT_4HR].filteredData},
		{SPN_REG_DIFFERENTIAL_PRESSURE_15_MINUTE_AVERAGE,MB_REG_DIFFERENTIAL_PRESSURE_15_MINUTE_AVERAGE,TYPE_FLOAT,(dataUnionType *)&regulatoryDatalog[RLOG_DIFF_PRESSURE_15MIN].filteredData},
		{SPN_ENGINE_SPEED,								MB_INVALID,				TYPE_FLOAT,	(dataUnionType *)&operationalData.engine.rpm},

		// Module information
		{SPN_ECU_UPTIME,								MB_ECU_UPTIME,			TYPE_FLOAT, (dataUnionType *)&operationalData.control.runtime},
		{SPN_ECU_RUNTIME,								MB_ECU_RUNTIME,			TYPE_FLOAT, (dataUnionType *)&runtimeData.control.totalRuntime},
		{SPN_EMIT_SERIAL_NUMBER,						MB_EMIT_SERIAL_NUMBER,	TYPE_U32,	(dataUnionType *)&operationalData.control.serialNumber},
		{SPN_HARDWARE_ID,								MB_HARDWARE_ID,			TYPE_U16,	(dataUnionType *)&configurationData.control.hardwareID},
		{SPN_APPLICATION_ID,							MB_APPLICATION_ID,		TYPE_U16,	(dataUnionType *)&configurationData.control.applicationID},
		{SPN_TEMPERATURE_CONFIG_C_OR_F,					MB_INVALID,				TYPE_U8,	(dataUnionType *)&configurationData.temperatureIsFarenheitIfTrue},

		// Thermcouple gain and offset
		{SPN_EXHAUST_TEMPERATURE_PRECAT_GAIN,			MB_INVALID,				TYPE_FLOAT,	(dataUnionType *)&configurationData.exhaustTemperature.precatGain},
		{SPN_EXHAUST_TEMPERATURE_PRECAT_OFFSET,			MB_INVALID,				TYPE_FLOAT,	(dataUnionType *)&configurationData.exhaustTemperature.precatOffset},
		{SPN_EXHAUST_TEMPERATURE_POSTCAT_GAIN,			MB_INVALID,				TYPE_FLOAT,	(dataUnionType *)&configurationData.exhaustTemperature.postcatGain},
		{SPN_EXHAUST_TEMPERATURE_POSTCAT_OFFSET,		MB_INVALID,				TYPE_FLOAT,	(dataUnionType *)&configurationData.exhaustTemperature.postcatOffset},

		{SPN_MINIMUM_EXHAUST_TEMPERATURE,				MB_INVALID,				TYPE_FLOAT,	(dataUnionType *)&configurationData.exhaustTemperature.minimum},
		{SPN_MAXIMUM_EXHAUST_TEMPERATURE,				MB_INVALID,				TYPE_FLOAT,	(dataUnionType *)&configurationData.exhaustTemperature.maximum},

	#ifndef DISABLE_DATATAG
		{SPN_DATATAG_STATUS_ENGINE,						MB_INVALID,				TYPE_U8,	(dataUnionType *)&datatagData[ENGINE_TAG].state},
		{SPN_DATATAG_STATUS_COOLER,						MB_INVALID,				TYPE_U8,	(dataUnionType *)&datatagData[COOLER_TAG].state},
		{SPN_DATATAG_STATUS_COMPRESSOR,					MB_INVALID,				TYPE_U8,	(dataUnionType *)&datatagData[COMPRESSOR_TAG].state},
	#endif

		{SPN_RUN_SIGNAL,								MB_INVALID,				TYPE_U8,	(dataUnionType *)&operationalData.RunSignal_enableSensors},

		// Added 2012/09/20
		{SPN_NOX,										MB_NOX,					TYPE_FLOAT,		(dataUnionType *)&operationalData.noxSensor.nox_ppm},
		{SPN_O2,										MB_O2,					TYPE_FLOAT,		(dataUnionType *)&operationalData.noxSensor.o2_percent},
		{SPN_NOX_SENSOR_STATUS,							MB_NOX_SENSOR_STATUS,	TYPE_U8,		(dataUnionType *)&operationalData.noxSensor.sensorStatus.all},
		{SPN_NOX_HEATER,								MB_NOX_HEATER,			TYPE_U8,		(dataUnionType *)&operationalData.noxSensor.heaterStatus.all},
		{SPN_NOX_ERROR,									MB_NOX_ERROR,			TYPE_U8,		(dataUnionType *)&operationalData.noxSensor.noxStatus.all},
		{SPN_O2_ERROR,									MB_O2_ERROR,			TYPE_U8,		(dataUnionType *)&operationalData.noxSensor.o2Status.all},
		{SPN_DEW_POINT,									MB_DEW_POINT,			TYPE_U8,		(dataUnionType *)&operationalData.noxSensor.dewPoint},

		// Added 2014-07-09
		{SPN_FREQUENCY,									MB_FREQUENCY,			TYPE_FLOAT,		(dataUnionType *)&operationalData.engine.frequency},

	#ifdef PRODUCTION_TEST
		// Added 2012/06/26
		{SPN_PRODUCTION_TEST_STATE_SLAVE,				MB_INVALID,				TYPE_U8,	(dataUnionType *)&operationalData.test.state},
		{SPN_PRODUCTION_TEST_STATE_MASTER,				MB_INVALID,				TYPE_U8,	(dataUnionType *)&operationalData.test.masterState},
		{SPN_PRODUCTION_TARGET_TEMPERATURE,				MB_INVALID,				TYPE_FLOAT,	(dataUnionType *)&operationalData.test.targetTemperature},
		{SPN_PRODUCTION_OK_TO_CALIBRATE,				MB_INVALID,				TYPE_U8,	(dataUnionType *)&operationalData.test.okToCalibrate},
		{SPN_PRODUCTION_CALIBRATED,						MB_INVALID,				TYPE_U8,	(dataUnionType *)&factoryCalibration.calibrated},
	#endif
	};

#endif
#if (MY_MODULE_TYPE == AFRC_LITE_TYPE) || (MY_MODULE_TYPE == AFRC_ADVANCED_TYPE)
	const ioTableType ioTable[] = {
		{SPN_BATTERY_POTENTIAL,							MB_INVALID,	TYPE_FLOAT,	(dataUnionType *)&adcData[BATTERY_VOLTAGE].volts},

		// Left Bank
		{SPN_EXHAUST_TEMPERATURE_PRECAT,				MB_INVALID,	TYPE_FLOAT,	(dataUnionType *)&operationalData.temperatureC[TC_PRECAT]},
		{SPN_EXHAUST_TEMPERATURE_POSTCAT,				MB_INVALID,	TYPE_FLOAT,	(dataUnionType *)&operationalData.temperatureC[TC_POSTCAT]},
		{SPN_EXHAUST_DIFFERENTIAL_TEMPERATURE,			MB_INVALID,	TYPE_FLOAT,	(dataUnionType *)&operationalData.differentialTemperatureC},
		{SPN_WIDEBAND_LAMBDA_LEFT_BANK_1,				MB_INVALID,	TYPE_FLOAT, (dataUnionType *)&sensorData[LOCATION_BANK_1].lambda.value},
		{SPN_DPV_DESIRED_POSITION_LEFT_BANK_1,			MB_INVALID,	TYPE_FLOAT,	(dataUnionType *)&stepperMotorData[LOCATION_BANK_1].desiredPosition},
		{SPN_DPV_ACTUAL_POSITION_LEFT_BANK_1,			MB_INVALID,	TYPE_FLOAT,	(dataUnionType *)&stepperMotorData[LOCATION_BANK_1].actualPosition},
		{SPN_DPV_DESIRED_POSITION_LEFT_BANK_1_STEPS,	MB_INVALID,	TYPE_U16,	(dataUnionType *)&stepperMotorData[LOCATION_BANK_1].desiredPosition_steps},
		{SPN_DPV_ACTUAL_POSITION_LEFT_BANK_1_STEPS,		MB_INVALID,	TYPE_U16,	(dataUnionType *)&stepperMotorData[LOCATION_BANK_1].actualPosition_steps},
		{SPN_WIDEBAND_NERNST_CELL_RESISTANCE_LEFT_BANK_1,MB_INVALID,	TYPE_FLOAT,	(dataUnionType *)&sensorData[LOCATION_BANK_1].heater.filteredResistance},
		{SPN_O2_SENSOR_CLOSED_LOOP_OPERATION_LEFT_BANK_1,	MB_INVALID,	TYPE_U8,	(dataUnionType *)&stepperMotorData[LOCATION_BANK_1].DPVisInAutoMode},
		{SPN_NARROWBAND_VOLTAGE_LEFT_BANK_1,			MB_INVALID,	TYPE_FLOAT,	(dataUnionType *)&sensorData[LOCATION_BANK_1].lambda.longFilteredValue},
		{SPN_SENSOR_STATUS_LEFT_BANK_1_1,				MB_INVALID,	TYPE_U8,	(dataUnionType *)&sensorData[LOCATION_BANK_1].sensorStatus1.all},
		{SPN_SENSOR_STATUS_LEFT_BANK_1_2,				MB_INVALID,	TYPE_U8,	(dataUnionType *)&sensorData[LOCATION_BANK_1].sensorStatus2.all},
		{SPN_PID_RESPONSE_TIME_LEFT_BANK_1,				MB_INVALID,	TYPE_FLOAT,	(dataUnionType *)&configurationData.sensor[LOCATION_BANK_1].PID_responseTime},

	#if (MY_MODULE_TYPE == AFRC_ADVANCED_TYPE)
		// 2012-01-31
		{SPN_ENABLE_AUTOCONTROL_MODE,					MB_INVALID,	TYPE_U8,	(dataUnionType *)&configurationData.pcmControl.enable},
		{SPN_AUTOCONTROL_ADJUSTMENT,					MB_INVALID,	TYPE_FLOAT,	(dataUnionType *)&configurationData.pcmControl.afrControlAdjustment},
		{SPN_AUTOCONTROL_MINIMUM_POSTCAT_VOLTS,			MB_INVALID,	TYPE_FLOAT,	(dataUnionType *)&configurationData.pcmControl.postcat_minimum_volts},
		{SPN_AUTOCONTROL_TARGET_AQUISITION_TIME,		MB_INVALID,	TYPE_FLOAT,	(dataUnionType *)&configurationData.pcmControl.target_aquisition_time_seconds},
		{SPN_AUTOCONTROL_TARGET_HOLD_TIME,				MB_INVALID,	TYPE_FLOAT,	(dataUnionType *)&configurationData.pcmControl.target_hold_time_seconds},

		// Right Bank
		{SPN_WIDEBAND_LAMBDA_RIGHT_BANK_2,				MB_INVALID,	TYPE_FLOAT, (dataUnionType *)&sensorData[LOCATION_BANK_2].lambda.value},
		{SPN_DPV_DESIRED_POSITION_RIGHT_BANK_2,			MB_INVALID,	TYPE_FLOAT,	(dataUnionType *)&stepperMotorData[LOCATION_BANK_2].desiredPosition},
		{SPN_DPV_ACTUAL_POSITION_RIGHT_BANK_2,			MB_INVALID,	TYPE_FLOAT,	(dataUnionType *)&stepperMotorData[LOCATION_BANK_2].actualPosition},
		{SPN_DPV_DESIRED_POSITION_RIGHT_BANK_2_STEPS,	MB_INVALID,	TYPE_U16,	(dataUnionType *)&stepperMotorData[LOCATION_BANK_2].desiredPosition_steps},
		{SPN_DPV_ACTUAL_POSITION_RIGHT_BANK_2_STEPS,	MB_INVALID,	TYPE_U16,	(dataUnionType *)&stepperMotorData[LOCATION_BANK_2].actualPosition_steps},
		{SPN_NARROWBAND_VOLTAGE_RIGHT_BANK_2,			MB_INVALID,	TYPE_FLOAT,	(dataUnionType *)&sensorData[LOCATION_BANK_2].lambda.longFilteredValue},
		{SPN_O2_SENSOR_CLOSED_LOOP_OPERATION_RIGHT_BANK_2,	MB_INVALID,	TYPE_U8,	(dataUnionType *)&stepperMotorData[LOCATION_BANK_2].DPVisInAutoMode},
		{SPN_WIDEBAND_NERNST_CELL_RESISTANCE_RIGHT_BANK_2,	MB_INVALID,	TYPE_FLOAT,	(dataUnionType *)&sensorData[LOCATION_BANK_2].heater.filteredResistance},
		{SPN_SENSOR_STATUS_RIGHT_BANK_2_1,				MB_INVALID,	TYPE_U8,	(dataUnionType *)&sensorData[LOCATION_BANK_2].sensorStatus1.all},
		{SPN_SENSOR_STATUS_RIGHT_BANK_2_2,				MB_INVALID,	TYPE_U8,	(dataUnionType *)&sensorData[LOCATION_BANK_2].sensorStatus2.all},
		{SPN_PID_RESPONSE_TIME_RIGHT_BANK_2,			MB_INVALID,	TYPE_FLOAT,	(dataUnionType *)&configurationData.sensor[LOCATION_BANK_2].PID_responseTime},

		// Postcat
		{SPN_WIDEBAND_LAMBDA_POSTCAT,					MB_INVALID,	TYPE_FLOAT, (dataUnionType *)&sensorData[LOCATION_POST_CATALYST].lambda.value},
		{SPN_NARROWBAND_VOLTAGE_POSTCAT,				MB_INVALID,	TYPE_FLOAT,	(dataUnionType *)&sensorData[LOCATION_POST_CATALYST].lambda.longFilteredValue},
		{SPN_O2_SENSOR_CLOSED_LOOP_OPERATION_POSTCAT,	MB_INVALID,	TYPE_U8,	(dataUnionType *)&stepperMotorData[LOCATION_POST_CATALYST].DPVisInAutoMode},
		{SPN_WIDEBAND_NERNST_CELL_RESISTANCE_POSTCAT,	MB_INVALID,	TYPE_FLOAT,	(dataUnionType *)&sensorData[LOCATION_POST_CATALYST].heater.filteredResistance},
		{SPN_SENSOR_STATUS_POSTCAT_1,					MB_INVALID,	TYPE_U8,	(dataUnionType *)&sensorData[LOCATION_POST_CATALYST].sensorStatus1.all},
		{SPN_SENSOR_STATUS_POSTCAT_2,					MB_INVALID,	TYPE_U8,	(dataUnionType *)&sensorData[LOCATION_POST_CATALYST].sensorStatus2.all},

		{SPN_AFRC_CONTROL_STATUS_BANK_2,				MB_INVALID,	TYPE_U8,	(dataUnionType *)&sensorData[LOCATION_BANK_2].status},
		{SPN_AFRC_CONTROL_STATUS_POSTCAT,				MB_INVALID,	TYPE_U8,	(dataUnionType *)&sensorData[LOCATION_POST_CATALYST].status},
		{SPN_DISCRETE_INPUT,							MB_INVALID,	TYPE_U8,	(dataUnionType *)&operationalData.DI1switchIsClosed},
		{SPN_OIL_PRESSURE_SWITCH,						MB_INVALID,	TYPE_U8,	(dataUnionType *)&operationalData.oilPressureSwitchIsClosed},
	#endif

		{SPN_SINGLE_OR_DUAL_BANK,						MB_INVALID,	TYPE_U8,	(dataUnionType *)&configurationData.dualBankIfTrue},	
		{SPN_USE_POST_CATALYST_SENSOR,					MB_INVALID,	TYPE_U8,	(dataUnionType *)&configurationData.usePostCatSensorIfTrue},
		{SPN_ERROR_RELAY,								MB_INVALID,	TYPE_U8,	(dataUnionType *)&operationalData.errorRelayIsClosed},

		// O2 Sensors
		{SPN_O2_SENSOR_TYPE,							MB_INVALID,	TYPE_U8,	(dataUnionType *)&configurationData.O2sensorType},
		{SPN_AUTO_MANUAL_MODE,							MB_INVALID,	TYPE_U8,	(dataUnionType *)&configurationData.enableAutomaticValveControlMode},
		{SPN_RUN_SIGNAL,								MB_INVALID,	TYPE_U8,	(dataUnionType *)&operationalData.RunSignal_enableSensors},
		{SPN_AFRC_CONTROL_STATUS,						MB_INVALID,	TYPE_U8,	(dataUnionType *)&sensorControlData.status},
		{SPN_AFRC_CONTROL_STATUS_BANK_1,				MB_INVALID,	TYPE_U8,	(dataUnionType *)&sensorData[LOCATION_BANK_1].status},
	#if (MY_MODULE_TYPE == AFRC_ADVANCED_TYPE)
		{SPN_AFRC_CONTROL_STATUS_BANK_2,				MB_INVALID,	TYPE_U8,	(dataUnionType *)&sensorData[LOCATION_BANK_2].status},
		{SPN_AFRC_CONTROL_STATUS_POSTCAT,				MB_INVALID,	TYPE_U8,	(dataUnionType *)&sensorData[LOCATION_POST_CATALYST].status},
	#endif

		// Left
		{SPN_LEFT_BANK_ENABLED,							MB_INVALID,	TYPE_U8,	(dataUnionType *)&sensorData[LOCATION_BANK_1].enabled},
		{SPN_LEFT_WIDEBAND_KP,							MB_INVALID,	TYPE_FLOAT, (dataUnionType *)&configurationData.sensor[LOCATION_BANK_1].wideband.Kp},
		{SPN_LEFT_WIDEBAND_KI,							MB_INVALID,	TYPE_FLOAT, (dataUnionType *)&configurationData.sensor[LOCATION_BANK_1].wideband.Ki},
		{SPN_LEFT_WIDEBAND_KD,							MB_INVALID,	TYPE_FLOAT, (dataUnionType *)&configurationData.sensor[LOCATION_BANK_1].wideband.Kd},
		{SPN_LEFT_NARROWBAND_KP,						MB_INVALID,	TYPE_FLOAT, (dataUnionType *)&configurationData.sensor[LOCATION_BANK_1].narrowband.Kp},
		{SPN_LEFT_NARROWBAND_KI,						MB_INVALID,	TYPE_FLOAT, (dataUnionType *)&configurationData.sensor[LOCATION_BANK_1].narrowband.Ki},
		{SPN_LEFT_NARROWBAND_KD,						MB_INVALID,	TYPE_FLOAT, (dataUnionType *)&configurationData.sensor[LOCATION_BANK_1].narrowband.Kd},
		{SPN_LEFT_NERNST_KP,							MB_INVALID,	TYPE_FLOAT,	(dataUnionType *)&configurationData.sensor[LOCATION_BANK_1].nernst.Kp},
		{SPN_LEFT_NERNST_KI,							MB_INVALID,	TYPE_FLOAT,	(dataUnionType *)&configurationData.sensor[LOCATION_BANK_1].nernst.Ki},
		{SPN_LEFT_NERNST_KD,							MB_INVALID,	TYPE_FLOAT,	(dataUnionType *)&configurationData.sensor[LOCATION_BANK_1].nernst.Kd},
		{SPN_LEFT_WIDEBAND_LAMBDA_SETPOINT,				MB_INVALID,	TYPE_FLOAT,	(dataUnionType *)&configurationData.sensor[LOCATION_BANK_1].wideband.setpoint},
		{SPN_LEFT_NARROWBAND_VOLTAGE_SETPOINT,			MB_INVALID,	TYPE_FLOAT,	(dataUnionType *)&configurationData.sensor[LOCATION_BANK_1].narrowband.setpoint},
		{SPN_RAW_ADC_NERNST_1,							MB_INVALID,	TYPE_FLOAT,	(dataUnionType *)&operationalData.adc[ADC_NERNST_1].adcVolts_unfiltered},
		{SPN_RAW_ADC_WIDEBAND_1,						MB_INVALID,	TYPE_FLOAT,	(dataUnionType *)&operationalData.adc[ADC_WIDEBAND_1].adcVolts_unfiltered},
		{SPN_RAW_ADC_NARROWBAND_1,						MB_INVALID,	TYPE_FLOAT,	(dataUnionType *)&operationalData.adc[ADC_NARROWBAND_1].adcVolts_unfiltered},

	#if (MY_MODULE_TYPE == AFRC_ADVANCED_TYPE)
		// Right
		{SPN_RIGHT_BANK_ENABLED,						MB_INVALID,	TYPE_U8,	(dataUnionType *)&sensorData[LOCATION_BANK_2].enabled},
		{SPN_RIGHT_WIDEBAND_KP,							MB_INVALID,	TYPE_FLOAT, (dataUnionType *)&configurationData.sensor[LOCATION_BANK_2].wideband.Kp},
		{SPN_RIGHT_WIDEBAND_KI,							MB_INVALID,	TYPE_FLOAT, (dataUnionType *)&configurationData.sensor[LOCATION_BANK_2].wideband.Ki},
		{SPN_RIGHT_WIDEBAND_KD,							MB_INVALID,	TYPE_FLOAT, (dataUnionType *)&configurationData.sensor[LOCATION_BANK_2].wideband.Kd},
		{SPN_RIGHT_NARROWBAND_KP,						MB_INVALID,	TYPE_FLOAT, (dataUnionType *)&configurationData.sensor[LOCATION_BANK_2].narrowband.Kp},
		{SPN_RIGHT_NARROWBAND_KI,						MB_INVALID,	TYPE_FLOAT, (dataUnionType *)&configurationData.sensor[LOCATION_BANK_2].narrowband.Ki},
		{SPN_RIGHT_NARROWBAND_KD,						MB_INVALID,	TYPE_FLOAT, (dataUnionType *)&configurationData.sensor[LOCATION_BANK_2].narrowband.Kd},
		{SPN_RIGHT_NERNST_KP,							MB_INVALID,	TYPE_FLOAT,	(dataUnionType *)&configurationData.sensor[LOCATION_BANK_2].nernst.Kp},
		{SPN_RIGHT_NERNST_KI,							MB_INVALID,	TYPE_FLOAT,	(dataUnionType *)&configurationData.sensor[LOCATION_BANK_2].nernst.Ki},
		{SPN_RIGHT_NERNST_KD,							MB_INVALID,	TYPE_FLOAT,	(dataUnionType *)&configurationData.sensor[LOCATION_BANK_2].nernst.Kd},
		{SPN_RIGHT_WIDEBAND_LAMBDA_SETPOINT,			MB_INVALID,	TYPE_FLOAT,	(dataUnionType *)&configurationData.sensor[LOCATION_BANK_2].wideband.setpoint},
		{SPN_RIGHT_NARROWBAND_VOLTAGE_SETPOINT,			MB_INVALID,	TYPE_FLOAT,	(dataUnionType *)&configurationData.sensor[LOCATION_BANK_2].narrowband.setpoint},
		{SPN_RAW_ADC_NERNST_2,							MB_INVALID,	TYPE_FLOAT,	(dataUnionType *)&operationalData.adc[ADC_NERNST_2].adcVolts_unfiltered},
		{SPN_RAW_ADC_WIDEBAND_2,						MB_INVALID,	TYPE_FLOAT,	(dataUnionType *)&operationalData.adc[ADC_WIDEBAND_2].adcVolts_unfiltered},
		{SPN_RAW_ADC_NARROWBAND_2,						MB_INVALID,	TYPE_FLOAT,	(dataUnionType *)&operationalData.adc[ADC_NARROWBAND_2].adcVolts_unfiltered},

		// Post Catalyst
		{SPN_POSTCAT_ENABLED,							MB_INVALID,	TYPE_U8,	(dataUnionType *)&sensorData[LOCATION_POST_CATALYST].enabled},
		{SPN_POSTCAT_WIDEBAND_KP,						MB_INVALID,	TYPE_FLOAT, (dataUnionType *)&configurationData.sensor[LOCATION_POST_CATALYST].wideband.Kp},
		{SPN_POSTCAT_WIDEBAND_KI,						MB_INVALID,	TYPE_FLOAT, (dataUnionType *)&configurationData.sensor[LOCATION_POST_CATALYST].wideband.Ki},
		{SPN_POSTCAT_WIDEBAND_KD,						MB_INVALID,	TYPE_FLOAT, (dataUnionType *)&configurationData.sensor[LOCATION_POST_CATALYST].wideband.Kd},
		{SPN_POSTCAT_NARROWBAND_KP,						MB_INVALID,	TYPE_FLOAT, (dataUnionType *)&configurationData.sensor[LOCATION_POST_CATALYST].narrowband.Kp},
		{SPN_POSTCAT_NARROWBAND_KI,						MB_INVALID,	TYPE_FLOAT, (dataUnionType *)&configurationData.sensor[LOCATION_POST_CATALYST].narrowband.Ki},
		{SPN_POSTCAT_NARROWBAND_KD,						MB_INVALID,	TYPE_FLOAT, (dataUnionType *)&configurationData.sensor[LOCATION_POST_CATALYST].narrowband.Kd},
		{SPN_POSTCAT_NERNST_KP,							MB_INVALID,	TYPE_FLOAT,	(dataUnionType *)&configurationData.sensor[LOCATION_POST_CATALYST].nernst.Kp},
		{SPN_POSTCAT_NERNST_KI,							MB_INVALID,	TYPE_FLOAT,	(dataUnionType *)&configurationData.sensor[LOCATION_POST_CATALYST].nernst.Ki},
		{SPN_POSTCAT_NERNST_KD,							MB_INVALID,	TYPE_FLOAT,	(dataUnionType *)&configurationData.sensor[LOCATION_POST_CATALYST].nernst.Kd},
		{SPN_POSTCAT_WIDEBAND_LAMBDA_SETPOINT,			MB_INVALID,	TYPE_FLOAT,	(dataUnionType *)&configurationData.sensor[LOCATION_POST_CATALYST].wideband.setpoint},
		{SPN_POSTCAT_NARROWBAND_VOLTAGE_SETPOINT,		MB_INVALID,	TYPE_FLOAT,	(dataUnionType *)&configurationData.sensor[LOCATION_POST_CATALYST].narrowband.setpoint},
		{SPN_RAW_ADC_NERNST_POSTCAT,					MB_INVALID,	TYPE_FLOAT,	(dataUnionType *)&operationalData.adc[ADC_NERNST_POST].adcVolts_unfiltered},
		{SPN_RAW_ADC_WIDEBAND_POSTCAT,					MB_INVALID,	TYPE_FLOAT,	(dataUnionType *)&operationalData.adc[ADC_O2_SENSOR_POST].adcVolts_unfiltered},
		{SPN_RAW_ADC_NARROWBAND_POSTCAT,				MB_INVALID,	TYPE_FLOAT,	(dataUnionType *)&operationalData.adc[ADC_O2_SENSOR_POST].adcVolts_unfiltered},
	#endif

		{SPN_O2_SENSOR_ERROR_TIME,						MB_INVALID,	TYPE_U16,	(dataUnionType *)&configurationData.O2sensorErrorTime_seconds},
		{SPN_AUTOSTART_DELAY,							MB_INVALID,	TYPE_U16,	(dataUnionType *)&configurationData.autostartDelay},
		{SPN_DPV_HOME_POSITION,							MB_INVALID,	TYPE_FLOAT,	(dataUnionType *)&configurationData.dpvHomePosition},
		{SPN_ADAPTIVE_GAIN_LOWEST_VALUE,				MB_INVALID,	TYPE_FLOAT,	(dataUnionType *)&configurationData.adaptiveGainLowestValue},
		{SPN_NEVER_GIVE_UP,								MB_INVALID,	TYPE_U8,	(dataUnionType *)&configurationData.neverGiveUpIfTrue},

		{SPN_HEATER_DUTY_CYCLE_BANK_1,					MB_INVALID,	TYPE_FLOAT,	(dataUnionType *)&sensorData[LOCATION_BANK_1].heater.dutycycle_actual},
		{SPN_HEATER_CONTROL_TIME_BANK_1,				MB_INVALID,	TYPE_U16,	(dataUnionType *)&sensorData[LOCATION_BANK_1].heater.controlTime_ms},
		{SPN_HEATER_PID_STATE_BANK_1,					MB_INVALID,	TYPE_U8,	(dataUnionType *)&sensorData[LOCATION_BANK_1].heater.state},
		{SPN_HEATER_PID_ENABLED_BANK_1,					MB_INVALID,	TYPE_U8,	(dataUnionType *)&sensorData[LOCATION_BANK_1].heater.pid.enabled},

		{SPN_MANIFOLD_PRESSURE_LEFT_BANK_1,				MB_INVALID,	TYPE_FLOAT,	(dataUnionType *)&adcData[MANIFOLD_PRESSURE_LEFT].current},

	#if (MY_MODULE_TYPE == AFRC_ADVANCED_TYPE)
		{SPN_HEATER_DUTY_CYCLE_BANK_2,					MB_INVALID,	TYPE_FLOAT,	(dataUnionType *)&sensorData[LOCATION_BANK_2].heater.dutycycle_actual},
		{SPN_HEATER_CONTROL_TIME_BANK_2,				MB_INVALID,	TYPE_U16,	(dataUnionType *)&sensorData[LOCATION_BANK_2].heater.controlTime_ms},
		{SPN_HEATER_PID_STATE_BANK_2,					MB_INVALID,	TYPE_U8,	(dataUnionType *)&sensorData[LOCATION_BANK_2].heater.state},
		{SPN_HEATER_PID_ENABLED_BANK_2,					MB_INVALID,	TYPE_U8,	(dataUnionType *)&sensorData[LOCATION_BANK_2].heater.pid.enabled},

		{SPN_HEATER_DUTY_CYCLE_POSTCAT,					MB_INVALID,	TYPE_FLOAT,	(dataUnionType *)&sensorData[LOCATION_POST_CATALYST].heater.dutycycle_actual},
		{SPN_HEATER_CONTROL_TIME_POSTCAT,				MB_INVALID,	TYPE_U16,	(dataUnionType *)&sensorData[LOCATION_POST_CATALYST].heater.controlTime_ms},
		{SPN_HEATER_PID_STATE_POSTCAT,					MB_INVALID,	TYPE_U8,	(dataUnionType *)&sensorData[LOCATION_POST_CATALYST].heater.state},
		{SPN_HEATER_PID_ENABLED_POSTCAT,				MB_INVALID,	TYPE_U8,	(dataUnionType *)&sensorData[LOCATION_POST_CATALYST].heater.pid.enabled},

		// Undefined
		{SPN_AUXILARY_TEMPERATURE_1,					MB_INVALID,	TYPE_FLOAT,	(dataUnionType *)&operationalData.temperatureC[TC_SPARE_1]},
		{SPN_ANALOG_VOLTAGE_1,							MB_INVALID,	TYPE_FLOAT,	(dataUnionType *)&adcData[ANALOG_1].volts},
		{SPN_ANALOG_CURRENT_1,							MB_INVALID,	TYPE_FLOAT,	(dataUnionType *)&adcData[ANALOG_1].current},
		{SPN_ANALOG_1_IS_4_20MA,						MB_INVALID,	TYPE_U8,	(dataUnionType *)&configurationData.adc[ANALOG_1].inputIsCurrent},
		{SPN_AMBIENT_TEMPERATURE_THERMISTOR,			MB_INVALID,	TYPE_FLOAT,	(dataUnionType *)&operationalData.temperatureC[TM_AMBIENT]},

		{SPN_MANIFOLD_PRESSURE_RIGHT_BANK_2,			MB_INVALID,	TYPE_FLOAT,	(dataUnionType *)&adcData[MANIFOLD_PRESSURE_RIGHT].current},
		{SPN_ENGINE_SPEED,								MB_INVALID,	TYPE_FLOAT,	(dataUnionType *)&operationalData.engine.rpm},
		{SPN_NUMBER_OF_TEETH_ON_FLYWHEEL,				MB_INVALID,	TYPE_U16,	(dataUnionType *)&configurationData.engine.numberOfTeethOnFlywheel},
	#endif

		// Module information
		{SPN_ECU_UPTIME,								MB_ECU_UPTIME,			TYPE_FLOAT, (dataUnionType *)&operationalData.control.runtime},
		{SPN_ECU_RUNTIME,								MB_ECU_RUNTIME,			TYPE_FLOAT, (dataUnionType *)&runtimeData.control.totalRuntime},
		{SPN_EMIT_SERIAL_NUMBER,						MB_EMIT_SERIAL_NUMBER,	TYPE_U32,	(dataUnionType *)&operationalData.control.serialNumber},
		{SPN_HARDWARE_ID,								MB_HARDWARE_ID,			TYPE_U16,	(dataUnionType *)&configurationData.control.hardwareID},
		{SPN_APPLICATION_ID,							MB_APPLICATION_ID,		TYPE_U16,	(dataUnionType *)&configurationData.control.applicationID},
		{SPN_TEMPERATURE_CONFIG_C_OR_F,					MB_INVALID,				TYPE_U8,	(dataUnionType *)&configurationData.temperatureIsFarenheitIfTrue},

		// Multi-setpoint table
		{SPN_MULTISETPOINT_TABLE_SETUP,					MB_INVALID,	TYPE_MST_UPDATE,(dataUnionType *)pNULL},
		{SPN_MULTISETPOINT_TABLE_CONFIG,				MB_INVALID,	TYPE_U8,		(dataUnionType *)&operationalData.multiSetpointTable.config},
		{SPN_MULTISETPOINT_TABLE_LEFT_OR_RIGHT,			MB_INVALID,	TYPE_U8,		(dataUnionType *)&operationalData.multiSetpointTable.leftIfFalse_rightIfTrue},
		{SPN_MULTISETPOINT_TABLE_SOURCE,				MB_INVALID,	TYPE_U16,		(dataUnionType *)&operationalData.multiSetpointTable.sourceSPN},
		{SPN_MULTISETPOINT_TABLE_ENTRY_0_INPUT,			MB_INVALID,	TYPE_FLOAT,		(dataUnionType *)&operationalData.multiSetpointTable.row[0].input},
		{SPN_MULTISETPOINT_TABLE_ENTRY_0_OFFSET,		MB_INVALID,	TYPE_FLOAT,		(dataUnionType *)&operationalData.multiSetpointTable.row[0].offset},
		{SPN_MULTISETPOINT_TABLE_ENTRY_1_INPUT,			MB_INVALID,	TYPE_FLOAT,		(dataUnionType *)&operationalData.multiSetpointTable.row[1].input},
		{SPN_MULTISETPOINT_TABLE_ENTRY_1_OFFSET,		MB_INVALID,	TYPE_FLOAT,		(dataUnionType *)&operationalData.multiSetpointTable.row[1].offset},
		{SPN_MULTISETPOINT_TABLE_ENTRY_2_INPUT,			MB_INVALID,	TYPE_FLOAT,		(dataUnionType *)&operationalData.multiSetpointTable.row[2].input},
		{SPN_MULTISETPOINT_TABLE_ENTRY_2_OFFSET,		MB_INVALID,	TYPE_FLOAT,		(dataUnionType *)&operationalData.multiSetpointTable.row[2].offset},
		{SPN_MULTISETPOINT_TABLE_ENTRY_3_INPUT,			MB_INVALID,	TYPE_FLOAT,		(dataUnionType *)&operationalData.multiSetpointTable.row[3].input},
		{SPN_MULTISETPOINT_TABLE_ENTRY_3_OFFSET,		MB_INVALID,	TYPE_FLOAT,		(dataUnionType *)&operationalData.multiSetpointTable.row[3].offset},
		{SPN_MULTISETPOINT_TABLE_ENTRY_4_INPUT,			MB_INVALID,	TYPE_FLOAT,		(dataUnionType *)&operationalData.multiSetpointTable.row[4].input},
		{SPN_MULTISETPOINT_TABLE_ENTRY_4_OFFSET,		MB_INVALID,	TYPE_FLOAT,		(dataUnionType *)&operationalData.multiSetpointTable.row[4].offset},
		{SPN_MULTISETPOINT_TABLE_ROW_0_ENABLED,			MB_INVALID,	TYPE_U8,		(dataUnionType *)&operationalData.multiSetpointTable.row[0].enabled},
		{SPN_MULTISETPOINT_TABLE_ROW_1_ENABLED,			MB_INVALID,	TYPE_U8,		(dataUnionType *)&operationalData.multiSetpointTable.row[1].enabled},
		{SPN_MULTISETPOINT_TABLE_ROW_2_ENABLED,			MB_INVALID,	TYPE_U8,		(dataUnionType *)&operationalData.multiSetpointTable.row[2].enabled},
		{SPN_MULTISETPOINT_TABLE_ROW_3_ENABLED,			MB_INVALID,	TYPE_U8,		(dataUnionType *)&operationalData.multiSetpointTable.row[3].enabled},
		{SPN_MULTISETPOINT_TABLE_ROW_4_ENABLED,			MB_INVALID,	TYPE_U8,		(dataUnionType *)&operationalData.multiSetpointTable.row[4].enabled},

		// Thermcouple gain and offset
		{SPN_EXHAUST_TEMPERATURE_PRECAT_GAIN,			MB_INVALID,	TYPE_FLOAT,		(dataUnionType *)&configurationData.exhaustTemperature.precatGain},
		{SPN_EXHAUST_TEMPERATURE_PRECAT_OFFSET,			MB_INVALID,	TYPE_FLOAT,		(dataUnionType *)&configurationData.exhaustTemperature.precatOffset},
		{SPN_EXHAUST_TEMPERATURE_POSTCAT_GAIN,			MB_INVALID,	TYPE_FLOAT,		(dataUnionType *)&configurationData.exhaustTemperature.postcatGain},
		{SPN_EXHAUST_TEMPERATURE_POSTCAT_OFFSET,		MB_INVALID,	TYPE_FLOAT,		(dataUnionType *)&configurationData.exhaustTemperature.postcatOffset},

		{SPN_CALIBRATE_DPV_HOME_POSITION,				MB_INVALID,	TYPE_U8,		(dataUnionType *)&operationalData.masterRequest_calibrateDPVhomePosition},
		{SPN_HOME_POSITION_CALIBRATION_COUNTER,			MB_INVALID,	TYPE_U16,		(dataUnionType *)&stepperMotorData[LOCATION_BANK_1].homeCounter},
		{SPN_MINIMUM_EXHAUST_TEMPERATURE,				MB_INVALID,	TYPE_FLOAT,		(dataUnionType *)&configurationData.exhaustTemperature.minimum},
		{SPN_MAXIMUM_EXHAUST_TEMPERATURE,				MB_INVALID,	TYPE_FLOAT,		(dataUnionType *)&configurationData.exhaustTemperature.maximum},

	#ifdef PRODUCTION_TEST
		// Production Test
		{SPN_PRODUCTION_TEST_STATE_SLAVE,				MB_INVALID,	TYPE_U8,		(dataUnionType *)&operationalData.test.state},
		{SPN_PRODUCTION_TEST_STATE_MASTER,				MB_INVALID,	TYPE_U8,		(dataUnionType *)&operationalData.test.masterState},
		{SPN_PRODUCTION_TEST_SELECT_DPV,				MB_INVALID,	TYPE_U8,		(dataUnionType *)&operationalData.test.dpvSelect},
		{SPN_PRODUCTION_TEST_SET_DPV_PHASE,				MB_INVALID,	TYPE_U8,		(dataUnionType *)&operationalData.test.dpvPhase},
		{SPN_PRODUCTION_TEST_SELECT_HEATER,				MB_INVALID,	TYPE_U8,		(dataUnionType *)&operationalData.test.heaterSelect},
		{SPN_PRODUCTION_TEST_SET_SENSOR_HEATER_PWM,		MB_INVALID,	TYPE_FLOAT,		(dataUnionType *)&operationalData.test.heaterValue},
		{SPN_PRODUCTION_TARGET_TEMPERATURE,				MB_INVALID,	TYPE_FLOAT,		(dataUnionType *)&operationalData.test.targetTemperature},
		#if (MY_MODULE_TYPE != AFRC_LITE_TYPE)
			{SPN_PRODUCTION_OK_TO_CALIBRATE,			MB_INVALID,	TYPE_U8,		(dataUnionType *)&operationalData.test.okToCalibrate},
		#endif
		{SPN_PRODUCTION_CALIBRATED,						MB_INVALID,	TYPE_U8,		(dataUnionType *)&factoryCalibration.calibrated},
	#endif
	#if (MY_MODULE_TYPE == AFRC_LITE_TYPE)
		#ifdef USE_SIMULATED_DATA
			// Try to mess up the EIM by putting something unexpected on the CAN bus
			{SPN_TACHOGRAPH_VEHICLE_SPEED,				MB_INVALID,	TYPE_FLOAT,		(dataUnionType *)&engineSimulation.vehicleSpeed},
			{SPN_NAVIGATION_BASED_VEHICLE_SPEED,		MB_INVALID,	TYPE_FLOAT,		(dataUnionType *)&engineSimulation.gpsVehicleSpeed},
		#endif
	#endif
		{SPN_X,											MB_INVALID,	TYPE_U8,		(dataUnionType *)&operationalData.display.x},
		{SPN_Y,											MB_INVALID,	TYPE_U8,		(dataUnionType *)&operationalData.display.y},
		{SPN_STRING,									MB_INVALID,	TYPE_STRING,	(dataUnionType *)&operationalData.display.string},
	};
#endif

const int NUMBER_OF_SPNS_SUPPORTED_BY_THIS_MODULE = sizeof(ioTable)/sizeof(ioTableType);

//=================================================================================================
// Functions
//=================================================================================================

//-------------------------------------------------------------------------------------------------
// Returns FAIL or the index
//-------------------------------------------------------------------------------------------------
ErrorCodeIntType getSPNMasterTableIndex(spnEnumType spn) {
	U16 index;

	if (spn == SPN_INVALID) {
		return(FAIL);
	}

	for (index=0; index<NUMBER_OF_SPNS_IN_MASTER_TABLE; index++) {
		if (spn == spnMasterTable[index].spn) {
			return(index);
		}
	}

	#ifdef __DEBUG
		// Wait here forever because this should never ever happen. It indicates a missing SLOT for a given SPN.
//		while(TRUE);
	#endif
	return(FAIL);
}

//-------------------------------------------------------------------------------------------------
// Returns FAIL or the index
//-------------------------------------------------------------------------------------------------
ErrorCodeIntType getModbusModuleIndex(modbusEnumType mbID) {
	U16 index;

	for (index=0; index<NUMBER_OF_SPNS_SUPPORTED_BY_THIS_MODULE; index++) {
		if (ioTable[index].mbID == mbID) {
			return(index);
		}
	}

	#ifdef __DEBUG
		// Wait here forever because this should never ever happen. It indicates a missing SLOT for a given SPN.
		while(TRUE);
	#endif
	return(FAIL);
}

//-------------------------------------------------------------------------------------------------
// Returns FAIL or the index
//-------------------------------------------------------------------------------------------------
ErrorCodeIntType getSPNmoduleIndex(spnEnumType spn) {
	U16 index;

	for (index=0; index<NUMBER_OF_SPNS_SUPPORTED_BY_THIS_MODULE; index++) {
		if (spn == ioTable[index].spn) {
			return(index);
		}
	}

	#ifdef __DEBUG
		// Wait here forever because this should never ever happen. It indicates a missing SLOT for a given SPN.
//		while(TRUE);
	#endif
	return(FAIL);
}

//-------------------------------------------------------------------------------------------------
// Returns FAIL or the index
//-------------------------------------------------------------------------------------------------
ErrorCodeIntType get_IOtableIndex(spnEnumType spn) {
	U16 index;

	for (index=0; index<NUMBER_OF_SPNS_SUPPORTED_BY_THIS_MODULE; index++) {
		if (spn == ioTable[index].spn) {
			return(index);
		}
	}

	return(FAIL);
}

//-------------------------------------------------------------------------------------------------
// Returns FAIL or the index
//-------------------------------------------------------------------------------------------------
ErrorCodeIntType getSLOTindex(slotEnumType slotIdentifier) {
	U16 index;

	for (index=0; index<NUMBER_OF_SLOTS; index++) {
		if (slotIdentifier == slotTable[index].slotIdentifier) {
			return(index);
		}
	}

	#ifdef __DEBUG
		// Wait here forever because this should never ever happen. It indicates a missing SLOT for a given SPN.
		while(TRUE);
	#endif
	return(FAIL);
}

//-------------------------------------------------------------------------------------------------
// Returns NULL on error
//-------------------------------------------------------------------------------------------------
char *getSPN_unitsLabel(spnEnumType spn) {
	int masterTableIndex;
	int slotTableIndex;

#if (MY_MODULE_TYPE == AFRC_LITE_TYPE) || (MY_MODULE_TYPE == AFRC_ADVANCED_TYPE)
	switch (spn) {
	// The multisetpoint table Input column uses the slot index from the sourceSPN 
	case SPN_MULTISETPOINT_TABLE_ENTRY_0_INPUT:
	case SPN_MULTISETPOINT_TABLE_ENTRY_1_INPUT:
	case SPN_MULTISETPOINT_TABLE_ENTRY_2_INPUT:
	case SPN_MULTISETPOINT_TABLE_ENTRY_3_INPUT:
	case SPN_MULTISETPOINT_TABLE_ENTRY_4_INPUT:
		masterTableIndex = getSPNMasterTableIndex(operationalData.multiSetpointTable.sourceSPN);
		break;
	default:
		masterTableIndex = getSPNMasterTableIndex(spn);
		break;
	}
#else
	masterTableIndex = getSPNMasterTableIndex(spn);
#endif

	if (masterTableIndex == FAIL) {
		return(NULL);
	}

	slotTableIndex = getSLOTindex(spnMasterTable[masterTableIndex].slotIdentifier);
	if (slotTableIndex == FAIL) {
		return(NULL);
	}

	return((char *)slotTable[slotTableIndex].label);
}

//-------------------------------------------------------------------------------------------------
// No matter what format is used to store it, return the value as a float with gain and offset
// applied.
//-------------------------------------------------------------------------------------------------
// DWH TBD: Finish this function
//-------------------------------------------------------------------------------------------------
float getSPNValue_float(spnEnumType spn, dataEnumType *pType) {
#define RETURN_ERROR	{*pType = TYPE_UNSUPPORTED;	return(0);}

	int spnMasterIndex;
	int ioTableIndex;
	int slotIndex;
	dataUnionType dataUnion;
	float tempFloat;
	#if (MY_MODULE_TYPE == AFRC_LITE_TYPE) || (MY_MODULE_TYPE == AFRC_ADVANCED_TYPE)
		int tempInt;
	#endif

	//---------------------------------------
	// Find this spn in the master spn table
	//---------------------------------------
	spnMasterIndex = getSPNMasterTableIndex(spn);
	if (spnMasterIndex == FAIL) {
		RETURN_ERROR;
	}

	//-----------------------------------------------
	// Find this spn in this module's spn data table
	//-----------------------------------------------
	ioTableIndex = getSPNmoduleIndex(spn);
	if (ioTableIndex == FAIL) {
		RETURN_ERROR;
	}

#if (MY_MODULE_TYPE == AFRC_LITE_TYPE) || (MY_MODULE_TYPE == AFRC_ADVANCED_TYPE)
	switch (spn) {
	// The multisetpoint table Input column uses the slot index from the sourceSPN 
	case SPN_MULTISETPOINT_TABLE_ENTRY_0_INPUT:
	case SPN_MULTISETPOINT_TABLE_ENTRY_1_INPUT:
	case SPN_MULTISETPOINT_TABLE_ENTRY_2_INPUT:
	case SPN_MULTISETPOINT_TABLE_ENTRY_3_INPUT:
	case SPN_MULTISETPOINT_TABLE_ENTRY_4_INPUT:
		tempInt = getSPNMasterTableIndex(operationalData.multiSetpointTable.sourceSPN);
		if (tempInt != FAIL) {
			slotIndex = getSLOTindex(spnMasterTable[tempInt].slotIdentifier);
		} else {
			slotIndex = FAIL;
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
		RETURN_ERROR;
	}

	// Found everything we need to lookup the value
	switch (spnMasterTable[spnMasterIndex].slotIdentifier) {
	case SAE_TM04:
		// 1 byte float
		switch (ioTable[ioTableIndex].dataType) {
		case TYPE_FLOAT:
			dataUnion = *ioTable[ioTableIndex].pValue;

			// Convert to a float
			tempFloat = dataUnion.u8 * 1.0;

			// Subtract offset
			tempFloat -= slotTable[slotIndex].offset;

			// Divide by gain
			tempFloat /= slotTable[slotIndex].gain;
			return(tempFloat);
		default:
			RETURN_ERROR;
		}
		break;
	default:
		RETURN_ERROR;
	}
}

// Last Line
