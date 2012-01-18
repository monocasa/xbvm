#ifndef XBVM_CPU_H
#define XBVM_CPU_H

#include "Types.h"

enum CpuRegisterEnum
{
	CPU_EAX,      CPU_EBX,    CPU_ECX,     CPU_EDX,
	CPU_ESP,      CPU_EBP,    CPU_ESI,     CPU_EDI,
	CPU_EIP,      CPU_EFLAGS, CPU_CR0,     CPU_CR2,
	CPU_CR3,      CPU_CR4,    CPU_EFER,    CPU_APICBASE, 
	CPU_MM0,      CPU_MM1,    CPU_MM2,     CPU_MM3,
	CPU_MM4,      CPU_MM5,    CPU_MM6,     CPU_MM7,
	CPU_CSSELEC,  CPU_CSBASE, CPU_CSLIMIT, CPU_CSFLAGS,
	CPU_DSSELEC,  CPU_DSBASE, CPU_DSLIMIT, CPU_DSFLAGS,
	CPU_ESSELEC,  CPU_ESBASE, CPU_ESLIMIT, CPU_ESFLAGS,
	CPU_FSSELEC,  CPU_FSBASE, CPU_FSLIMIT, CPU_FSFLAGS,
	CPU_GSSELEC,  CPU_GSBASE, CPU_GSLIMIT, CPU_GSFLAGS,
	CPU_SSSELEC,  CPU_SSBASE, CPU_SSLIMIT, CPU_SSFLAGS,
	CPU_LDSELEC,  CPU_LDBASE, CPU_LDLIMIT, CPU_LDFLAGS,
	CPU_TRSELEC,  CPU_TRBASE, CPU_TRLIMIT, CPU_TRFLAGS,
	CPU_GDTB,     CPU_GDTL,   CPU_IDTB,    CPU_IDTL,
	CPU_FCW,      CPU_FSW,    CPU_FTW,     CPU_MXCSR,
	CPU_DR0,      CPU_DR1,    CPU_DR2,     CPU_DR3,
	CPU_DR6,      CPU_DR7,    NUM_CPU_REGS
};

enum CpuSegmentFlagEnum
{
	CPU_SEGMENT_FLAG_PRESENT,
	CPU_SEGMENT_FLAG_PRIVILEGE
};

struct CpuRegs
{
	u32 eax;
	u32 ebx;
	u32 ecx;
	u32 edx;
	u32 esi;
	u32 edi;
	u32 esp;
	u32 ebp;
	u32 eip;
	u32 eflags;
};

struct CpuSegment
{
	u32 base;
	u32 limit;
	u32 selector;
	u8  type;
	u8  present;
	u8  dpl;
	u8  db;
	u8  s;
	u8  l;
	u8  g;
	u8  avl;
};

struct CpuDescriptorTable
{
	u32 base;
	u16 limit;
};

struct CpuSystemRegs
{
	CpuSegment cs;
	CpuSegment ds;
	CpuSegment es;
	CpuSegment fs;
	CpuSegment gs;
	CpuSegment ss;
	CpuSegment tr;
	CpuSegment ldt;
	CpuDescriptorTable gdt;
	CpuDescriptorTable idt;
	u32 cr0;
	u32 cr2;
	u32 cr3;
	u32 cr4;
};

class Cpu
{
public:
	Cpu();
	virtual ~Cpu();

	virtual int Initialize( void ) = 0;
	virtual int Run( void ) = 0;
	virtual int SingleStep( void ) = 0;
	virtual int PrintRegs( void ) = 0;
	virtual u32 GetReg( CpuRegisterEnum reg ) = 0;

	virtual bool GetRegs( CpuRegs &regs ) = 0;
	virtual bool GetSystemRegs( CpuSystemRegs &sregs ) = 0;

	static u64 GetInitialValue( CpuRegisterEnum reg );
	static int GetInitialFlag( CpuRegisterEnum flagReg, CpuSegmentFlagEnum flag );

private:
	const static u64 m_initValues[ NUM_CPU_REGS ];
	const static char *m_regNames[ NUM_CPU_REGS ];
};

extern Cpu *g_cpu;

#endif /*XBVM_CPU_H*/

