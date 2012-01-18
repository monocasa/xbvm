#include "audio/Ac97Codec.h"
#include "Log.h"
#include "Mem.h"

Ac97Codec *g_ac97;

Ac97Codec::Ac97Codec() : PciDevice( 0, 6, 0, PCI_NVIDIA_AUDIO_AC97_DEVICE_ID ),
   m_ioport1base( 0 ),
   m_ioport2base( 0 ),
   m_mmiobase( 0 )
{
}

Ac97Codec::~Ac97Codec()
{
}

bool
Ac97Codec::WriteBAR( int num, u32 &data )
{
	switch( num ) {
	case 0:
		if( (data & 1) != 1 ) {
			EPRINT( "Can not map %s BAR%d to non IO space %08x", GetName(), num, data );
			return false;
		}
		m_ioport1base =  data & 0xFFFE;
		for( int i = 0; i < 16; i++ ) {
			g_mem->AddDeviceToIoMap( this, m_ioport1base + i );
		}
		return true;

	case 1:
		if( (data & 1) != 1 ) {
			EPRINT( "Can not map %s BAR%d to non IO space %08x", GetName(), num, data );
			return false;
		}
		m_ioport2base =  data & 0xFFFE;
		for( int i = 0; i < 16; i++ ) {
			g_mem->AddDeviceToIoMap( this, m_ioport2base + i );
		}
		return true;

	case 2:
		if( (data & 1) != 0 ) {
			EPRINT( "Can not map %s BAR%d to IO space %08x", GetName(), num, data );
			return false;
		}
		m_mmiobase =  data;
		g_mem->AssignMmioRange( m_mmiobase, 0x1000, this );
		return true;

	default:
		if( data != 0 ) {
			return PciDevice::WriteBAR( num, data );
		}
		return true;
	}
}

