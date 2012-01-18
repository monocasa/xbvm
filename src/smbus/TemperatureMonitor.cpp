#include "smbus/TemperatureMonitor.h"

TemperatureMonitor *g_temp;

bool
TemperatureMonitor::ReadCommand8( u8 command, u8 *data )
{
	switch( command ) {
	case 0x0:
		*data = 50;
		return true;

	case 0x1:
		*data = 50;
		return true;

	default:
		return SmBusDevice::ReadCommand8( command, data );
	}
}

