#include "graphics/VideoOverlayEngine.h"
#include "Log.h"

VideoOverlayEngine::VideoOverlayEngine( Nv2a &nv2a ) : Engine( nv2a ),
	m_overlayBase( 0 )
{
	m_nv2a.RegisterEngineMmio( NV2A_MMIO_BASE_PVIDEO, this );
}

VideoOverlayEngine::~VideoOverlayEngine()
{
}

bool
VideoOverlayEngine::MmioWrite32( u32 reg, u32 data )
{
	switch( reg ) {
	case 0:
		m_overlayBase = data;
		return true;

	default:
		return Engine::MmioWrite32( reg, data );
	}
}

