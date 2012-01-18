#ifndef XBVM_IDE_IDEDEVICE_H
#define XBVM_IDE_IDEDEVICE_H

#include "ide/DiskImage.h"
#include "Device.h"

#define IDE_DEVICE_SECTOR_SIZE          (512)
#define IDE_DEVICE_MAX_MULTIPLE_SECTORS (16)

#define IDE_COMMAND_ATAPI_SOFT_RESET          (0x08)
#define IDE_COMMAND_READ_SECTORS_W_RETRIES    (0x20)
#define IDE_COMMAND_READ_SECTORS_WO_RETRIES   (0x21)
#define IDE_COMMAND_READ_SECTORS_EXT          (0x24)
#define IDE_COMMAND_READ_MULTIPLE_SECTORS_EXT (0x29)
#define IDE_COMMAND_PACKET_IDENTIFY           (0xA1)
#define IDE_COMMAND_READ_MULTIPLE_SECTORS     (0xC4)
#define IDE_COMMAND_IDENTIFY                  (0xEC)
#define IDE_COMMAND_SECURITY_UNLOCK           (0xF2)

class DiskImage;

class IdeDevice : public Device 
{
public:
	IdeDevice() :
	   m_status(),
	   m_errorRegister( 0x01 ), //diagnostic code:  no error
	   m_headNum( 0 ),
	   m_sectorCount( 1 ),
	   m_sectorNum( 1 ),
	   m_cylinderNum( 0 ),
	   m_bufferSize( 0 ),
	   m_bufferIndex( 0 ),
	   m_drqIndex( 0 ),
	   m_currentCommand( 0x00 ),
	   m_multipleSectors( 0 ),
	   m_lbaMode( 0 ),
	   m_packetDma( false ),
	   m_mdmaMode( 0 ),
	   m_udmaMode( 0 ),
	   m_control(),
	   m_resetInProgress( 0 ),
	   m_features( 0 ),
	   m_hob(),
	   m_numSectors( 0 ),
	   m_lba48( false ),
	   m_diskImage( NULL ),
	   m_sectorSize( 0 )
	{
		m_status.busy            = false;
		m_status.driveReady      = true;
		m_status.writeFault      = false;
		m_status.seekComplete    = true;
		m_status.drq             = false;
		m_status.correctedData   = false;
		m_status.indexPulse      = false;
		m_status.indexPulseCount = 0;
		m_status.err             = false;

		m_control.reset      = false;
		m_control.disableIrq = false;

		m_hob.feature = 0;
		m_hob.nsector = 0;
		m_hob.sector  = 0;
		m_hob.lcyl    = 0;
		m_hob.hcyl    = 0;
	}

	virtual ~IdeDevice()
	{ }

	bool ReadData( u16 &data );
	bool WriteData( u16 &data );

	bool WriteCommand( u8 &data );
	void WriteCylinderHigh( u8 &data );
	void WriteCylinderLow( u8 &data );
	void WriteFeatures( u8 &data );
	void WriteHeadNum( u8 headNum );
	void WriteLbaMode( u8 lbaMode );
	void WriteSectorCount( u8 &data );
	void WriteSectorNum( u8 &data );

	u8 ReadStatus();

protected:
	struct {
		bool busy;
		bool driveReady;
		bool writeFault;
		bool seekComplete;
		bool drq;
		bool correctedData;
		bool indexPulse;
		int  indexPulseCount;
		bool err;
	}    m_status;

	u8   m_errorRegister;
	u8   m_headNum;

	union {
		u8 m_sectorCount;
		struct {
			//@todo Only little endian
			u8 c_d : 1;
			u8 i_o : 1;
			u8 rel : 1;
			u8 tag : 5;
		} m_interruptReason;
	};

	u8   m_sectorNum;

	union {
		u16 m_cylinderNum;
		u16 m_byteCount;
	};

	u8   m_buffer[ (IDE_DEVICE_MAX_MULTIPLE_SECTORS * IDE_DEVICE_SECTOR_SIZE) + 4 ];
	u32  m_bufferSize;
	u32  m_bufferIndex;
	u32  m_drqIndex;
	u8   m_currentCommand;
	u8   m_multipleSectors;
	u8   m_lbaMode;
	bool m_packetDma;
	u8   m_mdmaMode;
	u8   m_udmaMode;

	struct {
		bool reset;
		bool disableIrq;
	}    m_control;

	u8   m_resetInProgress;
	u8   m_features;

	struct {
		u8 feature;
		u8 nsector;
		u8 sector;
		u8 lcyl;
		u8 hcyl;
	}    m_hob;

	u32  m_numSectors;
	bool m_lba48;

	DiskImage* m_diskImage;

	int m_sectorSize;

	void AbortCommand();
	bool IncrementAddresses( u64 sector );
	bool PerformRead( u8 command );

	virtual void OnAtapiSoftReset() { AbortCommand(); }
	virtual void OnIdentifyCommand() { AbortCommand(); }
	virtual void OnIdentifyPacketDevice() { AbortCommand(); }
	virtual void OnSecurityUnlockCommand() { AbortCommand(); }
};

#endif /*XBVM_IDE_IDEDEVICE_H*/

