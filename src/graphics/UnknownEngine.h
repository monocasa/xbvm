#ifndef GRAPHICS_UNKNOWNENGINE_H
#define GRAPHICS_UNKNOWNENGINE_H

#include "graphics/Engine.h"

class Nv2a;

class UnknownEngine : public Engine
{
public:
	UnknownEngine( Nv2a& );
	virtual ~UnknownEngine();

	virtual const char* GetName() { return "Unknown Engine"; }

	virtual u32 GetRegBase() { return 0; }

	virtual bool MmioWrite32( u32 reg, u32 data );
private:
	u32 m_fb;
};

#endif /*GRAPHICS_UNKNOWNENGINE_H*/

