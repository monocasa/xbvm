#include "superio/Cmos.h"
#include "Mem.h"

Cmos *g_cmos;

Cmos::Cmos( void ) :
	m_reg( 0 ),
	m_statusRegA( 0 ),
	m_statusRegB( 0 ),
	m_statusRegC( 0 ),
	m_statusRegD( 0 )
{
	g_mem->AddDeviceToIoMap( this, 0x70 );
	g_mem->AddDeviceToIoMap( this, 0x71 );
}

Cmos::~Cmos( void )
{
}

bool
Cmos::IoRead8( u16 port, u8 &data )
{
	switch( port )
	{
	case 0x71:
		return ProcessRead( data );

	default:
		return HostDevice::IoRead8( port, data );
	}
}

bool
Cmos::IoWrite8( u16 port, u8 &data )
{
	switch( port )
	{
	case 0x70:
		m_reg = data;
		return true;

	case 0x71:
		return ProcessWrite( data );

	default:
		return HostDevice::IoWrite8( port, data );
	}	
}

bool
Cmos::ProcessRead( u8 &data )
{
	switch( m_reg )
	{
	case 0x0b:
		data = m_statusRegB;
		return true;

	case 0x0c:
		data = m_statusRegC;
		return true;

	case 0x0d:
		data = m_statusRegD;
		return true;

	default:
		EPRINT( "Unknown CMOS register read from %02x", m_reg );
		return false;
	}
}

bool
Cmos::ProcessWrite( u8 data )
{
	switch( m_reg )
	{
	case 0x0a:
		m_statusRegA = data;
		return true;

	case 0x0b:
		m_statusRegB = data;
		return true;

	case 0x0c:
		m_statusRegC = data;
		return true;

	case 0x0d:
		m_statusRegD = data;
		return true;

	default:
		EPRINT( "Unknown CMOS register written to %02x->%02x", data, m_reg );
		return false;
	}
}

