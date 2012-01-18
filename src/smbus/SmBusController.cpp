#include "smbus/ConexantEncoder.h"
#include "smbus/PIC16L.h"
#include "smbus/SerialEeprom.h"
#include "smbus/SmBusController.h"
#include "smbus/TemperatureMonitor.h"
#include "Log.h"
#include "Mem.h"

SmBusController *g_smBus;

SmBusController::SmBusController() : PciDevice( 0, 1, 1, PCI_NVIDIA_SMBUS_DEVICE_ID ),
	m_bar1Base( 0 ),
	m_bar2Base( 0 ),
	m_portc200( 0 ),
	m_address( 0 ),
	m_command( 0 ),
	m_data( 0 ),
	m_status( 0x10 )
{
	g_conexantEncoder = new ConexantEncoder();
	g_pic16l = new PIC16L();
	g_eeprom = new SerialEeprom();
	g_temp = new TemperatureMonitor();
}

SmBusController::~SmBusController( void )
{
}

bool
SmBusController::IoRead8( u16 port, u8 &data )
{
	if( (port >= m_bar1Base) && (port < (m_bar1Base + 16)) ) {
		u16 addr = port - m_bar1Base;
		switch( addr ) {

		case SMBUS_PORT_DATA:
			data= m_data;
			return true;

		case SMBUS_PORT_STATUS:
			data = m_status;
			return true;
		
		default:
			EPRINT( "Read from unimplemented SMBus BAR1 %x", addr );
			return false;
		}
	}
	return PciDevice::IoRead8( port, data );
}

bool
SmBusController::IoRead16( u16 port, u16 &data )
{
	if( (port >= m_bar1Base) && (port < (m_bar1Base + 16)) ) {
		u16 addr = port - m_bar1Base;
		switch( addr ) {

		case SMBUS_PORT_DATA:
			data = m_data;
			return true;

		case SMBUS_PORT_STATUS:
			data = m_status;
			return true;
		
		default:
			EPRINT( "Read from unimplemented SMBus BAR1 %x", addr );
			return false;
		}
	}
	return PciDevice::IoRead16( port, data );
}

bool
SmBusController::IoWrite8( u16 port, u8 &data )
{
	if( (port >= m_bar1Base) && (port < (m_bar1Base + 16)) ) {
		u16 addr = port - m_bar1Base;
		switch( addr ) {

		case SMBUS_PORT_ADDRESS:
			m_address = data;
			return true;

		case SMBUS_PORT_COMMAND:
			m_command = data;
			return true;

		case SMBUS_PORT_CONTROL:
			return WriteControl( data );

		case SMBUS_PORT_DATA:
			m_data = data;
			return true;

		case SMBUS_PORT_STATUS:
			//TODO:  Do we ignore this?
			return true;

		default:
			EPRINT( "Write of %02x to unknown BAR1 smbus port %x %x ", data, addr, port );
			return false;
		}
	}
	else if( (port >= m_bar2Base) && (port < (m_bar2Base + 32)) ) {
		switch( port - m_bar2Base ) {
		case 0:
			m_portc200 = data;
			return true;

		default:
			EPRINT( "Write of %02x to unknown BAR2 smbus port %x ", data, port );
			return false;
		}
	}
	return PciDevice::IoWrite8( port, data );
}

bool
SmBusController::IoWrite16( u16 port, u16 &data )
{
	if( (port >= m_bar1Base) && (port < (m_bar1Base + 16)) ) {
		u16 addr = port - m_bar1Base;
		switch( addr ) {

		case SMBUS_PORT_DATA:
			m_data = data;
			return true;

		case SMBUS_PORT_STATUS:
			//TODO:  Do we ignore this?
			return true;

		default:
			EPRINT( "Write of %04x to unknown BAR1 smbus port %x %x ", data, addr, port );
			return false;
		}
	}
	return PciDevice::IoWrite16( port, data );
}


bool
SmBusController::WriteBAR( int num, u32 &data )
{
	u32 addr = data & 0xFFFFFFFE;
	bool isIo = ((data & 1) == 1);
	switch( num ) {
	case 1:
		if( !isIo ) {
			EPRINT( "Setting SMBus BAR1 to non I/O space not implemented" );
			return false;
		}
		m_bar1Base = addr;
		for( int i = 0; i < 16; i++ ) {
			g_mem->AddDeviceToIoMap( this, m_bar1Base + i );
		}
		return true;

	case 2:
		if( !isIo ) {
			EPRINT( "Setting SMBus BAR1 to non I/O space not implemented" );
			return false;
		}
		m_bar2Base = addr;
		for( int i = 0; i < 32; i++ ) {
			g_mem->AddDeviceToIoMap( this, m_bar2Base + i );
		}
		return true;

	default:
		return PciDevice::WriteBAR( num, data );
	}
}

SmBusDevice*
SmBusController::GetDeviceForAddr( u8 addr )
{
	if( addr == g_conexantEncoder->GetAddr() ) {
		return g_conexantEncoder;
	}
	else if( addr == g_pic16l->GetAddr() ) {
		return g_pic16l;
	}
	else if( addr == g_eeprom->GetAddr() ) {
		return g_eeprom;
	}
	else if( addr == g_temp->GetAddr() ) {
		return g_temp;
	}
	return NULL;
}

bool
SmBusController::WriteControl( u8 control )
{
	if( (control & 0xE4) != 0 ) { //Checking if anything other than bits 0, 1, 3, or 5 are set
		EPRINT( "Unimplemented bits in SMBUS Control:  %02x", control );
		return false;
	}

	if( (control & 8) != 0 ) { //check for start bit
		SmBusDevice* device = GetDeviceForAddr( m_address >> 1 );
		if( NULL == device ) {
			EPRINT( "No Device at SmBus addr %02x", m_address >> 1 );
			return false;
		}

		if( (control & 1) == 0 ) { //eight bit data
			if( (m_address & 1) == 0 ) {
				return device->WriteCommand8( m_command, (u8)m_data ); 
			}
			else {
				return device->ReadCommand8( m_command, (u8*)&m_data );
			}
		}
		else { //sixteen bit data
			EPRINT( "16 bit data in SMBus control not implemented" );
			return false;
		}
	}
	return true;
}

