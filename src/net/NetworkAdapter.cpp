#include "net/NetworkAdapter.h"
#include "Log.h"
#include "Mem.h"

NetworkAdapter *g_net;

NetworkAdapter::NetworkAdapter() : PciDevice( 0, 4, 0, PCI_NVIDIA_NETWORK_DEVICE_ID ),
   m_mmiobase( 0 ),
   m_ioportbase( 0 )
{
	for( int i = 0; i < 6; i++ ) {
		m_macAddress[i] = 0;
	}
}

NetworkAdapter::~NetworkAdapter()
{
}

bool
NetworkAdapter::WriteBAR( int num, u32 &data )
{
	switch( num ) {
	case 0:
		if( (data & 1) != 0 ) {
			EPRINT( "Mapping Network Adapter BAR0 to IO space not supported %08x", data );
			return false;
		}
		m_mmiobase = data;
		g_mem->AssignMmioRange( m_mmiobase, 0x1000, this );
		return true;

	case 1:
		if( (data & 1) != 1 ) {
			EPRINT( "Mapping Network Adapter BAR1 to non IO space not supported %08x", data );
			return false;
		}
		m_ioportbase = data & 0xFFFE;
		for( int i = 0; i < 16; i++ ) {
			g_mem->AddDeviceToIoMap( this, m_ioportbase + i );
		}
		return true;

	case 2:
	case 3:
	case 4:
	case 5:
		if( data != 0 ) {
			EPRINT( "Assigning a range to unknown BAR%d on %s %08x", num, GetName(), data );
			return false;
		}
		return true;

	default:
		return PciDevice::WriteBAR( num, data );
	}
}

bool
NetworkAdapter::MmioWrite8( u32 addr, u8 data )
{
	u32 reg = addr - m_mmiobase;

	switch( reg ) {
	case 0xa8:
	case 0xa9:
	case 0xaa:
	case 0xab:
	case 0xac:
	case 0xad:
		m_macAddress[ 5 - (reg - 0xa8) ] = data;
		if( reg == 0xad ) {
			DPRINT( "MAC Address=%02x:%02x:%02x:%02x:%02x:%02x", 
			        m_macAddress[ 0 ], m_macAddress[ 1 ], 
			        m_macAddress[ 2 ], m_macAddress[ 3 ], 
			        m_macAddress[ 4 ], m_macAddress[ 5 ] );
		}
		return true;

	default:
		return PciDevice::MmioWrite8( addr, data );	
	}
}

