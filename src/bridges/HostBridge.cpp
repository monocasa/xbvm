#include "bridges/HostBridge.h"

HostBridge *g_hostBridge;

HostBridge::~HostBridge()
{
}

bool
HostBridge::ReadConfigSpace( u8 reg, u32 &data )
{
	switch( reg ) {
	case 100:
		data = m_configReg100;
		return true;

	case 108:
		data = m_configReg108;
		return true;

	default:
		return PciDevice::ReadConfigSpace( reg, data );
	}
}

bool
HostBridge::WriteConfigSpace( u8 reg, u32 &data )
{
	switch( reg ) {
	case 100:
		m_configReg100 = data;
		return true;

	case 108:
		m_configReg108 = data;
		return true;

	case 128:
		m_configReg128 = data;
		return true;

	case 132:
		m_configReg132 = data;
		return true;

	default:
		return PciDevice::WriteConfigSpace( reg, data );
	}
}

bool
HostBridge::WriteConfigSpace8( u8 reg, u8 &data )
{
	switch( reg ) {
	case 135:
		m_configReg135 = data;
		return true;

	default:
		return PciDevice::WriteConfigSpace8( reg, data );
	}
}
	
