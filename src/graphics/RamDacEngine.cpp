#include "graphics/RamDacEngine.h"
#include "graphics/Nv2a.h"

RamDacEngine::RamDacEngine( Nv2a &nv2a ) : Engine( nv2a )
{
	m_nv2a.RegisterEngineMmio( NV2A_MMIO_BASE_PRAMDAC, this );

	for( int i = 0; i < 1024; i++ ) {
		m_regs[ i ] = 0;
	}
}

RamDacEngine::~RamDacEngine()
{
}

bool
RamDacEngine::MmioWrite32( u32 reg, u32 data )
{
	switch( reg ) {
	case NV2A_REG_PRAMDAC_CURSOR_POS:
	case NV2A_REG_PRAMDAC_NVPLL:
	case NV2A_REG_PRAMDAC_MPLL:
	case NV2A_REG_PRAMDAC_VPLL:
	case NV2A_REG_PRAMDAC_PLL_CONTROL:
	case NV2A_REG_PRAMDAC_UNKNOWN_630:
	case NV2A_REG_PRAMDAC_VDISPEND:
	case NV2A_REG_PRAMDAC_VTOTAL:
	case NV2A_REG_PRAMDAC_VCRTC:
	case NV2A_REG_PRAMDAC_VSYNCSTART:
	case NV2A_REG_PRAMDAC_VSYNCEND:
	case NV2A_REG_PRAMDAC_VVALIDSTART:
	case NV2A_REG_PRAMDAC_VVALIDEND:
	case NV2A_REG_PRAMDAC_HDISPEND:
	case NV2A_REG_PRAMDAC_HTOTAL:
	case NV2A_REG_PRAMDAC_HCRTC:
	case NV2A_REG_PRAMDAC_HSYNCSTART:
	case NV2A_REG_PRAMDAC_HSYNCEND:
	case NV2A_REG_PRAMDAC_HVALIDSTART:
	case NV2A_REG_PRAMDAC_HVALIDEND:
	case NV2A_REG_PRAMDAC_UNKNOWN_84C:
	case NV2A_REG_PRAMDAC_UNKNOWN_880:
	case NV2A_REG_PRAMDAC_UNKNOWN_884:
	case NV2A_REG_PRAMDAC_UNKNOWN_888:
	case NV2A_REG_PRAMDAC_UNKNOWN_88C:
	case NV2A_REG_PRAMDAC_UNKNOWN_890:
	case NV2A_REG_PRAMDAC_UNKNOWN_894:
	case NV2A_REG_PRAMDAC_UNKNOWN_898:
	case NV2A_REG_PRAMDAC_UNKNOWN_89C:
	case NV2A_REG_PRAMDAC_UNKNOWN_8C4:
		m_regs[ reg / sizeof(u32) ] = data;
		return true;

	default:
		return Engine::MmioWrite32( reg, data );
	}
}

u32
RamDacEngine::MmioRead32( u32 reg )
{
	switch( reg ) {
	case NV2A_REG_PRAMDAC_CURSOR_POS:
	case NV2A_REG_PRAMDAC_NVPLL:
	case NV2A_REG_PRAMDAC_MPLL:
	case NV2A_REG_PRAMDAC_VPLL:
	case NV2A_REG_PRAMDAC_PLL_CONTROL:
		return m_regs[ reg / sizeof(u32) ];

	default:
		return Engine::MmioRead32( reg );
	}
}

