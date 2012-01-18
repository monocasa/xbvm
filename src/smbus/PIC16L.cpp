#include "smbus/PIC16L.h"
#include "Log.h"

PIC16L *g_pic16l;

const char *PIC16L::VERSION_STRING_DEBUG      = "DXB";
const char *PIC16L::VERSION_STRING_RETAIL_1_0 = "P05";
const char *PIC16L::VERSION_STRING_RETAIL_1_1 = "P01";

const char LED_COLOURS[] = { '-', 'R', 'G', 'O' };

bool
PIC16L::WriteCommand8( u8 command, u8 data )
{
	switch( command ) {
	case 0x1:
		WriteVersion( data );
		return true;

	case 0x6:
		m_powerFanSpeed = data;
		return true;

	case 0x7:
		if( data == 1 ) {
			DPRINT( "Using custom LED sequence" );
		}
		else {
			DPRINT( "Using automatic LED sequence" );
		}
		return true;

	case 0x8:
		WriteLEDSequence( data );
		return true;

	case 0xB:
		m_unknownReg0B = data;
		return true;

	case 0x12:
		m_unknownReg12 = data;
		return true;

	case 0x19:
		if( data == 1 ) {
			m_resetOnEject = false;
		}
		else if( data == 0 ) {
			m_resetOnEject = true;
		}
		return true;

	case 0x1a:
		if( data == 1 ) {
			m_interruptEnabled = true;
		}
		return true;

	case 0x1b:
		m_scratchBuffer = data;
		return true;

	case 0x13:
		m_unknownReg13 = data;
		return true;

	case 0x20:
		if( data != 0x40 ) {
			EPRINT( "Wrong response to PIC challenge %02x:%02x but what ev's, this is an emulator.  We'll let it slide.", command, data );
		}
		return true;

	case 0x21:
		if( data != 0x7D ) {
			EPRINT( "Wrong response to PIC challenge %02x:%02x but what ev's, this is an emulator.  We'll let it slide.", command, data );
		}
		return true;

	default:
		return SmBusDevice::WriteCommand8( command, data );
	}
}

bool
PIC16L::ReadCommand8( u8 command, u8 *data )
{
	switch( command ) {
	case 0x1:
		*data = m_version[ m_versionCurrentOffset ];
		m_versionCurrentOffset++;
		if( m_versionCurrentOffset >= 3 )
			m_versionCurrentOffset = 0;
		return true;

	case 0x4:
		*data = 0x06; //Telling that a standard composite AV pack is plugged in
		return true;

	case 0x06:
		*data = m_powerFanSpeed;
		return true;

	case 0x1C:
		*data = 0x52;
		return true;

	case 0x1D:
		*data = 0x72;
		return true;

	case 0x1E:
		*data = 0xEA;
		return true;

	case 0x1F:
		*data = 0x46;
		return true;
	
	default:
		return SmBusDevice::ReadCommand8( command, data );
	}
}

void
PIC16L::WriteVersion( u16 data )
{
	if( 0 == data ) {
		m_versionCurrentOffset = 0;
	}
	else {
		EPRINT( "Unknown Versio command %d written to %s", data, GetName() );
	}
}

void
PIC16L::WriteLEDSequence( u8 data )
{
	char str[ 5 ];

	int rbits = data >> 4;
	int gbits = data & 4;

	for( int i = 0; i < 4; i++ ) {
		int offset = ((rbits & 1) << 1) | (gbits & 1);
		str[ i ] = LED_COLOURS[ offset ];
	}
	str[ 4 ] = '\0';

	DPRINT( "LED colour set to %s", str );
}

