#ifndef XBVM_MEM_H
#define XBVM_MEM_H

#include <cstring>
#include <vector>

#include <unistd.h>

#include "Log.h"
#include "HostDevice.h"
#include "Types.h"

#define MEM_MCPX_SIZE (512)
#define MEM_TSOP_SIZE (1024 * 1024)

#define MEM_NUM_IOPORTS (64 * 1024)
#define MEM_NUM_PHYS_PAGES ( 1024 * 1024 )

class PciDevice;

class Mem
{
public:
	Mem() : m_ram(NULL), m_ramSize(0), m_rom(NULL), m_pageSize(0)
	{
		memset( m_ioPortMapping, 0, sizeof(m_ioPortMapping) );
	}

	bool Initialize( void );

	u32 ReadPhysU32( u32 addr, bool isDebug = false );
	u16 ReadPhysU16( u32 addr, bool isDebug = false );
	u8 ReadPhysU8( u32 addr, bool isDebug = false );

	bool WritePhysU32( u32 addr, u32 data );
	bool WritePhysU16( u32 addr, u16 data );
	bool WritePhysU8( u32 addr, u8 data );

	bool ReadIo8( u16 port, u8 &data ) {
		HostDevice *device = m_ioPortMapping[ port ];
		if( NULL == device ) {
			EPRINT( "Unknown u8 read from I/O port %x:  %02x", port, data );
			return false;
		}
		return device->IoRead8( port, data );
	}

	bool ReadIo16( u16 port, u16 &data ) {
		HostDevice *device = m_ioPortMapping[ port ];
		if( NULL == device ) {
			EPRINT( "Unknown u16 read from I/O port %x:  %02x", port, data );
			return false;
		}
		return device->IoRead16( port, data );
	}

	bool ReadIo32( u16 port, u32 &data ) {
		HostDevice *device = m_ioPortMapping[ port ];
		if( NULL == device ) {
			EPRINT( "Unknown u32 read from I/O port %x:  %02x", port, data );
			return false;
		}
		return device->IoRead32( port, data );
	}

	bool WriteIo8( u16 port, u8 data ) {
		HostDevice *device = m_ioPortMapping[ port ];
		if( NULL == device ) {
			EPRINT( "Unknown u8 write to I/O port %x:  %02x", port, data );
			return false;
		}
		return device->IoWrite8( port, data );
	}

	bool WriteIo16( u16 port, u16 data ) {
		HostDevice *device = m_ioPortMapping[ port ];
		if( NULL == device ) {
			EPRINT( "Unknown u16 write to I/O port %x:  %02x", port, data );
			return false;
		}
		return device->IoWrite16( port, data );
	}

	bool WriteIo32( u16 port, u32 data ) {
		HostDevice *device = m_ioPortMapping[ port ];
		if( NULL == device ) {
			EPRINT( "Unknown u32 write to I/O port %x:  %08x", port, data );
			return false;
		}
		return device->IoWrite32( port, data );
	}

	void *GetRamAddr( void )
	{
		return m_ram;
	}

	void *GetRomAddr( void )
	{
		return m_rom;
	}

	u32 GetRamSize( void )
	{
		return m_ramSize;
	}

	u32 GetRomSize( void )
	{
		return ROM_PHYS_SIZE;
	}

	u32 GetPageSize( void )
	{
		if( 0 == m_pageSize ) {
			m_pageSize = sysconf( _SC_PAGESIZE );
		}
		return m_pageSize;
	}

	bool
	AddDeviceToIoMap( HostDevice *device, u16 port )
	{
		if( m_ioPortMapping[ port ] != NULL ) {
			EPRINT( "Trying to add ioPortMapping to existing mapping:  %Xh", port );
			return false;
		}
		m_ioPortMapping[ port ] = device;
		return true;
	}

	bool DeassignMmioRange( u32 base, int size );
	bool AssignMmioRange( u32 base, int size, HostDevice *device );
	std::vector<u32> FindAlignedDwords( u32 value );

private:
	static const u32 RETAIL_RAM_SIZE;
	static const u32 DEBUG_RAM_SIZE;
	static const u32 ROM_PHYS_SIZE;
	static const u32 TSOP_SIZE;
	static const u32 MCPX_SIZE;

	static const char *DEFAULT_MCPX_FILE_PATH;
	static const char *DEFAULT_TSOP_FILE_PATH;

	void* m_ram;
	u32   m_ramSize;
	void* m_rom;
	u8    m_mcpx[MEM_MCPX_SIZE];
	u8    m_tsop[MEM_TSOP_SIZE];
	u32   m_pageSize;

	HostDevice *m_ioPortMapping[ MEM_NUM_IOPORTS ];
	HostDevice *m_mmioPageMapping[ MEM_NUM_PHYS_PAGES ];

	bool AllocateRam( void );
	bool AllocateRom( void );
	bool LoadMcpx( const char *path );
	bool LoadTsop( const char *path );
	bool LoadRom( void );

	bool LoadFile( const char *path, void *buffer, u32 bufferSize );
};

extern Mem *g_mem;

#endif /*XBVM_MEM_H*/

