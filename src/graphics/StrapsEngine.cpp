#include "graphics/StrapsEngine.h"
#include "graphics/Nv2a.h"

StrapsEngine::StrapsEngine( Nv2a &nv2a ) : Engine( nv2a ),
	m_reg_straps0_primary( 0 )
{
	m_nv2a.RegisterEngineMmio( NV2A_MMIO_BASE_PSTRAPS, this );
}

StrapsEngine::~StrapsEngine()
{
}

bool
StrapsEngine::MmioWrite32( u32 reg, u32 data )
{
	switch( reg ) {
	case NV2A_REG_PSTRAPS_STRAPS0_PRIMARY:
		m_reg_straps0_primary = data;
		return true;

	default:
		return Engine::MmioWrite32( reg, data );
	}
}

u32
StrapsEngine::MmioRead32( u32 reg )
{
	switch( reg ) {
	case NV2A_REG_PSTRAPS_STRAPS0_PRIMARY:
		return m_reg_straps0_primary;

	default:
		return Engine::MmioRead32( reg );
	}
}

