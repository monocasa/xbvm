#ifndef XBVM_SMBUS_SERIALEEPROM_H
#define XBVM_SMBUS_SERIALEEPROM_H

#include "smbus/SmBusDevice.h"

class SerialEeprom : public SmBusDevice
{
public:
	SerialEeprom();
	virtual ~SerialEeprom();

	virtual const char* GetName() { return "EEPROM"; }

	virtual bool ReadCommand8( u8 command, u8 *data );

	virtual u8 GetAddr() { return 0x54; }

private:
	u8 m_data[256];
};

extern SerialEeprom *g_eeprom;

#endif /*XBVM_SMBUS_SERIALEEPROM_H*/

