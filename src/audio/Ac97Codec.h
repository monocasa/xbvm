#ifndef XBVM_AUDIO_AC97CODEC_H
#define XBVM_AUDIO_AC97CODEC_H

#include  "PciDevice.h"

class Ac97Codec : public PciDevice
{
public:
	Ac97Codec();
	virtual ~Ac97Codec();

	virtual const char* GetName( void ) { return "AC97 Codec"; }

	virtual bool WriteBAR( int num, u32 &data );
private:
	u32 m_ioport1base;
	u32 m_ioport2base;
	u32 m_mmiobase;
};

extern Ac97Codec *g_ac97;

#endif /*XBVM_AUDIO_AC97CODEC_H*/

