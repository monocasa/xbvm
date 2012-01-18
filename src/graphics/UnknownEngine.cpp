#include "graphics/UnknownEngine.h"

UnknownEngine::UnknownEngine( Nv2a &nv2a ) : Engine( nv2a ),
	m_fb( 0 )
{
}

UnknownEngine::~UnknownEngine()
{
}

bool
UnknownEngine::MmioWrite32( u32 reg, u32 data )
{
	switch( reg ) {
	// Cromwell writes the framebuffer address here as a video overlay location.
	case 0x608000:
		m_fb = data;
		return true;

	default:
		return Engine::MmioWrite32( reg, data );
	}
}

