#ifndef XBVM_SMBUSCONTROLLER_H
#define XBVM_SMBUSCONTROLLER_H

#include "PciDevice.h"

#define SMBUS_PORT_STATUS  (0x00)
#define SMBUS_PORT_CONTROL (0x02)
#define SMBUS_PORT_ADDRESS (0x04)
#define SMBUS_PORT_DATA    (0x06)
#define SMBUS_PORT_COMMAND (0x08)

class SmBusDevice;

class SmBusController : public PciDevice 
{
public:
	SmBusController( void );

	virtual ~SmBusController( void );

	virtual bool IoRead8( u16 port, u8 &data );
	virtual bool IoRead16( u16 port, u16 &data );
	virtual bool IoWrite8( u16 port, u8 &data );
	virtual bool IoWrite16( u16 port, u16 &data );

	virtual const char* GetName( void ) { return "SMBus Controller"; }

	virtual bool WriteBAR( int num, u32 &data );

private:
	u16 m_bar1Base;
	u16 m_bar2Base;

	u8 m_portc200;

	u8  m_address;
	u8  m_command;
	u16 m_data;
	u8  m_status;

	bool WriteControl( u8 control );

	SmBusDevice* GetDeviceForAddr( u8 addr );
};

extern SmBusController *g_smBus;

#endif /*XBVM_SMBUSCONTROLLER*/

