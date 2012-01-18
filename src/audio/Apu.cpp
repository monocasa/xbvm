#include "audio/Apu.h"
#include "Log.h"
#include "Mem.h"

Apu *g_apu;

Apu::Apu() : PciDevice( 0, 5, 0, PCI_NVIDIA_AUDIO_APU_DEVICE_ID ),
   m_mmiobase( 0 )
{
}	

Apu::~Apu()
{
}

bool
Apu::WriteBAR( int num, u32 &data )
{
	switch( num ) {
	case 0:
		if( (data & 1) != 0 ) {
			EPRINT( "Can not map %s BAR%d to IO space %08x", GetName(), num, data );
			return false;
		}
		m_mmiobase =  data;
		g_mem->AssignMmioRange( m_mmiobase, 512*1024, this );
		return true;

	default:
		if( data != 0 ) {
			return PciDevice::WriteBAR( num, data );
		}
		return true;
	}
}

