#ifndef GRAPHICS_VIDEOOVERLAYENGINE_H
#define GRAPHICS_VIDEOOVERLAYENGINE_H

#include "graphics/Engine.h"

class VideoOverlayEngine : public Engine 
{
public:
	VideoOverlayEngine( Nv2a &nv2a );
	virtual ~VideoOverlayEngine();

	virtual const char* GetName() { return "Video Overlay Engine"; }

	virtual u32 GetRegBase() { return NV2A_MMIO_BASE_PVIDEO; }

	virtual bool MmioWrite32( u32 reg, u32 data );

	u32 GetOverlayAddress() { return m_overlayBase; }

private:
	u32 m_overlayBase;
};

#endif /*GRAPHICS_VIDEOOVERLAYENGINE_H*/

