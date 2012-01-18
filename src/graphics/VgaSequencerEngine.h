#ifndef GRAPHICS_VGASEQUENCERENGINE_H
#define GRAPHICS_VGASEQUENCERENGINE_H

#include "graphics/Engine.h"

class VgaSequencerEngine : public Engine
{
public:
	VgaSequencerEngine( Nv2a &nv2a );
	virtual ~VgaSequencerEngine();

	virtual const char* GetName() { return "NV2A VGA Sequencer Engine"; }

	virtual bool MmioWrite8( u32 reg, u8 data );

	virtual u32 GetRegBase() { return NV2A_MMIO_BASE_PRMVIO; }
};

#endif /*GRAPHICS_VGASEQUENCERENGINE_H*/

