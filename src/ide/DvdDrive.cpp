#include <cstring>

#include "ide/DvdDrive.h"
#include "Log.h"

void
DvdDrive::OnAtapiSoftReset()
{
	m_errorRegister &= ~(1 << 7);

	m_status.writeFault = false;
	m_status.drq = false;
	m_status.correctedData = false;
	m_status.busy = false;

	DPRINT( "Yo yo yo" );
}

void
DvdDrive::OnIdentifyPacketDevice()
{
	m_currentCommand = IDE_COMMAND_PACKET_IDENTIFY;
	m_errorRegister = 0x00;

	m_status.busy = false;
	m_status.driveReady = true;
	m_status.writeFault = false;
	m_status.drq = true;
	m_status.seekComplete = true;
	m_status.correctedData = false;
	m_bufferIndex = 0;

	PopulateIdSector( (u16*)m_buffer );

	//@todo raise irql
}

void
DvdDrive::PopulateIdSector( u16 *buffer )
{
	memset( buffer, 0, IDE_DEVICE_SECTOR_SIZE );
}

