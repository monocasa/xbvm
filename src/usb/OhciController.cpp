#include "usb/OhciController.h"
#include "Log.h"
#include "Mem.h"

OhciController *g_usb0;
OhciController *g_usb1;

bool
OhciController::WriteConfigSpace( u8 reg, u32 &data )
{
	switch( reg ) {
	case 60:
		m_config60 = data;
		return true;

	default:
		return PciDevice::WriteConfigSpace( reg, data );
	}
}

bool
OhciController::ReadConfigSpace( u8 reg, u32 &data )
{
	switch( reg ) {
	case 60:
		data = m_config60;
		return true;

	default:
		return PciDevice::ReadConfigSpace( reg, data );
	}
}

bool
OhciController::WriteBAR( int num, u32 &data )
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

u32
OhciController::MmioRead32( u32 addr )
{
	u32 reg = addr - m_mmiobase;

	switch( reg ) {
	case OHCI_REG_REVISION:
		return 0x10;

	case OHCI_REG_CONTROL:
		return 0x690;

	case OHCI_REG_COMMANDSTATUS:
		return 1;

	case OHCI_REG_INTERRUPTSTATUS:
		return 0;

	default:
		return PciDevice::MmioRead32( addr );
	}
}

bool
OhciController::MmioWrite32( u32 addr, u32 data )
{
	u32 reg = addr - m_mmiobase;

	switch( reg ) {
	case OHCI_REG_CONTROL:
		return true;

	case OHCI_REG_COMMANDSTATUS:
		return true;

	case OHCI_REG_INTERRUPTSTATUS:
		return true;

	case OHCI_REG_INTERRUPTDISABLE:
		if( data == 0x80000000 ) {
			m_interruptEnabled = false;
		}		
		else {
			EPRINT( "Unknown bits set in Ohci->InterruptDiable %08x", data );
			return false;
		}
		return true;

	default:
		return PciDevice::MmioWrite32( addr, data );
	}
}

