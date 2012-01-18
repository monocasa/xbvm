#include "smbus/SmBusDevice.h"
#include "Log.h"

bool
SmBusDevice::WriteCommand8( u8 command, u8 data )
{
	EPRINT( "Unimplemented write of %02x:%02x of %s", command, data, GetName() );
	return false;
}

bool
SmBusDevice::WriteCommand16( u8 command, u16 data )
{
	EPRINT( "Unimplemented write of %02x:%04x of %s", command, data, GetName() );
	return false;
}

bool
SmBusDevice::ReadCommand8( u8 command, u8 *data )
{
	EPRINT( "Unimplemented read from %02x of %s", command, GetName() );
	*data = 0xFF;
	return false;
}

