#ifndef XBVM_PCIBUS_H
#define XBVM_PCIBUS_H

#include "Log.h"
#include "HostDevice.h"
#include "PciDevice.h"
#include "Types.h"

#define PCI_BUS_CONFIG_ADDRESS_PORT (0xCF8)
#define PCI_BUS_CONFIG_DATA_PORT    (0xCFC)

class PciBus : public HostDevice
{
public:
	PciBus();
	virtual ~PciBus();

	virtual bool IoRead32( u16 port, u32 &data )
	{
		switch( port ) {
		case PCI_BUS_CONFIG_DATA_PORT:
			return ReadConfigData( data );

		default:
			return HostDevice::IoRead32( port, data );
		}
	}

	virtual bool IoWrite32( u16 port, u32 &data )
	{
		switch( port ) {
		case PCI_BUS_CONFIG_ADDRESS_PORT:
			return WriteConfigAddress( data );

		case PCI_BUS_CONFIG_DATA_PORT:
			return WriteConfigData( data );

		default:
			return HostDevice::IoWrite32( port, data );
		}
	}

	virtual bool IoRead8( u16 port, u8 &data )
	{
		int bus = ((m_configAddress >> 16) & 0xFF);
		int dev = ((m_configAddress >> 11) & 0x1F);
		int func = ((m_configAddress >> 8) & 0x7);
		int reg = (m_configAddress & 0xFF) + (port - PCI_BUS_CONFIG_DATA_PORT);
		int offset = (dev << 3) | func;

		switch( port ) {
		case PCI_BUS_CONFIG_DATA_PORT:
		case PCI_BUS_CONFIG_DATA_PORT+1:
		case PCI_BUS_CONFIG_DATA_PORT+2:
		case PCI_BUS_CONFIG_DATA_PORT+3:
			if( 0 == bus ) {
				if( NULL == m_bus0Devices[offset] ) {
					EPRINT( "Read %d:%d.%d[%d]", bus, dev, func, reg );
					return false;
				}
				return m_bus0Devices[offset]->ReadConfigSpace8( reg, data );
			}
			else if( 1 == bus ) {
				if( NULL == m_bus1Devices[offset] ) {
					EPRINT( "Read %d:%d.%d[%d]", bus, dev, func, reg );
					return false;
				}
				return m_bus1Devices[offset]->ReadConfigSpace8( reg, data );
			}
			else {
				EPRINT( "Unknown read from %d:%d.%d[%d]", 
				        bus, dev, func, reg );
				return false;
			}

		default:
			return HostDevice::IoWrite8( port, data );
		}
	}

		

	virtual bool IoWrite8( u16 port, u8 &data )
	{
		int bus = ((m_configAddress >> 16) & 0xFF);
		int dev = ((m_configAddress >> 11) & 0x1F);
		int func = ((m_configAddress >> 8) & 0x7);
		int reg = (m_configAddress & 0xFF) + (port - PCI_BUS_CONFIG_DATA_PORT);
		int offset = (dev << 3) | func;

		DPRINT( "Write 8 %d:%d.%d[%d] = %02x", bus,dev,func,reg,data );

		switch( port ) {
		case PCI_BUS_CONFIG_ADDRESS_PORT+1:
			m_configAddress = (m_configAddress & 0xFFFF00FF) + (((u32)data)<<8 );
			return true;
 
		case PCI_BUS_CONFIG_DATA_PORT:
		case PCI_BUS_CONFIG_DATA_PORT+1:
		case PCI_BUS_CONFIG_DATA_PORT+2:
		case PCI_BUS_CONFIG_DATA_PORT+3:
			if( 0 == bus ) {
				if( NULL == m_bus0Devices[offset] ) {
					EPRINT( "Read %d:%d.%d[%d]", bus, dev, func, reg );
					return false;
				}
				return m_bus0Devices[offset]->WriteConfigSpace8( reg, data );
			}
			else if( 1 == bus ) {
				if( NULL == m_bus1Devices[offset] ) {
					EPRINT( "Read %d:%d.%d[%d]", bus, dev, func, reg );
					return false;
				}
				return m_bus1Devices[offset]->WriteConfigSpace8( reg, data );
			}
			else {
				EPRINT( "Unknown read from %d:%d.%d[%d]", 
				        bus, dev, func, reg );
				return false;
			}

		default:
			EPRINT( "Yo" );
			return HostDevice::IoWrite8( port, data );
		}
	}

	bool
	ReadConfigData( u32 &data )
	{
		int bus = ((m_configAddress >> 16) & 0xFF);
		int dev = ((m_configAddress >> 11) & 0x1F);
		int func = ((m_configAddress >> 8) & 0x7);
		int reg = m_configAddress & 0xFF;
		int offset = (dev << 3) | func;

		if( 0 == bus ) {
			if( NULL == m_bus0Devices[offset] ) {
				EPRINT( "Read %d:%d.%d[%d]", bus, dev, func, reg );
				return false;
			}
			return m_bus0Devices[offset]->ReadConfigSpace( reg, data );
		}
		else if( 1 == bus ) {
			if( NULL == m_bus1Devices[offset] ) {
				EPRINT( "Read %d:%d.%d[%d]", bus, dev, func, reg );
				return false;
			}
			return m_bus1Devices[offset]->ReadConfigSpace( reg, data );
		}
		else {
			EPRINT( "Unknonw read from %d:%d.%d[%d]", 
			        bus, dev, func, reg );
			return false;
		}
	}
		
	bool
	WriteConfigData( u32 data )
	{
		int bus = ((m_configAddress >> 16) & 0xFF);
		int dev = ((m_configAddress >> 11) & 0x1F);
		int func = ((m_configAddress >> 8) & 0x7);
		int reg = m_configAddress & 0xFF;
		int offset = (dev << 3) | func;

		if( 0 == bus ) {
			if( NULL == m_bus0Devices[offset] ) {
				EPRINT( "Write %d:%d.%d[%d] = %Xh", bus, dev, func, reg, data );
				return false;
			}
			return m_bus0Devices[offset]->WriteConfigSpace( reg, data );
		}
		else if( 1 == bus ) {
			if( NULL == m_bus1Devices[offset] ) {
				EPRINT( "Write %d:%d.%d[%d] = %Xh", bus, dev, func, reg, data );
				return false;
			}
			return m_bus1Devices[offset]->WriteConfigSpace( reg, data );
		}
		else {
			EPRINT( "%d:%d.%d[%d] = %Xh", 
			        bus, dev, func, reg, data );
			return false;
		}
	}

	bool
	WriteConfigAddress( u32 addr )
	{
		m_configAddress = addr;
		return true;
	}

	bool
	AddPciDevice( PciDevice *device )
	{
		DPRINT( "adding %d:%d.%d  %p", device->GetBus(), device->GetDev(), device->GetFunc(), device );
		int bus = device->GetBus();
		int offset = (device->GetDev() << 3) | device->GetFunc();

		if( 0 == bus ) {
			m_bus0Devices[offset] = device;
		}
		else if( 1 == bus ) {
			m_bus1Devices[offset] = device;
		}
		else {
			EPRINT( "Unknown bus %d for PciDevice %p", bus, device );
			return false;
		}
		return true;
	}

	virtual
	const char*
	GetName( void )
	{
		return "PCI Bridge";
	}

private:
	u32 m_configAddress;

	PciBus( PciBus& );
	const PciBus& operator=( PciBus& );

	PciDevice* m_bus0Devices[256];
	PciDevice* m_bus1Devices[256];
};

extern PciBus *g_pciBus;

#endif /*XBVM_PCIBUS_H*/

