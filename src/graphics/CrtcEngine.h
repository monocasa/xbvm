#ifndef GRAPHICS_CRTCENGINE_H
#define GRAPHICS_CRTCENGINE_H

#include "graphics/Engine.h"

#define NV2A_REG_PCRTC_INTR        (0x100)
#define NV2A_REG_PCRTC_INTR_EN     (0x140)
#define NV2A_REG_PCRTC_START       (0x800)

class Nv2a;

class CrtcEngine : public Engine
{
public:
	CrtcEngine( Nv2a &nv2a );
	virtual ~CrtcEngine();

	virtual const char* GetName() { return "NV2A CRTC"; }

	virtual bool MmioWrite32( u32 reg, u32 data );
	virtual u32 MmioRead32( u32 reg );

	virtual u32 GetRegBase() { return NV2A_MMIO_BASE_PCRTC; }

private:
	u32 m_regs[ 1024 ];
};

#endif /*GRAPHICS_CRTCENGINE*/

