#ifndef GRAPHICS_STRAPSENGINE_H
#define GRAPHICS_STRAPSENGINE_H

#include "graphics/Engine.h"

#define NV2A_REG_PSTRAPS_STRAPS0_PRIMARY (0x000)

class Nv2a;

class StrapsEngine : public Engine
{
public:
	StrapsEngine( Nv2a& );
	virtual ~StrapsEngine();

	virtual const char* GetName() { return "NV2A Straps Engine"; }

	virtual bool MmioWrite32( u32 reg, u32 data );
	virtual u32 MmioRead32( u32 reg );

	virtual u32 GetRegBase() { return NV2A_MMIO_BASE_PSTRAPS; }

private:
	u32 m_reg_straps0_primary;
};

#endif /*GRAPHICS_STRAPSENGINE_H*/

