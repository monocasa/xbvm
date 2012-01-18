#ifndef XBVM_HOSTBRIDGE_H
#define XBVM_HOSTBRIDGE_H

#include "PciDevice.h"

class HostBridge : public PciDevice
{
public:
	HostBridge() : PciDevice( 0, 0, 0, PCI_NVIDIA_HOSTBRIDGE_DEVICE_ID ),
	   m_configReg100(0),
	   m_configReg108(0),
	   m_configReg128(0),
	   m_configReg132(0),
	   m_configReg135(0)
	{
	}

	virtual ~HostBridge();

	virtual const char* GetName( void ) { return "Host Bridge"; }

	virtual bool ReadConfigSpace( u8 reg, u32 &data );
	virtual bool WriteConfigSpace( u8 reg, u32 &data );
	virtual bool WriteConfigSpace8( u8 reg, u8 &data );

private:
	u32 m_configReg100;
	u32 m_configReg108;
	u32 m_configReg128;
	u32 m_configReg132;
	u8 m_configReg135;
};

extern HostBridge *g_hostBridge;

#endif /*XBVM_HOSTBRIDGE_H*/

