#include "graphics/Engine.h"
#include "Log.h"

Engine::Engine( Nv2a &nv2a ) :
   m_nv2a( nv2a )
{
}

Engine::~Engine()
{
}

bool
Engine::MmioWrite32( u32 reg, u32 data )
{
	EPRINT( "Write to unknown MMIO reg %08x->%06x in %s", data, reg, GetName() );
	return false;
}

bool
Engine::MmioWrite8( u32 reg, u8 data )
{
	EPRINT( "Write to unknown MMIO reg %02x->%06x in %s", data, reg, GetName() );
	return false;
}

u32
Engine::MmioRead32( u32 reg )
{
	EPRINT( "Read from unknown u32 MMIO reg %06x in %s", reg, GetName() );
	return 0xFFFFFFFF;
}

u8
Engine::MmioRead8( u32 reg )
{
	EPRINT( "Read from unknown u8 MMIO reg %06x in %s", reg, GetName() );
	return 0xFF;
}

