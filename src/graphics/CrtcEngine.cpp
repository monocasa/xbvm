#include "graphics/CrtcEngine.h"
#include "graphics/Nv2a.h"
#include "Log.h"

CrtcEngine::CrtcEngine( Nv2a &nv2a ) : Engine( nv2a )
{
	m_nv2a.RegisterEngineMmio( NV2A_MMIO_BASE_PCRTC, this );

	for( int i = 0; i< 1024; i++ ) {
		m_regs[ i ] = 0;
	}
}

CrtcEngine::~CrtcEngine()
{
}

bool
CrtcEngine::MmioWrite32( u32 reg, u32 data )
{
	switch( reg ) {
	case NV2A_REG_PCRTC_INTR:
	case NV2A_REG_PCRTC_INTR_EN:
		m_regs[ reg / sizeof(u32) ] = data;
		return true;

	case NV2A_REG_PCRTC_START:
		m_regs[ reg / sizeof(u32) ] = data;
		DPRINT( "CRTC:  FB set to %08x", data );
		return true;

	default:
		return Engine::MmioWrite32( reg, data );
	}
}

u32
CrtcEngine::MmioRead32( u32 reg )
{
	switch( reg ) {
	case NV2A_REG_PCRTC_INTR:
	case NV2A_REG_PCRTC_INTR_EN:
	case NV2A_REG_PCRTC_START:
		return m_regs[ reg / sizeof(u32) ];

	default:
		return Engine::MmioRead32( reg );
	}
}

