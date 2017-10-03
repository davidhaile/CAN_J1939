//-------------------------------------------------------------------------------------------------
// IO Table
//-------------------------------------------------------------------------------------------------
// $Header: https://togwotee/ets/svn/ETS_Libraries/trunk/Include/SPN%20Table.h 169 2011-11-28 22:54:21Z dhaile $
//-------------------------------------------------------------------------------------------------
#ifndef IO_TABLE_H
#define IO_TABLE_H

#include "modbus.h"

typedef struct {
	spnEnumType spn;		// CAN SPN
	modbusEnumType mbID;	// Modbus ID
	dataEnumType dataType;
	dataUnionType *pValue;
} ioTableType;

extern const ioTableType ioTable[];
extern const int NUMBER_OF_SPNS_SUPPORTED_BY_THIS_MODULE;

extern ErrorCodeIntType getSPNMasterTableIndex(spnEnumType);
extern ErrorCodeIntType getSPNmoduleIndex(spnEnumType);
extern ErrorCodeIntType getModbusModuleIndex(modbusEnumType);
extern ErrorCodeIntType get_IOtableIndex(spnEnumType);
extern ErrorCodeIntType getSLOTindex(slotEnumType);
extern dataUnionType getSPNValue(spnEnumType, dataEnumType *);
extern char *getSPN_unitsLabel(spnEnumType);

#endif

// Last Line
