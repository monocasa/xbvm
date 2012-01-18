#include "interp/InterpreterCpu.h"
#include "Log.h"

int
InterpreterCpu::Lgdt( void )
{
	if( !m_csSegmentOverride ) {
		EPRINT( "can't lgdt without cs segment override" );
		return -1;
	}
	u32 description_offset = ReadNextWord() + cs_base;

	gdt_size = g_mem->ReadPhysU16( description_offset );
	gdt_offset = g_mem->ReadPhysU32( description_offset + 2 );
	
	return 0;
}

int
InterpreterCpu::Lidt( void )
{
	if( !m_csSegmentOverride ) {
		EPRINT( "can't lidt without cs segment override" );
		return -1;
	}
	u32 description_offset = ReadNextWord() + cs_base;

	idt_size = g_mem->ReadPhysU16( description_offset );
	idt_offset = g_mem->ReadPhysU32( description_offset + 2 );
	
	return 0;
}

