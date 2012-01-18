#ifndef XBVM_IDE_IDECONTROLLER_H
#define XBVM_IDE_IDECONTROLLER_H

#include "PciDevice.h"

#define IDE_PORT_DATA           (0x1F0)
#define IDE_PORT_FEATURES       (0x1F1)
#define IDE_PORT_SECTOR_COUNT   (0x1F2)
#define IDE_PORT_SECTOR_NUM     (0x1F3)
#define IDE_PORT_CYLINDER_LOW   (0x1F4)
#define IDE_PORT_CYLINDER_HIGH  (0x1F5)
#define IDE_PORT_DRIVE_AND_HEAD (0x1F6)
#define IDE_PORT_COMMAND        (0x1F7)
#define IDE_PORT_STATUS         (0x1F7)

#define IDE_PORT_DEVICE_CONTROL (0x3F6)
#define IDE_PORT_ALT_STATUS     (0x3F6)

#define IDE_DRIVE_SELECT_RESERVED (0xA0) // 1010 0000

#define IDE_NUM_DRIVES          (2)

class IdeDevice;

class IdeController : public PciDevice
{
public:
	IdeController();
	virtual ~IdeController();

	virtual bool IoRead16( u16 port, u16 &data );
	virtual bool IoRead8( u16 port, u8 &data );

	virtual bool IoWrite16( u16 port, u16 &data );
	virtual bool IoWrite8( u16 port, u8 &data );

	virtual bool WriteBAR( int num, u32 &data );

	virtual const char* GetName( void ) { return "IDE Controller"; }
private:
	IdeController( const IdeController& );
	const IdeController& operator=(IdeController&);

	u16 m_iobase;

	u8 m_features;
	u8 m_sectorCount;
	u8 m_lbaLow;
	u8 m_lbaMid;
	u8 m_lbaHigh;
	u8 m_status;
	u16 m_dataBuffer[256];
	int m_dataOffset;
	u16 m_idDrive[256];

	int m_currentDrive;

	IdeDevice* m_drives[ IDE_NUM_DRIVES ];

	IdeDevice* CurrentDrive()
	{
		return m_drives[ m_currentDrive ];
	}
};

extern IdeController *g_ide;

#endif /*XBVM_IDE_IDECONTROLLER_H*/

