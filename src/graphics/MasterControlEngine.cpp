#include "graphics/MasterControlEngine.h"
#include "graphics/Nv2a.h"
#include "Log.h"

MasterControlEngine::MasterControlEngine( Nv2a &nv2a ) : Engine( nv2a )
{
	nv2a.RegisterEngineMmio( NV2A_MMIO_BASE_PMC, this );
}

MasterControlEngine::~MasterControlEngine()
{
}

bool
MasterControlEngine::MmioWrite32( u32 reg, u32 data )
{
	switch( reg ) {
	case NV2A_REG_PMC_INTR_0:
		// I thought this was read-only register?  cromwell writes 0x00000001 to it
		return true;

	case NV2A_REG_PMC_INTR_EN_0:
		if( data & NV2A_REG_PMC_INTR_EN_0_INTA_HARDWARE ) {
			DPRINT( "Enabled Hardware interrupts on NV2A" );
		}
		if( data & NV2A_REG_PMC_INTR_EN_0_INTA_SOFTWARE ) {
			DPRINT( "Enabled Software interrupts on NV2A" );
		}
		if( 0 == data ) {
			DPRINT( "Disabled interrupts on NV2A via PMC_INTR_EN_0" );
		}
		return true;

	default:
		return Engine::MmioWrite32( reg, data );
	}
}

