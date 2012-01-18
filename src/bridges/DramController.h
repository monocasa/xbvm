#ifndef XBVM_BRIDGES_H
#define XBVM_BRIDGES_H

#include "PciDevice.h"

class DramController : public PciDevice
{
public:
	DramController() : PciDevice( 0, 0, 3, PCI_NVIDIA_DRAM_DEVICE_ID ),
	   m_reg64( 0 ),
	   m_reg68( 0 ),
	   m_reg92( 0 ),
	   m_reg108( 0 )
	{
	}
	virtual ~DramController() {}

	virtual bool WriteConfigSpace( u8 reg, u32 &data );

	virtual const char* GetName( void ) { return "DRAM Controller"; }
private:
	u32 m_reg64;
	u32 m_reg68;
	u32 m_reg92;
	u32 m_reg108;
};

extern DramController* g_dramController;

#endif /*XBVM_BRIDGES_H*/

