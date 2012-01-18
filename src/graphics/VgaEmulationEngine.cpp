#include "graphics/VgaEmulationEngine.h"
#include "graphics/Nv2a.h"
#include "Log.h"

VgaEmulationEngine::VgaEmulationEngine( Nv2a &nv2a ) : Engine( nv2a ),
	m_crtIndex( 0 ),
	m_crtData( 0 )
{
	m_nv2a.RegisterEngineMmio( NV2A_MMIO_BASE_PRMCIO, this );
}

VgaEmulationEngine::~VgaEmulationEngine()
{
}

bool
VgaEmulationEngine::MmioWrite8( u32 reg, u8 data )
{
	switch( reg ) {
	case 0x3C0:
		//ignore 
		return true;
 
	case 0x3D4:
		m_crtIndex = data;
		return true;

	case 0x3D5:
		DPRINT( "CRT write %02x->%02x", m_crtIndex, data );
		m_crtData = data;
		return true;

	default:
		return Engine::MmioWrite8( reg, data );
	}
}

