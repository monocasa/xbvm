#ifndef XBVM_PCIDEVICE_H
#define XBVM_PCIDEVICE_H

#include "HostDevice.h"
#include "Types.h"

#define PCI_NVIDIA_VENDOR_ID (0x10DE)

#define PCI_NVIDIA_AUDIO_APU_DEVICE_ID  (0x01B0)
#define PCI_NVIDIA_AUDIO_AC97_DEVICE_ID (0x01B1)
#define PCI_NVIDIA_ISA_BRIDGE_DEVICE_ID (0x01B2)
#define PCI_NVIDIA_SMBUS_DEVICE_ID      (0x01B4)
#define PCI_NVIDIA_AGP_BRIDGE_DEVICE_ID (0x01B7)
#define PCI_NVIDIA_PCI_BRIDGE_DEVICE_ID (0x01B8)
#define PCI_NVIDIA_IDE_DEVICE_ID        (0x01BC)
#define PCI_NVIDIA_MODEM_DEVICE_ID      (0x01C1)
#define PCI_NVIDIA_OHCI_DEVICE_ID       (0x01C2)
#define PCI_NVIDIA_NETWORK_DEVICE_ID    (0x01C3)
#define PCI_NVIDIA_NV2A_DEVICE_ID       (0x02A0)
#define PCI_NVIDIA_HOSTBRIDGE_DEVICE_ID (0x02A5)
#define PCI_NVIDIA_DRAM_DEVICE_ID       (0x02A6)

#define PCI_CONFIG_REG_IDS (0)
#define PCI_CONFIG_REG_COMMAND (4)
#define PCI_CONFIG_REG_HEADER_TYPE (14)
#define PCI_CONFIG_REG_BAR0 (16)
#define PCI_CONFIG_REG_BAR1 (20)
#define PCI_CONFIG_REG_BAR2 (24)
#define PCI_CONFIG_REG_BAR3 (28)
#define PCI_CONFIG_REG_BAR4 (32)
#define PCI_CONFIG_REG_BAR5 (36)

#define PCI_CONFIG_REG_HEADER_TYPE_NORMAL  (0)
#define PCI_CONFIG_REG_HEADER_TYPE_BRIDGE  (1)
#define PCI_CONFIG_REG_HEADER_TYPE_CARDBUS (2)

class PciDevice : public HostDevice
{
public:
	PciDevice( int bus, int dev, int func, u16 deviceId ) : 
	   m_bus(bus),
	   m_dev(dev),
	   m_func(func),
	   m_deviceId( deviceId ),
	   m_vendorId( PCI_NVIDIA_VENDOR_ID )
	{
		m_bar[0] = 0;
		m_bar[1] = 0;
		m_bar[2] = 0;
		m_bar[3] = 0;
		m_bar[4] = 0;
		m_bar[5] = 0;
	}
	virtual ~PciDevice();

	virtual bool WriteConfigSpace( u8 reg, u32 &data );
	virtual bool WriteConfigSpace8( u8 reg, u8 &data );
	virtual bool ReadConfigSpace( u8 reg, u32 &data );
	virtual bool ReadConfigSpace8( u8 reg, u8 &data );

	virtual bool WriteBAR( int num, u32 &data );

	int GetBus() { return m_bus; }
	int GetDev() { return m_dev; }
	int GetFunc() { return m_func; }

	virtual u8 GetClassCode() { return 0; }
	virtual u8 GetSubClass() { return 0; }
	virtual u8 GetProg1F() { return 0; }
	virtual u8 GetHeaderType() { return PCI_CONFIG_REG_HEADER_TYPE_NORMAL; }

private:
	int m_bus;
	int m_dev;
	int m_func;
	u16 m_deviceId;
	u16 m_vendorId;

protected:
	u32 m_bar[6];
};

#endif /*XBVM_PCIDEVICE_H*/

