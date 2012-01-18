#ifndef XBVM_INTERPRETERCPU_H
#define XBVM_INTERPRETERCPU_H

#include "Cpu.h"
#include "Mem.h"
#include "Types.h"

enum CpuSegmenEnum {
	CPU_SEGMENT_CS,
	CPU_SEGMENT_DS,
	CPU_SEGMENT_ES,
	CPU_SEGMENT_FS,
	CPU_SEGMENT_GS,
	CPU_SEGMENT_SS,
	CPU_SEGMENT_LDTs,
	CPU_SEGMENT_TR,
};

class InterpreterCpu : public Cpu
{
public:
	InterpreterCpu();
	virtual ~InterpreterCpu();

	virtual int Initialize( void );
	virtual int Run( void );
	virtual int SingleStep( void );
	virtual int PrintRegs( void );

private:
	u32 eax;
	u32 ebx;
	u32 ecx;
	u32 edx;
	u32 edi;
	u32 esi;
	u32 ebp;
	u32 esp;
	u32 eip;
	u32 eflags;

	u32 cs_base;
	u32 cs_limit;

	u16 gdt_size;
	u32 gdt_offset;

	u16 idt_size;
	u32 idt_offset;

	u32 AddrToPhys( u32 virt )
	{
		if ( 0 == (eflags & 1) ) {
			//real mode
			return (virt & 0xffff) + cs_base;
		}

		else {
			//protected mode
			return virt + cs_base;
		}
	}

	u8 ReadNextByte( void )
	{
		return g_mem->ReadPhysU32( AddrToPhys(eip + m_instrOffset++) );
	}

	u16 ReadNextWord( void )
	{
		u16 word = ReadNextByte();
		word |= ReadNextByte() << 8;
		return word;
	}

	u32 ReadNextDWord( void )
	{
		u32 dword = ReadNextWord();
		dword |= ReadNextWord() << 16;
		return dword;
	}

	int m_instrOffset;

	u8 m_modRm;

	bool m_operandSizeOverride;
	bool m_addressSizeOverride;

	bool m_csSegmentOverride;
	bool m_dsSegmentOverride;
	bool m_esSegmentOverride;
	bool m_fsSegmentOverride;
	bool m_gsSegmentOverride;
	bool m_ssSegmentOverride;

	int DecodeMultibyte( void );
	int DecodeGroup7( void );

	//In InterpreterCpu_sys.cpp
	int Lgdt( void );
	int Lidt( void );
};

#endif /*XBVM_INTERPRETERCPU_H*/

