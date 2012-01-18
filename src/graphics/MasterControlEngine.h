#ifndef GRAPHICS_MASTERCONTROLENGINE_H
#define GRAPHICS_MASTERCONTROLENGINE_H

#include "graphics/Engine.h"

#define NV2A_REG_PMC_INTR_0                  (0x000100)
#define NV2A_REG_PMC_INTR_EN_0               (0x000140)
#define NV2A_REG_PMC_INTR_EN_0_INTA_HARDWARE (0x00000001)
#define NV2A_REG_PMC_INTR_EN_0_INTA_SOFTWARE (0x00000002)

class MasterControlEngine : public Engine
{
public:
	MasterControlEngine( Nv2a &nv2a );
	virtual ~MasterControlEngine();

	virtual const char* GetName() { return "NV2A Master Control Engine"; }

	virtual u32 GetRegBase() { return NV2A_MMIO_BASE_PMC; }

	virtual bool MmioWrite32( u32 reg, u32 data );
};

#endif /*GRAPHICS_MASTERCONTROLENGINE_H*/

