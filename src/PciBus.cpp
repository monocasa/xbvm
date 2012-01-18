#include <cstring>

#include "Mem.h"
#include "PciBus.h"

PciBus *g_pciBus;

PciBus::PciBus( void ) :
	m_configAddress( 0 )
{
	memset( m_bus0Devices, 0, sizeof(m_bus0Devices) );
	memset( m_bus1Devices, 0, sizeof(m_bus1Devices) );

	g_mem->AddDeviceToIoMap( this, 0xcf8 );
	g_mem->AddDeviceToIoMap( this, 0xcf9 );
	g_mem->AddDeviceToIoMap( this, 0xcfa );
	g_mem->AddDeviceToIoMap( this, 0xcfb );
	g_mem->AddDeviceToIoMap( this, 0xcfc );
	g_mem->AddDeviceToIoMap( this, 0xcfd );
	g_mem->AddDeviceToIoMap( this, 0xcfe );
	g_mem->AddDeviceToIoMap( this, 0xcff );
}

PciBus::~PciBus( void )
{
}

