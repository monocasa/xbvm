#ifndef XBVM_ISABRIDGE_H
#define XBVM_ISABRIDGE_H

#include "PciDevice.h"

class IsaBridge : public PciDevice
{
public:
	IsaBridge( void );
	virtual ~IsaBridge( void );

	virtual bool ReadConfigSpace8( u8 reg, u8 &data );
	virtual bool ReadConfigSpace( u8 reg, u32 &data );
	virtual bool WriteConfigSpace8( u8 reg, u8 &data );
	virtual bool WriteConfigSpace( u8 reg, u32 &data );

	virtual bool WriteBAR( int num, u32 &data ); 

	virtual
	const char*
	GetName( void )
	{
		return "ISA Bridge";
	}
	virtual bool IoRead32( u16 port, u32 &data );
	virtual bool IoWrite32( u16 port, u32 &data );
	virtual bool IoRead16( u16 port, u16 &data );
	virtual bool IoWrite16( u16 port, u16 &data );
	virtual bool IoRead8( u16 port, u8 &data );
	virtual bool IoWrite8( u16 port, u8 &data );

private:
	u16 m_ioBase;
	int m_hardwareMonitorPinPulses;

	u32 m_configReg76;
	u32 m_configReg84;
	u32 m_configReg96;
	u8  m_configReg100;
	u8  m_configReg106;
	u32 m_configReg108;
	u8  m_configReg129;
	u32 m_configReg132;
	u32 m_configReg140;
	u32 m_configReg156;
	u32 m_configReg180;

	u32 m_timerReg;
	u32 m_portB4;

	u16 m_port02;
	u16 m_port04;
	u16 m_port20;
	u16 m_port22;
	u16 m_port23;
	u16 m_port28;

	u8 m_port26;
	u8 m_port49;
	u8 m_portC0;
	u8 m_portCC;
	u8 m_portCD;
	u8 m_portCE;
	u8 m_portCF;
	u8 m_portD3;
	u8 m_portD6;
	u8 m_portD8;
	u8 m_portD9;
};

extern IsaBridge *g_isaBridge;

#endif /*XBVM_ISABRIDGE_H*/

