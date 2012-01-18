#include "ide/IdeController.h"
#include "ide/IdeDevice.h"
#include "Log.h"

#define INDEX_PULSE_CYCLE (10)

bool
IdeDevice::ReadData( u16 &data )
{
	if( m_status.drq == false ) {
		EPRINT( "IO read(0x%04x) with drq == 0; last command was 0x%02x %d %d %d",
				IDE_PORT_DATA, m_currentCommand, m_numSectors, m_bufferIndex, m_bufferSize );
		return false;
	}

	switch( m_currentCommand ) {
	case IDE_COMMAND_IDENTIFY:
		m_status.busy = false;
		m_status.driveReady = true;
		m_status.writeFault = false;
		m_status.seekComplete = true;
		m_status.correctedData = false;
		m_status.err = false;

		data = (m_buffer[ m_bufferIndex ]) | (m_buffer[ m_bufferIndex+1 ] << 8);
		m_bufferIndex += 2;
		if( m_bufferIndex >= IDE_DEVICE_SECTOR_SIZE ) {
			m_status.drq = false;
		}
		return true;

	case IDE_COMMAND_READ_SECTORS_EXT:          // 0x24
	case IDE_COMMAND_READ_SECTORS_W_RETRIES:    // 0x20
	case IDE_COMMAND_READ_SECTORS_WO_RETRIES:   // 0x21
		data = m_buffer[ m_bufferIndex++ ];
		data |= m_buffer[ m_bufferIndex++ ] << 8;

		//DPRINT( "%04x:  %04x", m_bufferIndex - 2, data );

		if( m_bufferIndex >= m_bufferSize ) {
			m_status.busy = false;
			m_status.driveReady = true;
			m_status.writeFault = false;
			m_status.seekComplete = true;
			m_status.correctedData = false;
			m_status.err = false;

			if( m_numSectors == 0 ) {
				m_status.drq = false;
			}
			else {
				m_status.drq = true;
				m_status.seekComplete = true;

				EPRINT( "More sectors %d %d", m_numSectors, m_bufferIndex );
				return false;
			}
		}
		return true;

	default:
		EPRINT( "Unknown command in read %02x on %s", m_currentCommand, GetName() );
		return false;
	}
}

bool
IdeDevice::WriteData( u16 &data )
{
	// check bochs

	switch( m_currentCommand ) {
	case IDE_COMMAND_SECURITY_UNLOCK:
		m_status.busy = false;
		m_status.driveReady = true;
		m_status.writeFault = false;
		m_status.seekComplete = true;
		m_status.correctedData = false;
		m_status.err = false;

		return true;

	default:
		EPRINT( "Unknown command in write %02x %04x on %s", m_currentCommand, data, GetName() );
		return false;
	}
}

bool
IdeDevice::WriteCommand( u8 &data )
{
	int command;

	//@todo lower irql on command writes

	//@todo error on write during busy bit set

	// not really sure why we do this, but bochs does it
	if( (data & 0xF0) == 0x10 ) {
		command = 0x10;
	}
	else {
		command = data;
	}

	DPRINT( "Command 0x%02x", command );

	switch( command ) {
	case IDE_COMMAND_ATAPI_SOFT_RESET: // 0x08
		OnAtapiSoftReset();
		return true;

	case IDE_COMMAND_READ_SECTORS_EXT:          // 0x24
	case IDE_COMMAND_READ_MULTIPLE_SECTORS_EXT: // 0x29
		m_lba48 = true;
		// fallthrough
	case IDE_COMMAND_READ_SECTORS_W_RETRIES:  // 0x20
	case IDE_COMMAND_READ_SECTORS_WO_RETRIES: // 0x21
	case IDE_COMMAND_READ_MULTIPLE_SECTORS:   // 0xC4
		return PerformRead( command );

	case IDE_COMMAND_IDENTIFY: // 0xEC
		//@todo abort command if device not present
		OnIdentifyCommand();
		return true;

	case IDE_COMMAND_PACKET_IDENTIFY: // 0xA1
		OnIdentifyPacketDevice();
		return true;

	case IDE_COMMAND_SECURITY_UNLOCK: // 0xF2
		//@todo abort command if device not present
		OnSecurityUnlockCommand();
		return true;

	default:
		EPRINT( "Unknown command %02x sent to %s", data, GetName() );
		return false;
	}
}	

void
IdeDevice::WriteCylinderHigh( u8 &data )
{
	m_hob.hcyl = m_cylinderNum >> 8;
	m_cylinderNum = (data << 8) | (m_cylinderNum & 0x00FF);
}

void
IdeDevice::WriteCylinderLow( u8 &data )
{
	m_hob.lcyl = m_cylinderNum & 0x00FF;
	m_cylinderNum = (m_cylinderNum & 0xFF00) | data;
}

void
IdeDevice::WriteFeatures( u8 &data )
{
	m_hob.feature = m_features;
	m_features = data;
}

void
IdeDevice::WriteHeadNum( u8 headNum )
{
	m_headNum = headNum;
}

void
IdeDevice::WriteLbaMode( u8 lbaMode )
{
	m_lbaMode = lbaMode;
}

void
IdeDevice::WriteSectorCount( u8 &data )
{
	m_hob.nsector = m_sectorCount;
	m_sectorCount = data;
}

void
IdeDevice::WriteSectorNum( u8 &data )
{
	m_hob.sector = m_sectorNum;
	m_sectorNum = data;
}

u8
IdeDevice::ReadStatus()
{
	u8 status = 0;

	status |= m_status.busy << 7;
	status |= m_status.driveReady << 6;
	status |= m_status.writeFault << 5;
	status |= m_status.seekComplete << 4;
	status |= m_status.drq << 3;
	status |= m_status.correctedData << 2;
	status |= m_status.indexPulse << 1;
	status |= m_status.err << 0;

	m_status.indexPulseCount++;
	m_status.indexPulse = false;
	
	if( m_status.indexPulseCount >= INDEX_PULSE_CYCLE ) {
		m_status.indexPulse = true;
		m_status.indexPulseCount = 0;
	}

	return status;
}

void
IdeDevice::AbortCommand()
{
	m_currentCommand = 0x00;
	m_status.busy = false;
	m_status.driveReady = true;
	m_status.err = true;
	m_status.drq = false;
	m_status.correctedData = false;
	m_errorRegister = 0x04; // command ABORTED
	m_bufferIndex = 0;
	//@todo raise irql
}

bool
IdeDevice::IncrementAddresses( u64 sector )
{
	m_sectorCount--;
	m_numSectors--;

	if( m_lbaMode ) {
		sector++;
		if( m_lba48 ) {
			m_hob.hcyl = (u8)((sector >> 40) & 0xFF);
			m_hob.lcyl = (u8)((sector >> 32) & 0xFF);
			m_hob.sector = (u8)((sector >> 24) & 0xFF);
			m_cylinderNum = (u16)((sector >> 8) & 0xFFFF);
			m_sectorNum = (u8)((sector >> 0) & 0xFF);
		}
		else {
			m_headNum = (u8)((sector >> 24) & 0xF);
			m_cylinderNum = (u8)((sector >> 8) & 0xFFFF);
			m_sectorNum = (u8)((sector >> 0) & 0xFF);
		}
		return true;
	}
	else {
		EPRINT( "C/H/S mode not supported" );
		return false;
	}
}

bool
IdeDevice::PerformRead( u8 command )
{
	u64 sectorNum = 0;
	u8 *buffer = m_buffer;
	u32 sectorCount;

	if( !m_lbaMode && ( m_headNum == 0 ) &&
	    ( m_cylinderNum == 0 ) && ( m_sectorNum == 0 ) ) {
		AbortCommand();
		return true;
	}

	if( m_lbaMode ) {
		if( m_lba48 ) {
			sectorNum = ((u64)m_hob.hcyl << 40) | ((u64)m_hob.lcyl << 32) |
			            ((u64)m_hob.sector << 24) | ((u64)m_cylinderNum << 8) |
			            ((u64)m_sectorNum);
		}
		else {
			sectorNum = (m_headNum << 24) | (m_cylinderNum << 8) | m_sectorNum;
		}
	}
	else {
		EPRINT( "C/H/S mode not supported" );
		return false;
	}

	DPRINT( "Yo, READ SECTORS %02x %lx", command, sectorNum * m_sectorSize );

	if( m_lba48 ) {
		if( m_sectorCount == 0 ) {
			m_numSectors = 256;
		}
		else {
			m_numSectors = m_sectorCount;
		}
	}
	else {
		if( (m_sectorCount == 0) && (m_hob.nsector == 0) ) {
			m_numSectors = 0;
		}
		else {
			m_numSectors = (m_hob.nsector << 8) | m_sectorCount;
		}
	}

	if( (command == IDE_COMMAND_READ_MULTIPLE_SECTORS) || (command == IDE_COMMAND_READ_MULTIPLE_SECTORS_EXT) ) {
		if( m_multipleSectors == 0 ) {
			AbortCommand();
			return true;
		}
		if( m_numSectors > m_multipleSectors ) {
			sectorCount = m_multipleSectors;
		}
		else {
			sectorCount = m_numSectors;
		}
	}
	else {
		sectorCount = 1;
	}
	m_currentCommand = command;

	m_bufferSize = sectorCount * m_sectorSize;

	do {
		if( !m_diskImage->FillBuffer( buffer, sectorNum * m_sectorSize, m_sectorSize ) ) {
			AbortCommand();
			return true;
		}

		IncrementAddresses( sectorCount );
		buffer += 512;
	} while( --sectorCount > 0 );

	m_errorRegister = 0;
	m_status.busy = false;
	m_status.driveReady = true;
	m_status.seekComplete = true;
	m_status.drq = true;
	m_status.correctedData = false;
	m_bufferIndex = 0;

	//@todo raise irql

	//DPRINT( "Raise IRQL" );

	return true;
}
