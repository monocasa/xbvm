#include "graphics/Nv2a.h"
#include "graphics/VgaSequencerEngine.h"

VgaSequencerEngine::VgaSequencerEngine( Nv2a &nv2a ) : Engine( nv2a )
{
	m_nv2a.RegisterEngineMmio( NV2A_MMIO_BASE_PRMVIO, this );
}

VgaSequencerEngine::~VgaSequencerEngine()
{
}

bool
VgaSequencerEngine::MmioWrite8( u32 reg, u8 data )
{
	switch( reg ) {
	case 0x3C4:
	case 0x3C5:
	case 0x3CE:
	case 0x3CF:
		//ignore for now
		return true;

	default:
		return Engine::MmioWrite8( reg, data );
	}
}

