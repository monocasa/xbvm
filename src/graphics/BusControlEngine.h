#ifndef GRAPHICS_BUSCONTROLENGINE_H
#define GRAPHICS_BUSCONTROLENGINE_H

#include "graphics/Engine.h"

#define NV2A_REG_PBUS_UNKNOWN_0B0    (0x0B0)
#define NV2A_REG_PBUS_UNKNOWN_0B4    (0x0B4)
#define NV2A_REG_PBUS_UNKNOWN_0B8    (0x0B8)
#define NV2A_REG_PBUS_UNKNOWN_0BC    (0x0BC)
#define NV2A_REG_PBUS_UNKNOWN_0C4    (0x0C4)
#define NV2A_REG_PBUS_UNKNOWN_0C8    (0x0C8)
#define NV2A_REG_PBUS_UNKNOWN_0CC    (0x0CC)
#define NV2A_REG_PBUS_UNKNOWN_0D4    (0x0D4)
#define NV2A_REG_PBUS_UNKNOWN_0D8    (0x0D8)
#define NV2A_REG_PBUS_UNKNOWN_0DC    (0x0DC)
#define NV2A_REG_PBUS_UNKNOWN_0E8    (0x0E8)
#define NV2A_REG_PBUS_ROM_TIMINGS    (0x200)
#define NV2A_REG_PBUS_UNKNOWN_210    (0x210)
#define NV2A_REG_PBUS_UNKNOWN_214    (0x214)
#define NV2A_REG_PBUS_UNKNOWN_218    (0x218)
#define NV2A_REG_PBUS_UNKNOWN_220    (0x220)
#define NV2A_REG_PBUS_UNKNOWN_228    (0x228)
#define NV2A_REG_PBUS_UNKNOWN_22C    (0x22C)
#define NV2A_REG_PBUS_UNKNOWN_230    (0x230)
#define NV2A_REG_PBUS_UNKNOWN_234    (0x234)
#define NV2A_REG_PBUS_UNKNOWN_238    (0x238)
#define NV2A_REG_PBUS_UNKNOWN_23C    (0x23C)
#define NV2A_REG_PBUS_UNKNOWN_240    (0x240)
#define NV2A_REG_PBUS_UNKNOWN_244    (0x244)
#define NV2A_REG_PBUS_UNKNOWN_248    (0x248)
#define NV2A_REG_PBUS_UNKNOWN_24C    (0x24C)
#define NV2A_REG_PBUS_UNKNOWN_250    (0x250)
#define NV2A_REG_PBUS_UNKNOWN_264    (0x264)
#define NV2A_REG_PBUS_CLOCK_GATING_2 (0x588)
#define NV2A_REG_PBUS_CLOCK_GATING_4 (0x590)


class Nv2a;

class BusControlEngine : public Engine
{
public:
	BusControlEngine( Nv2a &nv2a );
	virtual ~BusControlEngine();

	virtual const char* GetName() { return "NV2A Bus Control Engine"; }

	virtual bool MmioWrite32( u32 reg, u32 data );

	virtual u32 GetRegBase() { return NV2A_MMIO_BASE_PBUS; }

private:
	u32 m_regs[ 1024 ];
};

#endif /*GRAPHICS_BUSCONTROLENGINE_H*/

