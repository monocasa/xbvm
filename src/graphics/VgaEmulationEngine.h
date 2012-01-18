#ifndef GRAPHICS_VGAEMULATIONENGINE_H
#define GRAPHICS_VGAEMULATIONENGINE_H

#include "graphics/Engine.h"

class Nv2a;

class VgaEmulationEngine : public Engine
{
public:
	VgaEmulationEngine( Nv2a& );
	virtual ~VgaEmulationEngine();

	virtual const char* GetName() { return "NV2A VGA Emulation Engine"; }

	virtual bool MmioWrite8( u32 reg, u8 data );

	virtual u32 GetRegBase() { return NV2A_MMIO_BASE_PRMCIO; }

private:
	u8 m_crtIndex;
	u8 m_crtData;
};

#endif /*GRAPHICS_VGAEMULATION_H*/

