#ifndef SMBUS_TEMPERATUREMONITOR_H
#define SMBUS_TEMPERATUREMONITOR_H

#include "smbus/SmBusDevice.h"

class TemperatureMonitor : public SmBusDevice
{
public:
	TemperatureMonitor()
	{
	}

	virtual ~TemperatureMonitor()
	{
	}

	virtual const char* GetName() { return "SMBus Temperature Monitor"; }

	virtual u8 GetAddr() { return 0x4C; }

	virtual bool ReadCommand8( u8 command, u8 *data );
};

extern TemperatureMonitor *g_temp;

#endif /*SMBUS_TEMPERATUREMONITOR_H*/

