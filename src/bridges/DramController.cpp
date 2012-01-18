#include "bridges/DramController.h"

DramController* g_dramController;

bool
DramController::WriteConfigSpace( u8 reg, u32 &data )
{
	switch( reg ) {
	case 64:
		m_reg64 = data;
		return true;

	case 68:
		m_reg68 = data;
		return true;

	case 92:
		m_reg92 = data;
		return true;

	case 108:
		m_reg108 = data;
		return true;

	default:
		return PciDevice::WriteConfigSpace( reg, data );
	}
}

