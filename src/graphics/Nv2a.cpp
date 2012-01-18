#include "graphics/BusControlEngine.h"
#include "graphics/CrtcEngine.h"
#include "graphics/Engine.h"
#include "graphics/MasterControlEngine.h"
#include "graphics/MemoryInterfaceEngine.h"
#include "graphics/Nv2a.h"
#include "graphics/RamDacEngine.h"
#include "graphics/StrapsEngine.h"
#include "graphics/UnknownEngine.h"
#include "graphics/VgaEmulationEngine.h"
#include "graphics/VgaSequencerEngine.h"
#include "graphics/VideoOverlayEngine.h"
#include "Log.h"
#include "Mem.h"

Nv2a *g_nv2a;

Nv2a::Nv2a() : PciDevice( 1, 0, 0, PCI_NVIDIA_NV2A_DEVICE_ID ),
	m_mmioBase( 0 ),
	m_mmioBaseAssigned( false ),
	m_unknownEngine( NULL ),
	m_bus( NULL ),
	m_mc( NULL ),
	m_fb( NULL ),
	m_ramdac( NULL ),
	m_crtc( NULL ),
	m_straps( NULL ),
	m_vga( NULL ),
	m_vgaSeq( NULL ),
	m_video( NULL )
{
	m_unknownEngine = new UnknownEngine( *this );

	for( int i = 0; i < NV2A_NUM_ENGINE_MMIO_PAGES; i++ ) {
		m_engineMmioTable[i] = m_unknownEngine;
	}

	m_bus = new BusControlEngine( *this );
	m_mc = new MasterControlEngine( *this );
	m_fb = new MemoryInterfaceEngine( *this );
	m_ramdac = new RamDacEngine( *this );
	m_crtc = new CrtcEngine( *this );
	m_straps = new StrapsEngine( *this );
	m_vga = new VgaEmulationEngine( *this );
	m_vgaSeq = new VgaSequencerEngine( *this );
	m_video = new VideoOverlayEngine( *this );

	u32 mmioBase = 0xFD000000;
	WriteBAR( 0, mmioBase );
	u32 ramBase = 0xF0000000;
	WriteBAR( 1, ramBase );
}

Nv2a::~Nv2a()
{
}

bool
Nv2a::WriteBAR( int num, u32 &data )
{
	switch( num ) {
	case 0:
		if( (data & 1) == 1 ) {
			EPRINT( "%s MMIO BAR set to I/O space:  %08x", GetName(), data );
			return false;
		}
		DPRINT( "%s MMIO set to %08x", GetName(), data );
		m_bar[0] = data;
		if( m_mmioBaseAssigned ) 
			g_mem->DeassignMmioRange( m_mmioBase, NV2A_MMIO_SIZE );
		m_mmioBase = data;
		m_mmioBaseAssigned = true;
		return g_mem->AssignMmioRange( m_mmioBase, 16 * 1025 * 1024, this );

	case 1:
		if( data != 0xF0000000 ) {
			EPRINT( "%s RAM mirror set to unimplemented addr %08x", GetName(), data );
			return false;
		}
		m_bar[1] = data;
		return true;

	case 2:
		//ignore.  This shouldn't be setup to anything.  Cromwell writes 00000008
		return true;

	default:
		if( data != 0 ) {
			return PciDevice::WriteBAR( num, data );
		}
		return true;
	}
}

bool
Nv2a::MmioWrite32( u32 addr, u32 data )
{
	Engine *engine = GetEngineForAddress( addr );

	if( NULL == engine ) {
		EPRINT( "Write u32 of %08x to unknown NV2A MMIO register %06x", data, addr );
		return false;
	}

	//EPRINT( "%08x %08x %08x %s %08x", data, addr, engine->GetRegBase(), engine->GetName(), addr - engine->GetRegBase() );
	//return false;

	return engine->MmioWrite32( addr - m_mmioBase - engine->GetRegBase(), data );
}

bool
Nv2a::MmioWrite8( u32 addr, u8 data )
{
	Engine *engine = GetEngineForAddress( addr );

	if( NULL == engine ) {
		EPRINT( "Write u8 of %02x to unknown NV2A MMIO register %06x", data, addr );
		return false;
	}

	return engine->MmioWrite8( addr - m_mmioBase - engine->GetRegBase(), data );
}

u32
Nv2a::MmioRead32( u32 addr )
{
	Engine *engine = GetEngineForAddress( addr );

	if( NULL == engine ) {
		EPRINT( "Read u32 from unknown NV2A MMIO register %06x", addr );
		return 0xFFFFFFFF;
	}

	return engine->MmioRead32( addr - m_mmioBase - engine->GetRegBase() );
}

u8
Nv2a::MmioRead8( u32 addr )
{
	Engine *engine = GetEngineForAddress( addr );

	if( NULL == engine ) {
		EPRINT( "Read u8 from unknown NV2A MMIO register %06x", addr );
		return 0xFF;
	}

	return engine->MmioRead8( addr - m_mmioBase - engine->GetRegBase() );
}

Engine*
Nv2a::GetEngineForAddress( u32 addr )
{
	if( (addr < m_mmioBase) || (addr >= (m_mmioBase + NV2A_MMIO_SIZE )) || !m_mmioBaseAssigned ) {
		EPRINT( "Somehow got an MMIO read request from non mapped region %08x %08x", addr, m_mmioBase );
		return NULL;
	}

	u32 reg = addr - m_mmioBase;

	return m_engineMmioTable[ reg / NV2A_ENGINE_PAGE_SIZE ];
}

bool 
Nv2a::IsVideoOverlayValid()
{
	return GetOverlayAddress() != 0;
}

u32
Nv2a::GetOverlayAddress()
{
	return m_video->GetOverlayAddress();
}

