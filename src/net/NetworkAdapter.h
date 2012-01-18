#ifndef XBVM_NET_NETWORKADAPTER_H
#define XBVM_NET_NETWORKADAPTER_H

#include "PciDevice.h"

class NetworkAdapter : public PciDevice
{
public:
	NetworkAdapter();
	virtual ~NetworkAdapter();

	virtual const char* GetName( void ) { return "Network Adapter"; }

	virtual bool MmioWrite8( u32 reg, u8 data );

	virtual bool WriteBAR( int num, u32 &data );

private:
	u32 m_mmiobase;
	u32 m_ioportbase;

	u8 m_macAddress[6];
};

extern NetworkAdapter *g_net;

#endif /*XBVM_NET_NETWORKADAPTER_H*/

