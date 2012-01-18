#include "graphics/MemoryInterfaceEngine.h"
#include "graphics/Nv2a.h"

MemoryInterfaceEngine::MemoryInterfaceEngine( Nv2a &nv2a ) : Engine( nv2a )
{
	m_nv2a.RegisterEngineMmio( NV2A_MMIO_BASE_PFB, this );

	for( int i = 0; i < 1024; i++ ) {
		m_regs[i] = 0;
	}
}

MemoryInterfaceEngine::~MemoryInterfaceEngine()
{
}

bool
MemoryInterfaceEngine::MmioWrite32( u32 reg, u32 data )
{
	switch( reg ) {
	case NV2A_REG_PFB_CFG0:
	case NV2A_REG_PFB_CFG1:
	case NV2A_REG_PFB_MEM_AMOUNT:
	case NV2A_REG_PFB_REFCTRL:
	case NV2A_REG_PFB_PRAMIN_WR_PROT:
	case NV2A_REG_PFB_MEM_TIMINGS_0:
	case NV2A_REG_PFB_MEM_TIMINGS_1:
	case NV2A_REG_PFB_MEM_TIMINGS_2:
	case NV2A_REG_PFB_UNKNOWN_2C0:
	case NV2A_REG_PFB_UNKNOWN_2C4:
	case NV2A_REG_PFB_UNKNOWN_2C8:
	case NV2A_REG_PFB_UNKNOWN_2CC:
	case NV2A_REG_PFB_REFRESH:
	case NV2A_REG_PFB_PRECHARGE:
	case NV2A_REG_PFB_UNKNOWN_2D8:
	case NV2A_REG_PFB_SELF_REFRESH:
	case NV2A_REG_pFB_COMP_MAX_TAG:
	case NV2A_REG_PFB_UNKNOWN_328:
	case NV2A_REG_PFB_UNKNOWN_32C:
	case NV2A_REG_PFB_UNKNOWN_330:
	case NV2A_REG_PFB_UNKNOWN_338:
	case NV2A_REG_PFB_UNKNOWN_410:
		m_regs[ reg / sizeof(u32) ] = data;
		return true;

	default:
		return Engine::MmioWrite32( reg, data );
	}
}

u32
MemoryInterfaceEngine::MmioRead32( u32 reg )
{
	switch( reg ) {
	case NV2A_REG_PFB_CFG0:
	case NV2A_REG_PFB_CFG1:
	case NV2A_REG_PFB_MEM_AMOUNT:
	case NV2A_REG_PFB_REFCTRL:
	case NV2A_REG_PFB_PRAMIN_WR_PROT:
	case NV2A_REG_PFB_MEM_TIMINGS_0:
	case NV2A_REG_PFB_MEM_TIMINGS_1:
	case NV2A_REG_PFB_MEM_TIMINGS_2:
	case NV2A_REG_PFB_UNKNOWN_2C0:
	case NV2A_REG_PFB_UNKNOWN_2C4:
	case NV2A_REG_PFB_UNKNOWN_2C8:
	case NV2A_REG_PFB_UNKNOWN_2CC:
	case NV2A_REG_PFB_REFRESH:
	case NV2A_REG_PFB_PRECHARGE:
	case NV2A_REG_PFB_UNKNOWN_2D8:
	case NV2A_REG_PFB_SELF_REFRESH:
	case NV2A_REG_pFB_COMP_MAX_TAG:
	case NV2A_REG_PFB_UNKNOWN_328:
	case NV2A_REG_PFB_UNKNOWN_32C:
	case NV2A_REG_PFB_UNKNOWN_330:
	case NV2A_REG_PFB_UNKNOWN_338:
	case NV2A_REG_PFB_UNKNOWN_410:
		return m_regs[ reg / sizeof(u32) ];

	default:
		return Engine::MmioRead32( reg );
	}
}

