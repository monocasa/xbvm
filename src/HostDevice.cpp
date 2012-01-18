#include "HostDevice.h"
#include "Log.h"

bool
HostDevice::IoRead32( u16 port, u32 &data )
{
	EPRINT( "Unknown I/O read u32 from %x on %s", port, GetName() );
	data = 0xFFFFFFFF;
	return false;
}

bool
HostDevice::IoRead16( u16 port, u16 &data )
{
	EPRINT( "Unknown I/O read u16 from %x on %s", port, GetName() );
	data = 0xFFFF;
	return false;
}

bool
HostDevice::IoRead8( u16 port, u8 &data )
{
	EPRINT( "Unknown I/O read u8 from %x on %s", port, GetName() );
	data = 0xFF;
	return false;
}

bool
HostDevice::IoWrite32( u16 port, u32 &data )
{
	EPRINT( "Unknown I/O write u32 of %08x to %x on %s", data, port, GetName() );
	return false;
}

bool
HostDevice::IoWrite16( u16 port, u16 &data )
{
	EPRINT( "Unknown I/O write u16 of %04x to %x on %s", data, port, GetName() );
	return false;
}

bool
HostDevice::IoWrite8( u16 port, u8 &data )
{
	EPRINT( "Unknown I/O write u8 of %02x to %x on %s", data, port, GetName() );
	return false;
}

bool
HostDevice::MmioWrite32( u32 addr, u32 data )
{
	EPRINT( "Unknown MMIO write u32 of %08x to %08x on %s", data, addr, GetName() );
	return false;
}

bool
HostDevice::MmioWrite16( u32 addr, u16 data )
{
	EPRINT( "Unknown MMIO write u16 of %04x to %08x on %s", data, addr, GetName() );
	return false;
}

bool
HostDevice::MmioWrite8( u32 addr, u8 data )
{
	EPRINT( "Unknown MMIO write u8 of %02x to %08x on %s", data, addr, GetName() );
	return false;
}

u32
HostDevice::MmioRead32( u32 addr )
{
	EPRINT( "Unknown MMIO read u32 from %08x on %s", addr, GetName() );
	return 0xFFFFFFFF; 
}

u16
HostDevice::MmioRead16( u32 addr )
{
	EPRINT( "Unknown MMIO read u16 from %08x on %s", addr, GetName() );
	return 0xFFFF; 
}

u8
HostDevice::MmioRead8( u32 addr )
{
	EPRINT( "Unknown MMIO read u8 from %08x on %s", addr, GetName() );
	return 0xFF; 
}

