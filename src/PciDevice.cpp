#include "Log.h"
#include "PciDevice.h"

PciDevice::~PciDevice( void )
{
}

bool
PciDevice::WriteConfigSpace( u8 reg, u32 &data )
{
	switch( reg ) {
	case PCI_CONFIG_REG_IDS:
		return true;

	case PCI_CONFIG_REG_COMMAND:
		DPRINT( "Write Command %x to %s Config Space", data, GetName() );
		return true;
 
	case PCI_CONFIG_REG_BAR0:
		return WriteBAR( 0, data );

	case PCI_CONFIG_REG_BAR1:
		return WriteBAR( 1, data );

	case PCI_CONFIG_REG_BAR2:
		return WriteBAR( 2, data );

	case PCI_CONFIG_REG_BAR3:
		return WriteBAR( 3, data );

	case PCI_CONFIG_REG_BAR4:
		return WriteBAR( 4, data );

	case PCI_CONFIG_REG_BAR5:
		return WriteBAR( 5, data );

	default:
		//EPRINT( "Write of %08x to reg %d of %s Config Space", data, reg, GetName() );
		return true;
	}
}

bool
PciDevice::WriteConfigSpace8( u8 reg, u8 &data )
{
	switch( reg ) {
	case 0:
		return true;
	default:
		EPRINT( "Write of %02x to config reg %d of %s", data, reg, GetName() );
		return false;
	}
}

bool
PciDevice::ReadConfigSpace( u8 reg, u32 &data )
{
	switch( reg ) {
	case PCI_CONFIG_REG_IDS:
		data = (m_deviceId << 16) | (m_vendorId);
		return true;

	case PCI_CONFIG_REG_COMMAND:
		data = 0;
		return true;

	case PCI_CONFIG_REG_BAR0:
		data = m_bar[0];
		return true;

	case PCI_CONFIG_REG_BAR1:
		data = m_bar[1];
		return true;

	case PCI_CONFIG_REG_BAR2:
		data = m_bar[2];
		return true;

	case PCI_CONFIG_REG_BAR3:
		data = m_bar[3];
		return true;

	case PCI_CONFIG_REG_BAR4:
		data = m_bar[4];
		return true;

	case PCI_CONFIG_REG_BAR5:
		data = m_bar[5];
		return true;

	default:
		//EPRINT( "Read u32 from reg %d of %s Config Space", reg, GetName() );
		data = 0xFFFFFFFF;
		return true;
	}
}

bool
PciDevice::ReadConfigSpace8( u8 reg, u8 &data )
{
	switch( reg ) {
	case 8:
		data = GetClassCode();
		return true;

	case 9:
		data = GetSubClass();
		return true;

	case 10:
		data = GetProg1F();
		return true;

	case PCI_CONFIG_REG_HEADER_TYPE:
		data = GetHeaderType();
		return true;

	default:
		DPRINT( "Read u8 from reg %02x of %s Config Space", reg, GetName() );
		data = 0xFF;
		return false;
	}
}

bool
PciDevice::WriteBAR( int num, u32 &data )
{
	EPRINT( "Unimplemented:  BAR%d set to %08x on %s", num, data, GetName() );
	return false;
}

