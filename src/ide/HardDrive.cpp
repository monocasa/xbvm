#include <cstring>

#include "ide/HardDrive.h"
#include "Log.h"

void
HardDrive::OnIdentifyCommand()
{
	m_currentCommand = IDE_COMMAND_IDENTIFY;
	m_errorRegister = 0;

	m_status.busy = false;
	m_status.driveReady = true;
	m_status.writeFault = false;
	m_status.drq = true;
	m_status.seekComplete = true;
	m_status.correctedData = false;
	m_bufferIndex = 0;

	u16 idSector[ 256 ];
	PopulateIdSector( idSector );

	for( int i = 0; i < 256; i++ ) {
		m_buffer[ (i * 2) + 0 ] = idSector[ i ] & 0x00FF;
		m_buffer[ (i * 2) + 1 ] = idSector[ i ] >> 8;
	}

	for( int i = 0; i < 256; i+= 8 ) {
	}
	//@todo raise an interrupt
}

void
HardDrive::OnSecurityUnlockCommand()
{
	m_currentCommand = IDE_COMMAND_SECURITY_UNLOCK;
	m_errorRegister = 0;

	m_status.busy = false;
	m_status.driveReady = true;
	m_status.writeFault = false;
	m_status.drq = true;
	m_status.seekComplete = true;
	m_status.correctedData = false;
	m_bufferIndex = 0;
}

void
HardDrive::PopulateIdSector( u16 *buffer )
{
	memset( buffer, 0, IDE_DEVICE_SECTOR_SIZE );

	// Word 0:  general config bit significant info
	buffer[ 0 ] = 0x0040;

	// Word 1:  number of cylinders
	buffer[ 1 ] = 16383;

	// word 2:  reserved

	// Word 3:  number of heads
	buffer[ 3 ] = 16;

	// Word 4:  number of bytes per track
	buffer[ 4 ] = 32256;

	// Word 5:  number of bytes per sector
	buffer[ 5 ] = 512;

	// Word 6:  number of tracks
	buffer[ 6 ] = 63;

	// Words 7-9: vendor specific
	
	// Words 10-19:  Serial number
	const char *serial = "6DB2WQW2            ";
	for( int i = 0; i < 10; i++ ) {
		buffer[ 10 + i ] = (serial[ i * 2 ] << 8) | serial[ (i * 2) + 1 ];
	}

	// Word 20:  Buffer Type
	buffer[ 20 ] = 3;

	// Word 21:  Buffer Size in 512 byte increments
	buffer[ 21 ] = 1024;

	// Word 22:  # of ECC bytes available on read/write long cmds
	buffer[ 22 ] = 4;

	// Words 23-26:  Firmware Revision
	const char *firmware = "6.55    ";
	for( int i = 0; i < 4; i++ ) {
		buffer[ 23 + i ] = (firmware[ i * 2 ] << 8) | firmware[ (i * 2) + 1 ];
	}

	// Words 27-46:  Model number
	const char *model = "ST310211A                               ";
	for( int i = 0; i < 40; i++ ) {
		buffer[ 27 + i ] = (model[ i * 2 ] << 8) | model[ (i * 2) + 1 ];
	}

	// Word 47: 15-8:  vendor unique
	//           7-0:  max number of sectors that can be transferred per interrupt
	buffer[ 47 ] = 16;

	// Word 48:  Set to 1 if can perform dword IO, 0 if otherwise
	buffer[ 48 ] = 1;

	// Word 49:  Capabilities
	//           15-8: reserved
	//              9: 1 = LBA supported
	//              8: 1 = DMA supported
	//            7-0: vendor unique
	buffer[ 49 ] = (1<<9) | (1<<8);

	// Word 50:  Reserved

	// Word 51:  15-8:  PIO data transfer cycle timing mode
	//            7-0:  Vendor unique
	buffer[ 51 ] = 0x0200;

	// Word 52:  15-8:  DMA data transfer cycle timing mode
	//            7-0:  Vendor unique
	buffer[ 52 ] = 0x0200;

	// Word 53:  15-3 Reserved
	//              2 1=the fields reported in word 88 are valid
	//              1 1=the fields reported in words 64-70 are valid
	//              0 1=the fields reported in words 54-58 are valid
	buffer[ 53 ] = 0x0007;

	// Word 54:  cylinders
	buffer[ 54 ] = 16383;

	// Word 55:  heads
	buffer[ 55 ] = 16;

	// Word 56:  sectors/track
	buffer[ 56 ] = 63;

	// Word 57:  LSW current capacity in sectors
	buffer[ 57 ] = (16383*16*63) & 0xFFFF;

	// Word 58:  MSW current capacity in sectors
	buffer[ 58 ] = (16383*16*63) >> 16;

	// Word 59:  15-9 reserved
	//              8 1=multiple sector setting is valid
	//            7-0 current setting for number of sectors that can be 
	//                transferred per interrupt on R/W multiple commands
	buffer[ 59 ] = 0x0000;

	// Words 60-61:  Total number of LBA sectors
	int numSectors = 19541088;
	buffer[ 60 ] = numSectors & 0xFFFF;
	buffer[ 61 ] = numSectors >> 16;

	// Word 62:  15-8 single word DMA transfer mode active
	//            7-0 single word DMA transfer mode supported
	buffer[ 62 ] = 0x0000;

	// Word 63:  15-8 multiword DMA transfer mode active
	//            7-0 multiword DMA transfer mode supported
	buffer[ 63 ] = 0x0;

	// Word 64:  PIO modes supported
	buffer[ 64 ] = 0x0000;

	// Word 65-68:  PIO/DMA cycle time (nanoseconds)
	for( int i = 65; i <= 68; i++ ) {
		buffer[ i ] = 120;
	}

	// Words 69-79:  Reserved

	// Word 80:  15-7 reserved
	//              6 supports ATA/ATAPI-6
	//              5 supports ATA/ATAPI-5
	//              4 supports ATA/ATAPI-4
	//              3 supports ATA-3
	//              2 supports ATA-2
	//              1 supports ATA-1
	//              0 reserved
	buffer[ 80 ] = 0x007E;

	// Word 81:  Minor Revision Number
	buffer[ 81 ] = 0x0000;

	// Word 82:  15 obsolete
	//           14 NOP supported
	//           13 READ BUFFER command supported
	//           12 WRITE BUFFER command supported
	//           11 obsolete
	//           10 Host protected area feature set supported
	//            9 DEVICE RESET command supported
	//            8 SERVICE interrupt supported
	//            7 RELEASE interrupt supported
	//            6 look-ahead supported
	//            5 write cache supported
	//            4 PACKET command feature set supported
	//            3 supports power management feature set
	//            2 supports removable media feature set
	//            1 supports securite mode feature set
	//            0 SMART feature set supported
	buffer[ 82 ] = (1 << 14);

	buffer[ 83 ] = (1 << 14) | (1 << 13 ) | (1 << 12) | (1 << 10);
	buffer[ 84 ] = (1 << 14);
	buffer[ 85 ] = (1 << 14);
	buffer[ 86 ] = (1 << 14) | (1 << 13 ) | (1 << 12) | (1 << 10);
	buffer[ 87 ] = (1 << 14);

	buffer[ 88 ] = 0x0000;

	buffer[ 93 ] = (1 << 14) | (1 << 13) | (1 << 0);

	// Words 100-103:  48-bit total number of sectors
	buffer[ 100 ] = numSectors & 0xffff;
	buffer[ 101 ] = (numSectors >> 16) & 0xffff;
	buffer[ 102 ] = 0;
	buffer[ 103 ] = 0;

	// Word 128:  Security Settings
	buffer[ 128 ] = (1 << 2);
}

