#ifndef XBVM_AUDIO_APU_H
#define XBVM_AUDIO_APU_H

#include "PciDevice.h"

class Apu : public PciDevice
{
public:
	Apu();
	virtual ~Apu();

	virtual const char* GetName( void ) { return "APU"; }

	virtual bool WriteBAR( int num, u32 &data );

private:
	u32 m_mmiobase;
};

extern Apu *g_apu;

#endif /*XBVM_AUDIO_APU_H*/

