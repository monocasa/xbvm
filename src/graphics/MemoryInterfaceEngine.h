#ifndef GRAPHICS_MEMORYINTERFACEENGINE_H
#define GRAPHICS_MEMORYINTERFACEENGINE_H

#include "graphics/Engine.h"

#define NV2A_REG_PFB_CFG0           (0x200)
#define NV2A_REG_PFB_CFG1           (0x204)
#define NV2A_REG_PFB_MEM_AMOUNT     (0x20C)
#define NV2A_REG_PFB_REFCTRL        (0x210)
#define NV2A_REG_PFB_PRAMIN_WR_PROT (0x214)
#define NV2A_REG_PFB_MEM_TIMINGS_0  (0x220)
#define NV2A_REG_PFB_MEM_TIMINGS_1  (0x224)
#define NV2A_REG_PFB_MEM_TIMINGS_2  (0x228)
#define NV2A_REG_PFB_UNKNOWN_2C0    (0x2C0)
#define NV2A_REG_PFB_UNKNOWN_2C4    (0x2C4)
#define NV2A_REG_PFB_UNKNOWN_2C8    (0x2C8)
#define NV2A_REG_PFB_UNKNOWN_2CC    (0x2CC)
#define NV2A_REG_PFB_REFRESH        (0x2D0)
#define NV2A_REG_PFB_PRECHARGE      (0x2D4)
#define NV2A_REG_PFB_UNKNOWN_2D8    (0x2D8)
#define NV2A_REG_PFB_SELF_REFRESH   (0x2DC)
#define NV2A_REG_pFB_COMP_MAX_TAG   (0x320)
#define NV2A_REG_PFB_UNKNOWN_328    (0x328)
#define NV2A_REG_PFB_UNKNOWN_32C    (0x32C)
#define NV2A_REG_PFB_UNKNOWN_330    (0x330)
#define NV2A_REG_PFB_UNKNOWN_338    (0x338)
#define NV2A_REG_PFB_UNKNOWN_410    (0x410)

class Nv2a;

class MemoryInterfaceEngine : public Engine
{
public:
	MemoryInterfaceEngine( Nv2a& );
	virtual ~MemoryInterfaceEngine();

	virtual const char* GetName() { return "NV2A Memory Interface Engine"; }

	virtual bool MmioWrite32( u32 reg, u32 data );
	virtual u32 MmioRead32( u32 reg );

	virtual u32 GetRegBase() { return NV2A_MMIO_BASE_PFB; }

private:
	u32 m_regs[1024];
};

#endif /*GRAPHICS_MEMORYINTERFACEENGINE_H*/

