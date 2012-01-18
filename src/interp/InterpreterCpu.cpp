#include "interp/InterpreterCpu.h"
#include "Log.h"
#include "Mem.h"

InterpreterCpu::InterpreterCpu() :
	eax( 0 ),
	ebx( 0 ),
	ecx( 0 ),
	edx( 0 ),
	edi( 0 ),
	esi( 0 ),
	ebp( 0 ),
	esp( 0 ),
	eip( 0 ),
	eflags( 0 ),
	cs_base( 0 ),
	cs_limit( 0 ),
	gdt_size( 0 ),
	gdt_offset( 0 ),
	idt_size( 0 ),
	idt_offset( 0 ),
	m_instrOffset( 0 ),
	m_modRm( 0 ),
	m_operandSizeOverride( false ),
	m_addressSizeOverride( false ),
	m_csSegmentOverride( false ),
	m_dsSegmentOverride( false ),
	m_esSegmentOverride( false ),
	m_fsSegmentOverride( false ),
	m_gsSegmentOverride( false ),
	m_ssSegmentOverride( false )
{
}

InterpreterCpu::~InterpreterCpu()
{
}

int
InterpreterCpu::Initialize()
{
	return 0;
}

int
InterpreterCpu::Run( void )
{
	int status = 0;

	while( (status = SingleStep()) == 0 );

	return status;
}

int
InterpreterCpu::SingleStep( void )
{
	int ret = 0;
	u8 opcode = 0;
	bool loopForPrefix = true;

	m_instrOffset = 0;

	m_modRm = 0;

	m_operandSizeOverride = false;
	m_addressSizeOverride = false;

	m_csSegmentOverride = false;
	m_dsSegmentOverride = false;
	m_esSegmentOverride = false;
	m_fsSegmentOverride = false;
	m_gsSegmentOverride = false;
	m_ssSegmentOverride = false;

	while( loopForPrefix )
	{
		opcode = ReadNextByte();
		
		switch( opcode ) {
		case 0x2e:
			m_csSegmentOverride = true;
			break;

		case 0x66:
			m_operandSizeOverride = true;
			break;

		case 0x67:
			m_addressSizeOverride = true;
			break;

		default:
			loopForPrefix = false;
		}
	}

	switch( opcode ) {
	case 0xEB: //jmp absolute one byte
		eip += 2 + (s8)ReadNextByte();
		eip -= m_instrOffset;
		break;

	case 0x0f:
		ret = DecodeMultibyte();
		break;
		
	default:
		EPRINT( "Unknown Opcode %02x at %08x", opcode, eip );
		ret = -1;
	}

	if( 0 == ret ) {
		eip += m_instrOffset;
	}

	return ret;
}

int
InterpreterCpu::DecodeMultibyte( void )
{
	u8 opcode = ReadNextByte();
	switch( opcode ) {
	case 0x01:
		return DecodeGroup7();

	default:
		EPRINT( "Unknown Opcode 0f %02x at %08x", opcode, eip );
		return -1;
	}
}

int
InterpreterCpu::DecodeGroup7( void )
{
	m_modRm = ReadNextByte();

	switch( ((m_modRm >> 3) & 0x7) )
	{
	case 2:
		return Lgdt();

	case 3:
		return Lidt();
	
	default:
		EPRINT( "Unknown Group 7 Opcode 0f 01 %02x %d", m_modRm, ((m_modRm >> 3) & 0x7) );
		return -1;
	}
}

int
InterpreterCpu::PrintRegs( void )
{
	return -1;
}

