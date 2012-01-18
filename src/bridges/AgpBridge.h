#ifndef XBVM_AGPBRIDGE_H
#define XBVM_AGPBRIDGE_H

#include "PciDevice.h"

class AgpBridge : public PciDevice
{
public:
	AgpBridge() : PciDevice( 0, 30, 0, PCI_NVIDIA_AGP_BRIDGE_DEVICE_ID ),
	    m_configLimit( 0 ),
	    m_configTimers( 0 ),
	    m_configPreFetchLimit( 0 ),
	    m_configUnknown76( 0 )
	{
	}

	virtual ~AgpBridge();

	virtual const char* GetName( void ) { return "AGP Bridge"; }

	virtual bool WriteConfigSpace( u8 reg, u32 &data );
	virtual bool WriteBAR( int num, u32 &data );

private:
	u32 m_configLimit;
	u32 m_configTimers;
	u32 m_configPreFetchLimit;
	u32 m_configUnknown76;
};

extern AgpBridge *g_agpBridge;

#endif /*XBVM_AGPBRIDGE_H*/

