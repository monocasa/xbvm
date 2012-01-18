#include "bridges/AgpBridge.h"

AgpBridge *g_agpBridge;

AgpBridge::~AgpBridge()
{
}

bool
AgpBridge::WriteConfigSpace( u8 reg, u32 &data )
{
	switch( reg ) {
	case 0x10:
	case 0x14:
	case 0x1C:
		return true;

	case 24:
		m_configTimers = data;
		return true;

	case 32:
		m_configLimit = data;
		return true;

	case 36:
		m_configPreFetchLimit = data;
		return true;

	case 76:
		m_configUnknown76 = data;
		return true;

	default:
		return PciDevice::WriteConfigSpace( reg, data );
	}
}

bool
AgpBridge::WriteBAR( int num, u32 &data )
{
	switch( num ) {
	default:
		if( data != 0 ) {
			return PciDevice::WriteBAR( num, data );
		}
		return true;
	}
}

