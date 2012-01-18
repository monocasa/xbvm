#include "superio/Pit.h"
#include "Log.h"
#include "Mem.h"

Pit *g_pit;

Pit::Pit( void ) :
	m_highLowToggle( true ),
	m_chan0Ctc( 0 )
{
	g_mem->AddDeviceToIoMap( this, 0x40 );
	g_mem->AddDeviceToIoMap( this, 0x41 );
	g_mem->AddDeviceToIoMap( this, 0x42 );
	g_mem->AddDeviceToIoMap( this, 0x43 );
}

Pit::~Pit( void )
{
}

bool
Pit::IoWrite8( u16 port, u8 &data )
{
	switch( port )
	{
	case 0x40:
		if( m_highLowToggle ) { //Write to low byte
			m_chan0Ctc = data;
			m_highLowToggle = !m_highLowToggle;
		}
		else { //Write to high byte
			m_chan0Ctc = (m_chan0Ctc & 0xff) | (data << 8);
			m_highLowToggle = !m_highLowToggle;
			DPRINT( "Set PIT to %f hz", 1125000.0 / m_chan0Ctc );
		}
		return true;
		
	case 0x41: //Channel 1 (DRAM Refresh) Data port
		//ignore for now
		return true;

	case 0x42: //Channel 2 (Speaker) Data port
		//ignore for now
		return true;

	case 0x43:
		return WriteToMode( data );

	default:
		return HostDevice::IoWrite8( port, data );
	}
}

bool
Pit::WriteToMode( u8 data )
{
	DPRINT( "Write to PIT Mode: Channel %d, Access Mode %d, Operating Mode %d BCD/Binary Mode %d",
	        ((data >> 6 ) & 3), ((data >>  4) & 3), ((data >> 1) & 7), (data & 1) );
	return true;
}

