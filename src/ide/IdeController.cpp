#include "ide/DvdDrive.h"
#include "ide/HardDrive.h"
#include "ide/IdeController.h"
#include "Log.h"
#include "Mem.h"

IdeController *g_ide;

IdeController::IdeController() : PciDevice( 0, 9, 0, PCI_NVIDIA_IDE_DEVICE_ID ),
   m_iobase( 0 ),
   m_features( 0 ),
   m_sectorCount( 0 ),
   m_lbaLow( 0 ),
   m_lbaMid( 0 ),
   m_lbaHigh( 0 ),
   m_status( 0x0 ),
   m_dataOffset( 0 ),
   m_currentDrive( 0 )
{
	g_mem->AddDeviceToIoMap( this, 0x1f0 );
	g_mem->AddDeviceToIoMap( this, 0x1f1 );
	g_mem->AddDeviceToIoMap( this, 0x1f2 );
	g_mem->AddDeviceToIoMap( this, 0x1f3 );
	g_mem->AddDeviceToIoMap( this, 0x1f4 );
	g_mem->AddDeviceToIoMap( this, 0x1f5 );
	g_mem->AddDeviceToIoMap( this, 0x1f6 );
	g_mem->AddDeviceToIoMap( this, 0x1f7 );
	g_mem->AddDeviceToIoMap( this, 0x3f6 );

	m_drives[ 0 ] = new HardDrive();
	m_drives[ 1 ] = new DvdDrive();
}

IdeController::~IdeController()
{
}

bool
IdeController::IoRead16( u16 port, u16 &data )
{
	switch( port ) {
	case IDE_PORT_DATA:
		if( CurrentDrive() == NULL ) {
			data = 0xFFFF;
			return true;
		}
		return CurrentDrive()->ReadData( data );
		
	default:
		return PciDevice::IoRead16( port, data );
	}
}

bool
IdeController::IoRead8( u16 port, u8 &data )
{
	switch( port ) {
	case IDE_PORT_ALT_STATUS:
		if( CurrentDrive() == NULL ) {
			data = 0;
			return true;
		}

		data = CurrentDrive()->ReadStatus();

		return true;

	case IDE_PORT_STATUS:
		if( CurrentDrive() == NULL ) {
			data = 0;
			return true;
		}

		data = CurrentDrive()->ReadStatus();
		return true;

	default:
		break;
	}
//
//	if( port >= m_iobase && port < m_iobase + 6 ) {
//		switch( port - m_iobase ) {
//		default:
//			return PciDevice::IoRead8( port, data );
//		}
//	}
//
	return PciDevice::IoRead8( port, data );
}

bool
IdeController::IoWrite16( u16 port, u16 &data )
{
	switch( port ) {
	case IDE_PORT_DATA:
		if( CurrentDrive() == NULL ) {
			return true;
		}
		return CurrentDrive()->WriteData( data );

	default:
		return PciDevice::IoWrite16( port, data );
	}
}

bool
IdeController::IoWrite8( u16 port, u8 &data )
{
	switch( port ) {
	case IDE_PORT_ALT_STATUS:
		DPRINT( "IDE DevCntrl %02x", data );
		return true;

	case IDE_PORT_COMMAND:
		if( CurrentDrive() == NULL ) {
			return true;
		}
		return CurrentDrive()->WriteCommand( data );

	case IDE_PORT_CYLINDER_HIGH:
		for( int i = 0; i < IDE_NUM_DRIVES; i++ ) {
			if( m_drives[ i ] == NULL ) {
				continue;
			}
			m_drives[ i ]->WriteCylinderHigh( data );
		}
		return true;

	case IDE_PORT_CYLINDER_LOW:
		for( int i = 0; i < IDE_NUM_DRIVES; i++ ) {
			if( m_drives[ i ] == NULL ) {
				continue;
			}
			m_drives[ i ]->WriteCylinderLow( data );
		}
		return true;

	case IDE_PORT_DRIVE_AND_HEAD:
		if( (data & IDE_DRIVE_SELECT_RESERVED) != IDE_DRIVE_SELECT_RESERVED ) {
			EPRINT( "IO Write 0x%04x (%02x): not 1x1xxxxxb", IDE_PORT_DRIVE_AND_HEAD, data );
			return false;
		}
		if( m_currentDrive != ((data >> 4) & 0x01) ) {
			DPRINT( "Switching drive to %d", ((data >> 4) & 0x01) );
		}
		m_currentDrive = ( (data >> 4) & 0x01 );
		for( int i = 0; i < IDE_NUM_DRIVES; i++ ) {
			if( m_drives[ i ] == NULL ) {
				continue;
			}
			m_drives[ i ]->WriteLbaMode( (data >> 6) & 0x01 );
			m_drives[ i ]->WriteHeadNum( data & 0x0F );
		}
		return true;

	case IDE_PORT_FEATURES:
		for( int i = 0; i < IDE_NUM_DRIVES; i++ ) {
			if( m_drives[ i ] == NULL ) {
				continue;
			}
			m_drives[ i ]->WriteFeatures( data );
		}
		return true;

	case IDE_PORT_SECTOR_COUNT:
		for( int i = 0; i < IDE_NUM_DRIVES; i++ ) {
			if( m_drives[ i ] == NULL ) {
				continue;
			}
			m_drives[ i ]->WriteSectorCount( data );
		}
		return true;

	case IDE_PORT_SECTOR_NUM:
		for( int i = 0; i < IDE_NUM_DRIVES; i++ ) {
			if( m_drives[ i ] == NULL ) {
				continue;
			}
			m_drives[ i ]->WriteSectorNum( data );
		}
		return true;

	default:
		break;
	}

	if( port >= m_iobase && port < m_iobase + 6 ) {
		switch( port - m_iobase ) {
		case 0:  // this is labeled as "stop bus mastering" by cromwell (writes 0x00)
		case 2:  // this is labeled as "DMA possible for both drives" by cromwell (writes 0x62)
			return true;

		default:
			return PciDevice::IoWrite8( port, data );
		}
	}

	return PciDevice::IoWrite8( port, data );
}

bool
IdeController::WriteBAR( int num, u32 &data )
{
	switch( num ) {
	case 0:
	case 1:
	case 2:
	case 3:
	case 5:
		if( data != 0 ) {
			return PciDevice::WriteBAR( num, data );
		}
		return true;

	case 4:
		if( (data & 1) != 1 ) {
			EPRINT( "Trying to map IDE to non io space %08x", data );
			return false;
		}
		m_iobase = data & 0xFFFE;
		DPRINT( "Assigning IDE to %x", data );
		for( int i = 0; i < 8; i++ ) {
			g_mem->AddDeviceToIoMap( this, m_iobase + i );
		}
		return true;

	default:
		return PciDevice::WriteBAR( num, data );
	}
}

