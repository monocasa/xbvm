#include "superio/Pic.h"
#include "Mem.h"

Pic *g_pic;

Pic::Pic( void )
{
	g_mem->AddDeviceToIoMap( this, 0x20 );
	g_mem->AddDeviceToIoMap( this, 0x21 );
	g_mem->AddDeviceToIoMap( this, 0xA0 );
	g_mem->AddDeviceToIoMap( this, 0xA1 );
}

Pic::~Pic( void )
{
}

bool
Pic::IoWrite8( u16 port, u8 &data )
{
	switch( port ) {
	case 0x20:
		return CommandWrite( data, true );

	case 0x21:
		return DataWrite( data, true );

	case 0xA0:
		return CommandWrite( data, false );

	case 0xA1:
		return DataWrite( data, false );

	default:
		EPRINT( "Pic %x <- %02x", port, data );
		return HostDevice::IoWrite8( port, data );
	}
}

bool
Pic::CommandWrite( u8 command, bool isMaster )
{
	DPRINT( "Pic::CommandWrite( %02x, %s )", command, isMaster ? "true" : "false" );
	return true;
}

bool
Pic::DataWrite( u8 data, bool isMaster )
{
	DPRINT( "Pic::DataWrite( %02x, %s )", data, isMaster ? "true" : "false" );
	return true;
}

