#ifndef GRAPHICS_ENGINE_H
#define GRAPHICS_ENGINE_H

#include "graphics/Nv2a.h"
#include "Device.h"

class Engine : public Device
{
public:
	Engine( Nv2a& nv2a );
	virtual ~Engine();

	virtual bool MmioWrite32( u32 reg, u32 data );
	virtual bool MmioWrite8( u32 reg, u8 data );
	virtual u32 MmioRead32( u32 reg );
	virtual u8 MmioRead8( u32 reg );

	virtual u32 GetRegBase() = 0;

protected:
	Nv2a &m_nv2a;
};

#endif /*GRAPHICS_ENGINE_H*/

