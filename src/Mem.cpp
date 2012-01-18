#include <cstring>

#include <fcntl.h>

#include "Log.h"
#include "Mem.h"
//#include "PciBus.h"

Mem *g_mem;

const u32 Mem::RETAIL_RAM_SIZE =  64 * 1024 * 1024;
const u32 Mem::DEBUG_RAM_SIZE  = 128 * 1024 * 1024;
const u32 Mem::ROM_PHYS_SIZE   =  16 * 1024 * 1024;
const u32 Mem::MCPX_SIZE       =               512;
const u32 Mem::TSOP_SIZE       =       1024 * 1024;

const char *Mem::DEFAULT_MCPX_FILE_PATH = "/opt/xbvm/mcpx.bin";
const char *Mem::DEFAULT_TSOP_FILE_PATH = "/opt/xbvm/tsop.bin";

bool
Mem::Initialize( void )
{
	const char *mcpxFilePath = DEFAULT_MCPX_FILE_PATH;
	const char *tsopFilePath = DEFAULT_TSOP_FILE_PATH;

	m_ramSize = DEBUG_RAM_SIZE;

	if( !AllocateRam() ) {
		EPRINT( "Unable to allocate RAM" );
		return false;
	}

	if( !AllocateRom() ) {
		EPRINT( "Unable to allocate ROM" );
		return false;
	}

	if( !LoadMcpx( mcpxFilePath) ) {
		EPRINT( "Unable to load MCPX ROM from:  \"%s\"", mcpxFilePath );
		return false;
	}

	if( !LoadTsop( tsopFilePath ) ) {
		EPRINT( "Unable to load TSOP flash from:  \"%s\"", tsopFilePath );
		return false;
	}

	if( !LoadRom() ) {
		EPRINT( "Unable to compile rom" );
		return false;
	}

	return true;
}

u32
Mem::ReadPhysU32( u32 addr, bool isDebug )
{
	if( isDebug )
		return false;

	switch( addr >> 24 )
	{
	// RAM
	case 0x00:
	case 0x01:
	case 0x02:
	case 0x03:
	case 0x04:
	case 0x05:
	case 0x06:
	case 0x07:
		return *(u32*)(&((u8*)m_ram)[ addr ]);
	
	// GPU RAM Mirror
	case 0xF0:
	case 0xF1:
	case 0xF2:
	case 0xF3:
	case 0xF4:
	case 0xF5:
	case 0xF6:
	case 0xF7:
		return *(u32*)(&((u8*)m_ram)[ addr & 0x0FFFFFFF ]);

	// ROM
	case 0xFF:
		return *(u32*)(&((u8*)m_rom)[ addr & 0x00FFFFFF ]);

	default:
		HostDevice *device = m_mmioPageMapping[ addr >> 12 ];
		if( NULL == device ) {
			EPRINT( "Read U32 From Unknown MMIO address %08x", addr );
			return 0xFFFFFFFF;
		}
		return device->MmioRead32( addr );
	}

	return 0xFFFFFFFF;
}

u16
Mem::ReadPhysU16( u32 addr, bool isDebug )
{
	if( isDebug )
		return false;

	switch( addr >> 24 ) {
	// RAM
	case 0x00:
	case 0x01:
	case 0x02:
	case 0x03:
	case 0x04:
	case 0x05:
	case 0x06:
	case 0x07:
		return *(u16*)(&((u8*)m_ram)[ addr ]);

	// GPU RAM Mirror
	case 0xF0:
	case 0xF1:
	case 0xF2:
	case 0xF3:
	case 0xF4:
	case 0xF5:
	case 0xF6:
	case 0xF7:
		return *(u16*)(&((u8*)m_ram)[ addr & 0x0FFFFFFF ]);

	// ROM
	case 0xFF:
		return *(u16*)(&((u8*)m_rom)[ addr & 0x00FFFFFF ]);

	default:
		EPRINT( "Read u16 from unknown phys addr %08x", addr );
		break;
	}

	return 0xFFFF;
}

u8
Mem::ReadPhysU8( u32 addr, bool isDebug )
{
	if( isDebug )
		return false;

	switch( addr >> 24 ) {
	// RAM
	case 0x00:
	case 0x01:
	case 0x02:
	case 0x03:
	case 0x04:
	case 0x05:
	case 0x06:
	case 0x07:
		return ((u8*)m_ram)[ addr ];

	// GPU RAM Mirror
	case 0xF0:
	case 0xF1:
	case 0xF2:
	case 0xF3:
	case 0xF4:
	case 0xF5:
	case 0xF6:
	case 0xF7:
		return ((u8*)m_ram)[ addr ];

	// ROM
	case 0xFF:
		return ((u8*)m_rom)[ addr & 0x00FFFFFF ];

	default:
		HostDevice *device = m_mmioPageMapping[ addr >> 12 ];
		if( NULL == device ) {
			EPRINT( "Read U8 From Unknown MMIO address %08x", addr );
			return 0xFF;
		}
		return device->MmioRead8( addr );
	}

	return 0xFF;
}

bool
Mem::WritePhysU32( u32 addr, u32 data )
{
	switch( addr >> 24 ) {
	default:
		HostDevice *device = m_mmioPageMapping[ addr >> 12 ];
		if( NULL == device ) {
			EPRINT( "Write of %08x to Unknown MMIO address %08x", data, addr );
			return false;
		}
		return device->MmioWrite32( addr, data );
	}
	return false;
}

bool
Mem::WritePhysU16( u32 addr, u16 data )
{
	EPRINT( "Unimplemented: U16 Write of %04x to %08x", data, addr );
	return false;
}

bool
Mem::WritePhysU8( u32 addr, u8 data )
{
	HostDevice *device = m_mmioPageMapping[ addr >> 12 ];
	if( NULL == device ) {
		EPRINT( "Write u8 of %02x to Unknown MMIO address %08x", data, addr );
		return false;
	}
	return device->MmioWrite8( addr, data );
}

bool
Mem::DeassignMmioRange( u32 base, int size )
{
	for( u32 i = base; i < (base + size); i += 0x1000 ) {
		m_mmioPageMapping[ i >> 12 ] = NULL;
	}
	return true;
}

bool
Mem::AssignMmioRange( u32 base, int size, HostDevice *device )
{
	for( u32 i = base; i < (base + size); i += 0x1000 ) {
		m_mmioPageMapping[ i >> 12 ] = device;
	}
	return true;
}

std::vector<u32>
Mem::FindAlignedDwords( u32 value )
{
	u32 *ram = reinterpret_cast<u32*>(m_ram);
	std::vector<u32> addresses;
	for( size_t i = 0; i < (m_ramSize / sizeof(u32)); i++ ) {
		if( ram[i] == value ) {
			addresses.push_back( i * sizeof(u32) );
		}
	}
	return addresses;
}

bool
Mem::AllocateRam( void )
{
	return ( posix_memalign( &m_ram, GetPageSize(), GetRamSize() ) == 0 );
}

bool
Mem::AllocateRom( void )
{
	return ( posix_memalign( &m_rom, GetPageSize(), GetRomSize() ) == 0 );
}

bool
Mem::LoadMcpx( const char *path )
{
	return LoadFile( path, m_mcpx, MEM_MCPX_SIZE );
}

bool
Mem::LoadTsop( const char *path )
{
	return LoadFile( path, m_tsop, MEM_TSOP_SIZE );
}

bool
Mem::LoadRom( void )
{
	u8 *rom = (u8*)m_rom;

	for( int i = 0; i < 16; i++ )
		std::memcpy( &rom[i * MEM_TSOP_SIZE], m_tsop, MEM_TSOP_SIZE );

	std::memcpy( &rom[ROM_PHYS_SIZE - MEM_MCPX_SIZE],  m_mcpx, MEM_MCPX_SIZE );

	return true;
}

bool
Mem::LoadFile( const char *path, void *buffer, u32 bufferSize )
{
	int ret = 0;
	int fd = 0;

	fd = open( path, O_RDONLY );
	if( fd < 0 ) {
		EPRINT( "Failed to open %s", path );
		return false;
	}

	ret = read( fd, buffer, bufferSize );
	if( ret < 0 ) {
		EPRINT( "Failed to read %s", path );
		return false;
	}

	close( fd );

	return true;
}

