#include "superio/SuperIoController.h"
#include "Mem.h"

SuperIoController *g_superIo;

SuperIoController::SuperIoController( void ) : 
   m_port2E( 0 ),
   m_port2F( 0 ),
   m_port4E( 0 ),
   m_port4F( 0 ),
   m_port61( 0 )
{
	g_mem->AddDeviceToIoMap( this, 0x2e );
	g_mem->AddDeviceToIoMap( this, 0x2f );
	g_mem->AddDeviceToIoMap( this, 0x4e );
	g_mem->AddDeviceToIoMap( this, 0x4f );
	g_mem->AddDeviceToIoMap( this, 0x61 );
}

SuperIoController::~SuperIoController()
{
}

bool
SuperIoController::IoRead8( u16 port, u8 &data )
{
	switch( port ) {
	case 0x2E:
		data = m_port2E;
		return true;

	case 0x2F:
		data = m_port2F;
		return true;

	case 0x4E:
		data = m_port4E;
		return true;

	case 0x4F:
		data = m_port4F;
		return true;

	default:
		return HostDevice::IoRead8( port, data );
	}
}

bool
SuperIoController::IoWrite8( u16 port, u8 &data )
{
	switch( port ) {
	case 0x2E:
		m_port2E = data;
		return true;

	case 0x2F:
		m_port2F = data;
		return true;

	case 0x4E:
		m_port4E = data;
		return true;

	case 0x4F:
		m_port4F = data;
		return true;

	case 0x61:
		m_port61 = data;
		return true;

	default:
		return HostDevice::IoWrite8( port, data );
	}
}

