#ifndef XBVM_OHCICONTROLLER_H
#define XBVM_OHCICONTROLLER_H

#include <cstdio>

#include "PciDevice.h"

#define OHCI_REG_REVISION         (0x0000)
#define OHCI_REG_CONTROL          (0x0004)
#define OHCI_REG_COMMANDSTATUS    (0x0008)
#define OHCI_REG_INTERRUPTSTATUS  (0x000C)
#define OHCI_REG_INTERRUPTENABLE  (0x0010)
#define OHCI_REG_INTERRUPTDISABLE (0x0014)
#define OHCI_REG_HCCA             (0x0018)
#define OHCI_REG_PERIODCURRENTED  (0x001C)
#define OHCI_REG_CONTROLCURRENTED (0x0020)
#define OHCI_REG_CONTROLHEADED    (0x0024)
#define OHCI_REG_BULKHEADED       (0x0028)
#define OHCI_REG_BULKCURRENTED    (0x002C)
#define OHCI_REG_DONEHEAD         (0x0030)
#define OHCI_REG_FMINTERVAL       (0x0034)
#define OHCI_REG_FMREMAINING      (0x0038)
#define OHCI_REG_FMNUMBER         (0x003C)

class OhciController : public PciDevice
{
public:
	OhciController( int deviceNum ) : PciDevice( 0, deviceNum, 0, PCI_NVIDIA_OHCI_DEVICE_ID ),
	   m_mmiobase( 0 ),
	   m_config60( 0 ),
	   m_interruptEnabled( false )
	{
		snprintf( m_name, 32, "OHCI Controller %d:%d.%d", GetBus(), GetDev(), GetFunc() );
	}

	virtual ~OhciController() {}

	virtual const char* GetName() { return m_name; }

	virtual u32 MmioRead32( u32 addr );

	virtual bool MmioWrite32( u32 addr, u32 data );

	virtual bool WriteBAR( int num, u32 &data );

	virtual bool WriteConfigSpace( u8 reg, u32 &data );
	virtual bool ReadConfigSpace( u8 reg, u32 &data );

private:
	char m_name[32];
	u32 m_mmiobase;
	u32 m_config60;

	bool m_interruptEnabled;
};

extern OhciController *g_usb0;
extern OhciController *g_usb1;

#endif /*XBVM_OHCICONTROLLER_H*/

