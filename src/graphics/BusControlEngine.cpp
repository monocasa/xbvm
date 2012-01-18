#include "graphics/BusControlEngine.h"
#include "graphics/Nv2a.h"

BusControlEngine::BusControlEngine( Nv2a &nv2a ) : Engine( nv2a )
{
	m_nv2a.RegisterEngineMmio( NV2A_MMIO_BASE_PBUS, this );

	for( int i = 0; i < 1024; i++ ) {
		m_regs[ i ] = 0;
	}
}

BusControlEngine::~BusControlEngine()
{
}

bool
BusControlEngine::MmioWrite32( u32 reg, u32 data )
{
	switch( reg ) {
	case NV2A_REG_PBUS_UNKNOWN_0B0:
	case NV2A_REG_PBUS_UNKNOWN_0B4:
	case NV2A_REG_PBUS_UNKNOWN_0B8:
	case NV2A_REG_PBUS_UNKNOWN_0BC:
	case NV2A_REG_PBUS_UNKNOWN_0C4:
	case NV2A_REG_PBUS_UNKNOWN_0C8:
	case NV2A_REG_PBUS_UNKNOWN_0CC:
	case NV2A_REG_PBUS_UNKNOWN_0D4:
	case NV2A_REG_PBUS_UNKNOWN_0D8:
	case NV2A_REG_PBUS_UNKNOWN_0DC:
	case NV2A_REG_PBUS_UNKNOWN_0E8:
	case NV2A_REG_PBUS_ROM_TIMINGS:
	case NV2A_REG_PBUS_UNKNOWN_210:
	case NV2A_REG_PBUS_UNKNOWN_214:
	case NV2A_REG_PBUS_UNKNOWN_218:
	case NV2A_REG_PBUS_UNKNOWN_220:
	case NV2A_REG_PBUS_UNKNOWN_228:
	case NV2A_REG_PBUS_UNKNOWN_22C:
	case NV2A_REG_PBUS_UNKNOWN_230:
	case NV2A_REG_PBUS_UNKNOWN_234:
	case NV2A_REG_PBUS_UNKNOWN_238:
	case NV2A_REG_PBUS_UNKNOWN_23C:
	case NV2A_REG_PBUS_UNKNOWN_240:
	case NV2A_REG_PBUS_UNKNOWN_244:
	case NV2A_REG_PBUS_UNKNOWN_248:
	case NV2A_REG_PBUS_UNKNOWN_24C:
	case NV2A_REG_PBUS_UNKNOWN_250:
	case NV2A_REG_PBUS_UNKNOWN_264:
	case NV2A_REG_PBUS_CLOCK_GATING_2:
	case NV2A_REG_PBUS_CLOCK_GATING_4:
		m_regs[ reg / sizeof(u32) ] = data;
		return true;

	default:
		return Engine::MmioWrite32( reg, data );
	}
}

