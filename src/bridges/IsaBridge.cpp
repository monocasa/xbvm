#include "bridges/IsaBridge.h"
#include "Mem.h"

IsaBridge *g_isaBridge;

IsaBridge::IsaBridge() : PciDevice( 0, 1, 0, PCI_NVIDIA_ISA_BRIDGE_DEVICE_ID ), 
	m_ioBase( 0 ),
	m_hardwareMonitorPinPulses( 0 ),
	m_configReg76( 0 ),
	m_configReg84( 0 ),
	m_configReg96( 0 ),
	m_configReg100( 0 ),
	m_configReg106( 0 ),
	m_configReg108( 0 ),
	m_configReg129( 0 ),
	m_configReg132( 0 ),
	m_configReg140( 0 ),
	m_configReg156( 0 ),
	m_configReg180( 0 ),
	m_timerReg( 0 ),
	m_portB4( 0 ),
	m_port02( 0 ),
	m_port04( 0 ),
	m_port20( 0 ),
	m_port22( 0 ),
	m_port23( 0 ),
	m_port28( 0 ),
	m_port26( 0 ),
	m_port49( 0 ),
	m_portC0( 0 ),
	m_portCC( 0 ),
	m_portCD( 0 ),
	m_portCE( 0 ),
	m_portCF( 0 ),
	m_portD3( 0 ),
	m_portD6( 0 ),
	m_portD8( 0 ),
	m_portD9( 0 )
{
	//EISA IRQ Control according to Bochs PORTS.LST.  Makes sense in
	//  context.  This is the best place I could think of to put this. 
	//  Maybe there is a better place? 
	g_mem->AddDeviceToIoMap( this, 0x4d0 );
	g_mem->AddDeviceToIoMap( this, 0x4d1 );

	g_mem->AddDeviceToIoMap( this, 0x92 );

	u32 barAddress = 0x8001;
	WriteBAR( 0, barAddress );
}

IsaBridge::~IsaBridge()
{
}

bool
IsaBridge::ReadConfigSpace8( u8 reg, u8 &data )
{
	switch( reg ) {
	case 129:
		data = m_configReg129;
		return true;

	default:
		return PciDevice::ReadConfigSpace8( reg, data );
	}
}

bool
IsaBridge::ReadConfigSpace( u8 reg, u32 &data )
{
	switch( reg ) {
	case 96:
		data = m_configReg96;
		return true;

	case 84:
		data = m_configReg84;
		return true;

	case 180:
		data = m_configReg180;
		return true;

	default:
		return PciDevice::ReadConfigSpace( reg, data );
	}
}

bool
IsaBridge::WriteConfigSpace8( u8 reg, u8 &data )
{
	switch( reg ) {
	case 106:
		m_configReg106 = data;
		return true;

	case 128:
		EPRINT( "TODO:  Deactivate MCPX ROM" );
		return true;

	case 129:
		m_configReg129 = data;
		return true;

	default:
		return PciDevice::WriteConfigSpace8( reg, data );
	}
}

bool
IsaBridge::WriteConfigSpace( u8 reg, u32 &data )
{
	switch( reg ) {
	case 76:
		m_configReg76 = data;
		return true;

	case 84:
		m_configReg84 = data;
		return true;

	case 96:
		m_configReg96 = data;
		return true;

	case 100:
		m_configReg100 = data;
		return data;

	case 108:
		m_configReg108 = data;
		return true;

	case 132:
		m_configReg132 = data;
		return true;

	case 140:
		m_configReg140 = data;
		return true;

	case 156:
		m_configReg156 = data;
		return true;

	case 180:
		m_configReg180 = data;
		return true;

	default:
		return PciDevice::WriteConfigSpace( reg, data );
	}
}

bool
IsaBridge::WriteBAR( int num, u32 &data )
{
	u16 addr = data & 0xFFFE;

	switch( num ) {
	case 0:
		if( (data & 1) == 0 ) {
			EPRINT( "Setting %s BAR0 to non I/O Space %xh", GetName(), data );
			return false;
		}

		if( (data & 0xFFFE) == m_ioBase ) {
			return true;
		}
		// Mask off I/O bit
		m_ioBase = data & 0xFFFE;
		for( int i = addr; i < addr + 0x100; i++ ) {
			g_mem->AddDeviceToIoMap( this, i );
		}
		return true;

	default:
		return PciDevice::WriteBAR( num, data );
	}
}

bool
IsaBridge::IoWrite8( u16 port, u8 &data )
{
	u16 portOffset = port - m_ioBase;

	if( port == 0x4D0 || port == 0x4D1 || port == 0x92 ) {
		return true;
	}

	switch( portOffset ) {
	case 0x02:
		m_port02 = data;
		return true;

	case 0x04:
		m_port04 = data;
		return true;

	case 0x22:
		m_port22 = data;
		return true;

	case 0x23:
		m_port23 = data;
		return true;

	case 0x26:
		m_port26 = data;
		return true;

	case 0x49:
		m_port49 = data;
		return true;

	case 0x28:
		m_port28 = data;
		return true;

	case 0xC0:
		m_portC0 = data;
		return true;

	case 0xCC:
		m_portCC = data;
		return true;

	case 0xCD:
		m_portCD = data;
		return true;

	case 0xCE:
		m_portCE = data;
		return true;

	case 0xCF:
		if( (m_portCF == 0x05) && (data == 0x04) ) {
			m_hardwareMonitorPinPulses++;
			DPRINT( "Hardware Pulses:  %d", m_hardwareMonitorPinPulses );
		}
		m_portCF = data;
		return true;

	case 0xD3:
		m_portD3 = data;
		return true;

	case 0xD6:
		m_portD6 = data;
		return true;

	case 0xD8:
		m_portD8 = data;
		return true;

	case 0xD9:
		m_portD9 = data;
		return true;

	default:
		return PciDevice::IoWrite8( port, data );
	}
}

bool
IsaBridge::IoRead8( u16 port, u8 &data )
{
	u16 portOffset = port - m_ioBase;

	switch( portOffset ) {
	case 0x02:
		m_port02 = data;
		return true;

	case 0x04:
		m_port04 = data;
		return true;

	case 0x22:
		m_port22 = data;
		return true;

	case 0x23:
		m_port23 = data;
		return true;

	case 0x28:
		m_port28 = data;
		return true;

	default:
		return PciDevice::IoRead8( port, data );
	}
}

bool
IsaBridge::IoWrite16( u16 port, u16 &data )
{
	u16 portOffset = port - m_ioBase;

	switch( portOffset ) {
	case 0x2:
		m_port02 = data;
		return true;

	case 0x4:
		m_port04 = data;
		return true;

	case 0x20:
		m_port20 = data;
		return true;

	case 0x22:
		m_port22 = data;
		return true;

	case 0x23:
		m_port23 = data;
		return true;

	case 0x28:
		m_port28 = data;
		return true;

	default:
		return PciDevice::IoWrite16( port, data );
	}
}

bool
IsaBridge::IoRead16( u16 port, u16 &data )
{
	u16 portOffset = port - m_ioBase;

	switch( portOffset ) {
	case 0x2:
		data = m_port02;
		return true;

	case 0x4:
		data = m_port04;
		return true;

	case 0x20:
		m_port20 = data;
		return true;

	case 0x22:
		data = m_port22;
		return true;

	case 0x28:
		data = m_port28;
		return true;

	default:
		return PciDevice::IoRead16( port, data );
	}
}

bool
IsaBridge::IoWrite32( u16 port, u32 &data )
{
	u16 portOffset = port - m_ioBase;

	switch( portOffset ) {
	case 0xB4:
		m_portB4 = data;
		return true;

	default:
		return PciDevice::IoWrite32( port, data );
	}
}

bool
IsaBridge::IoRead32( u16 port, u32 &data )
{
	u16 portOffset = port - m_ioBase;

	switch( portOffset ) {
	case 0x08:
		m_timerReg += 10;
		data = m_timerReg;
		return true;

	default:
		return PciDevice::IoRead32( port, data );
	}
}

