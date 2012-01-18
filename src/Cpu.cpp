#include "Cpu.h"
#include "Log.h"

Cpu *g_cpu;

//TODO:  Verify EDX
const u64 Cpu::m_initValues[NUM_CPU_REGS] =
{
	0x00000010, 0x00000000, 0x00000000, 0x00000633, // EAX    EBX    ECX    EDX
	0x00000000, 0x00000000, 0x00000000, 0x00000000, // ESP    EBP    ESI    EDI
	0xFFFFFE0A, 0x00000046, 0x60000011, 0x00000000, // EIP    EFLAGS CR0    CR2
	0x00000000, 0x00000000, 0x00000000, 0xFEE00000, // CR3    CR4    EFER   APICBASE
	0x00000000, 0x00000000, 0x00000000, 0x00000000, // MM0    MM1    MM2    MM3
	0x00000000, 0x00000000, 0x00000000, 0x00000000, // MM4    MM5    MM6    MM7
	    0x0008, 0x00000000, 0xFFFFFFFF, 0x00C09B00, // SELEC  BASE   LIMIT  FLAGS {CS}
	    0x0010, 0x00000000, 0xFFFFFFFF, 0x00C09300, // SELEC  BASE   LIMIT  FLAGS {DS}
	    0x0010, 0x00000000, 0xFFFFFFFF, 0x00C09300, // SELEC  BASE   LIMIT  FLAGS {ES}
	    0x0000, 0x00000000, 0x0000FFFF, 0x00009300, // SELEC  BASE   LIMIT  FLAGS {FS}
	    0x0000, 0x00000000, 0x0000FFFF, 0x00009300, // SELEC  BASE   LIMIT  FLAGS {GS}
	    0x0010, 0x00000000, 0xFFFFFFFF, 0x00C09300, // SELEC  BASE   LIMIT  FLAGS {SS}
	    0x0000, 0x00000000, 0x0000FFFF, 0x00008200, // SELEC  BASE   LIMIT  FLAGS {LDT}
	    0x0000, 0x00000000, 0x0000FFFF, 0x00008B00, // SELEC  BASE   LIMIT  FLAGS {TR}
	0xFFFFFFD8,     0x0018, 0xFFFFFFD8,     0x0018, // GDTB   GDTL   IDTB   IDTL
	    0x037F,     0x0000,       0x00, 0x00001F80, // FCW    FSW    FTW    MXCSR
	0x00000000, 0x00000000, 0x00000000, 0x00000000, // DR0    DR1    DR2    DR3
	0xFFFF0FF0, 0x00000400                          // DR6    DR7  
};


Cpu::Cpu()
{
}

Cpu::~Cpu()
{
}

u64
Cpu::GetInitialValue(CpuRegisterEnum reg)
{
	if( reg >= NUM_CPU_REGS )
		return 0;

	return m_initValues[reg];
}

int
Cpu::GetInitialFlag( CpuRegisterEnum flagReg, CpuSegmentFlagEnum flagNum )
{
	u64 flags = GetInitialValue( flagReg );

	switch( flagNum ) {
	case CPU_SEGMENT_FLAG_PRESENT:
		return (flags >> 15) & 1;

	case CPU_SEGMENT_FLAG_PRIVILEGE:
		return (flags >> 13) & 3;
	default:
		EPRINT( "Unknown flag: %d", flagReg );
		return 0;
	}
}

