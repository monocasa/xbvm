#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/mman.h>

#include "kvm/KvmCpu.h"
#include "Log.h"
#include "Mem.h"

KvmCpu::KvmCpu() :
	m_kvmFd(0), m_vmFd(0), m_vcpuFd(0), m_kvmRun(NULL),
	m_kvmRunSize(0)
{
}

KvmCpu::~KvmCpu()
{
}

int
KvmCpu::Initialize()
{
	if( !OpenKvmInterface() ) {
		EPRINT( "Error opening /dev/kvm" );
		return -1;
	}

	if( !ExtensionExists( KVM_CAP_IRQCHIP ) ) {
		EPRINT( "KVM Interface doesn't have KVM_CAP_IRQCHIP capability" );
		return -2;
	}

	if( !KvmIoctl( KVM_CREATE_VM, 0, m_vmFd ) ) {
		EPRINT( "Error creating VM" );
		return -3;
	}

	if( !VmIoctl( KVM_SET_TSS_ADDR, (void*)0xE0000000 ) ) {
		EPRINT( "Error setting TSS address range" );
		return -4; 
	}

	if( !VmIoctl( KVM_CREATE_VCPU, 0, m_vcpuFd ) ) {
		EPRINT( "Error creating VPCU" );
		return -5;
	}

	if( !KvmIoctl( KVM_GET_VCPU_MMAP_SIZE, 0, m_kvmRunSize ) ) {
		EPRINT( "Error getting kvm_run size" );
		return -6;
	}

	if( !MmapKvmRun() ) {
		EPRINT( "Error mmaping kvm_run" );
		return -7;
	}

	if( !SetupMemoryRegions() ) {
		return -8;
	}

	if( !SetupRegisters() ) {
		return -9;
	}

	return 0;
}

int
KvmCpu::Run( void )
{
	int exitReason;

	bool running = true;

	while( running ) {
		if( !VcpuIoctl( KVM_RUN, 0 ) ) {
			DPRINT( "Can't KVM_RUN" );
			return -1;
		}

		exitReason = m_kvmRun->exit_reason;

		switch( exitReason ) {

		case KVM_EXIT_DEBUG:
			running = false;
			break;

		case KVM_EXIT_IO:
			if( KVM_EXIT_IO_OUT == m_kvmRun->io.direction ) { 
				u8 *dataPtr = (u8*)m_kvmRun + m_kvmRun->io.data_offset;
				if( 1 == m_kvmRun->io.count ) {
					switch( m_kvmRun->io.size ) {
					case 1:
						running = g_mem->WriteIo8( m_kvmRun->io.port, *(u8*)dataPtr );
						break;

					case 2:
						running = g_mem->WriteIo16( m_kvmRun->io.port, *(u16*)dataPtr );
						break;

					case 4:
						running = g_mem->WriteIo32( m_kvmRun->io.port, *(u32*)dataPtr );
						break;

					default:
						EPRINT( "Unknown io size %d in write", m_kvmRun->io.size );
						running = false;
						break;
					}
				}
				else {
					EPRINT( "Count of one not handled" );
					running = false;
				}
				break;
			}
			else {
				u8 *dataPtr = (u8*)m_kvmRun + m_kvmRun->io.data_offset;
				if( 1 == m_kvmRun->io.count ) {
					switch( m_kvmRun->io.size ) {
					case 1:
						running = g_mem->ReadIo8( m_kvmRun->io.port, *(u8*)dataPtr );
						break;

					case 2:
						running = g_mem->ReadIo16( m_kvmRun->io.port, *(u16*)dataPtr );
						break;

					case 4:
						running = g_mem->ReadIo32( m_kvmRun->io.port, *(u32*)dataPtr );
						break;

					default:
						EPRINT( "Unknown io size %d in read", m_kvmRun->io.size );
						running = false;
						break;
					}
				}
				else {
					EPRINT( "Count of one not handled" );
					running = false;
				}
				break;
			}
			break;

		case KVM_EXIT_MMIO:
			if( m_kvmRun->mmio.is_write ) {
				switch( m_kvmRun->mmio.len ) {
				case 1:
					running = g_mem->WritePhysU8( (u32)m_kvmRun->mmio.phys_addr, m_kvmRun->mmio.data[0] );
					break;
				case 4:
					running = g_mem->WritePhysU32( (u32)m_kvmRun->mmio.phys_addr, *(u32*)m_kvmRun->mmio.data );
					break;
				default:
					EPRINT( "Unknown MMIO Size %d in write", m_kvmRun->mmio.len );
					running = false;
					break;
				}
			}
			else {
				switch( m_kvmRun->mmio.len ) {
				case 1:
					*(u8*)m_kvmRun->mmio.data = g_mem->ReadPhysU8( (u32)m_kvmRun->mmio.phys_addr );
					break;
				case 4:
					*(u32*)m_kvmRun->mmio.data = g_mem->ReadPhysU32( (u32)m_kvmRun->mmio.phys_addr );
					break;
				default:
					EPRINT( "Unknown MMIO Size %d in read", m_kvmRun->mmio.len );
					running = false;
					break;
				}
			}
			break;

		case KVM_EXIT_SHUTDOWN:
			struct kvm_regs regs;
			VcpuIoctl( KVM_GET_REGS, &regs );

			// Hack for cromwell setting invalid cr3
			if( regs.rip == 0xFFFC1023 ) {
				DPRINT( "Executing horrible cromwell hack" );
				struct kvm_sregs sregs;
				VcpuIoctl( KVM_GET_SREGS, &sregs );
				sregs.cr3 = regs.rip & 0xFFFFF000;
				VcpuIoctl( KVM_SET_SREGS, &sregs );
			}

			// If we didn't fit one of the hacks, then we're in a very weird state
			else {
				DPRINT( "regs->eip = %08llx", regs.rip );
				DPRINT( "KVM_EXIT_SHUTDOWN" );
				running = false;
			}
			break;

		case KVM_EXIT_UNKNOWN:
			DPRINT( "KVM_EXIT_UNKNOWN -> %08llx", m_kvmRun->hw.hardware_exit_reason );
			running = false;
			break;

		default:
			DPRINT( "Unknown KVM exit reason of %d", exitReason );
			running = false;
		}

		//fixup eip and wrap around
		if( running ) {
			struct kvm_regs regs;

			if( !VcpuIoctl( KVM_GET_REGS, &regs ) ) {
				EPRINT( "Can not get regs" );
				running = false;
			}
			else {
				if( 0x100000000UL == regs.rip ) {
					DPRINT( "fixing up eip" );
					regs.rip = 0;
					if( !VcpuIoctl( KVM_SET_REGS, &regs ) ) {
						EPRINT( "Can not set regs" );
						running = false;
					}
				}
			}
		}
	}

	return exitReason;
}

int
KvmCpu::SingleStep( void )
{
	int ret = 0;
	struct kvm_guest_debug debug;
	
	debug.control = KVM_GUESTDBG_ENABLE | KVM_GUESTDBG_SINGLESTEP;
	if( !VcpuIoctl( KVM_SET_GUEST_DEBUG, &debug ) ) {
		EPRINT( "Error getting debug flags" );
		return -1;
	}

	ret = Run();

	debug.control = 0;
	if( !VcpuIoctl( KVM_SET_GUEST_DEBUG, &debug ) ) {
		EPRINT( "Error getting debug flags" );
		return -1;
	}

	return ret;
}

int
KvmCpu::PrintRegs( void )
{
	struct kvm_regs regs;

	if( !VcpuIoctl( KVM_GET_REGS, &regs ) ) {
		EPRINT( "Can not get regs" );
		return -1;
	}

	DPRINT( "EAX: %08llx  EBX: %08llx  ECX: %08llx  EDX: %08llx",
	        regs.rax, regs.rbx, regs.rcx, regs.rdx );
	DPRINT( "ESP: %08llx  EBP: %08llx  ESI: %08llx  EDI: %08llx",
	        regs.rsp, regs.rbp, regs.rsi, regs.rdi );
	DPRINT( "EFLAGS: %08llx  EIP: %08llx",
	        regs.rflags, regs.rip );

	return 0;
}

bool
KvmCpu::GetRegs( CpuRegs &retregs )
{
	struct kvm_regs regs;

	if( !VcpuIoctl( KVM_GET_REGS, &regs ) ) {
		EPRINT( "Can not get regs" );
		return false;
	}

	retregs.eax = regs.rax;
	retregs.ebx = regs.rbx;
	retregs.ecx = regs.rcx;
	retregs.edx = regs.rdx;
	retregs.esi = regs.rsi;
	retregs.edi = regs.rdi;
	retregs.esp = regs.rsp;
	retregs.ebp = regs.rbp;
	retregs.eip = regs.rip;
	retregs.eflags = regs.rflags;
	return true;
}

bool
KvmCpu::GetSystemRegs( CpuSystemRegs &retsregs )
{
	struct kvm_sregs sregs;

	if( !VcpuIoctl( KVM_GET_SREGS, &sregs ) ) {
		EPRINT( "Can not get sregs" );
		return false;
	}

	retsregs.cs.base     = sregs.cs.base;
	retsregs.cs.limit    = sregs.cs.limit;
	retsregs.cs.selector = sregs.cs.selector;
	retsregs.cs.type     = sregs.cs.type;
	retsregs.cs.present  = sregs.cs.present;
	retsregs.cs.dpl      = sregs.cs.dpl;
	retsregs.cs.db       = sregs.cs.db;
	retsregs.cs.s        = sregs.cs.s;
	retsregs.cs.l        = sregs.cs.l;
	retsregs.cs.g        = sregs.cs.g;
	retsregs.cs.avl      = sregs.cs.avl;

	retsregs.ds.base     = sregs.ds.base;
	retsregs.ds.limit    = sregs.ds.limit;
	retsregs.ds.selector = sregs.ds.selector;
	retsregs.ds.type     = sregs.ds.type;
	retsregs.ds.present  = sregs.ds.present;
	retsregs.ds.dpl      = sregs.ds.dpl;
	retsregs.ds.db       = sregs.ds.db;
	retsregs.ds.s        = sregs.ds.s;
	retsregs.ds.l        = sregs.ds.l;
	retsregs.ds.g        = sregs.ds.g;
	retsregs.ds.avl      = sregs.ds.avl;

	retsregs.es.base     = sregs.es.base;
	retsregs.es.limit    = sregs.es.limit;
	retsregs.es.selector = sregs.es.selector;
	retsregs.es.type     = sregs.es.type;
	retsregs.es.present  = sregs.es.present;
	retsregs.es.dpl      = sregs.es.dpl;
	retsregs.es.db       = sregs.es.db;
	retsregs.es.s        = sregs.es.s;
	retsregs.es.l        = sregs.es.l;
	retsregs.es.g        = sregs.es.g;
	retsregs.es.avl      = sregs.es.avl;

	retsregs.fs.base     = sregs.fs.base;
	retsregs.fs.limit    = sregs.fs.limit;
	retsregs.fs.selector = sregs.fs.selector;
	retsregs.fs.type     = sregs.fs.type;
	retsregs.fs.present  = sregs.fs.present;
	retsregs.fs.dpl      = sregs.fs.dpl;
	retsregs.fs.db       = sregs.fs.db;
	retsregs.fs.s        = sregs.fs.s;
	retsregs.fs.l        = sregs.fs.l;
	retsregs.fs.g        = sregs.fs.g;
	retsregs.fs.avl      = sregs.fs.avl;

	retsregs.gs.base     = sregs.gs.base;
	retsregs.gs.limit    = sregs.gs.limit;
	retsregs.gs.selector = sregs.gs.selector;
	retsregs.gs.type     = sregs.gs.type;
	retsregs.gs.present  = sregs.gs.present;
	retsregs.gs.dpl      = sregs.gs.dpl;
	retsregs.gs.db       = sregs.gs.db;
	retsregs.gs.s        = sregs.gs.s;
	retsregs.gs.l        = sregs.gs.l;
	retsregs.gs.g        = sregs.gs.g;
	retsregs.gs.avl      = sregs.gs.avl;

	retsregs.ss.base     = sregs.ss.base;
	retsregs.ss.limit    = sregs.ss.limit;
	retsregs.ss.selector = sregs.ss.selector;
	retsregs.ss.type     = sregs.ss.type;
	retsregs.ss.present  = sregs.ss.present;
	retsregs.ss.dpl      = sregs.ss.dpl;
	retsregs.ss.db       = sregs.ss.db;
	retsregs.ss.s        = sregs.ss.s;
	retsregs.ss.l        = sregs.ss.l;
	retsregs.ss.g        = sregs.ss.g;
	retsregs.ss.avl      = sregs.ss.avl;

	retsregs.tr.base     = sregs.tr.base;
	retsregs.tr.limit    = sregs.tr.limit;
	retsregs.tr.selector = sregs.tr.selector;
	retsregs.tr.type     = sregs.tr.type;
	retsregs.tr.present  = sregs.tr.present;
	retsregs.tr.dpl      = sregs.tr.dpl;
	retsregs.tr.db       = sregs.tr.db;
	retsregs.tr.s        = sregs.tr.s;
	retsregs.tr.l        = sregs.tr.l;
	retsregs.tr.g        = sregs.tr.g;
	retsregs.tr.avl      = sregs.tr.avl;

	retsregs.ldt.base     = sregs.ldt.base;
	retsregs.ldt.limit    = sregs.ldt.limit;
	retsregs.ldt.selector = sregs.ldt.selector;
	retsregs.ldt.type     = sregs.ldt.type;
	retsregs.ldt.present  = sregs.ldt.present;
	retsregs.ldt.dpl      = sregs.ldt.dpl;
	retsregs.ldt.db       = sregs.ldt.db;
	retsregs.ldt.s        = sregs.ldt.s;
	retsregs.ldt.l        = sregs.ldt.l;
	retsregs.ldt.g        = sregs.ldt.g;
	retsregs.ldt.avl      = sregs.ldt.avl;

	retsregs.gdt.base  = sregs.gdt.base;
	retsregs.gdt.limit = sregs.gdt.limit;

	retsregs.idt.base  = sregs.idt.base;
	retsregs.idt.limit = sregs.idt.limit;

	retsregs.cr0 = sregs.cr0;
	retsregs.cr2 = sregs.cr2;
	retsregs.cr3 = sregs.cr3;
	retsregs.cr4 = sregs.cr4;

	return true;
}

u32
KvmCpu::GetReg( CpuRegisterEnum reg )
{
	struct kvm_regs regs;

	if( !VcpuIoctl( KVM_GET_REGS, &regs ) ) {
		EPRINT( "Can not get regs" );
		return 0xFFFFFFFF;
	}

	switch( reg ) {
	case CPU_EAX:
		return regs.rax;

	case CPU_EIP:
		return regs.rip;

	default:
		EPRINT( "Unknown reg %d", reg );
		return 0xFFFFFFFF;
	}
}

bool
KvmCpu::OpenKvmInterface( void )
{
	if( (m_kvmFd = open( "/dev/kvm", O_RDWR )) < 0 ) {
		EPRINT( "Error opening /dev/kvm" );
		return false;
	}
	return true;
}

bool
KvmCpu::MmapKvmRun( void )
{
	m_kvmRun = reinterpret_cast<struct kvm_run*>(
	              mmap( NULL, 
	                    m_kvmRunSize, 
	                    PROT_READ | PROT_WRITE, 
	                    MAP_SHARED, 
	                    m_vcpuFd, 
	                    0 
	              )
	           );

	if( MAP_FAILED == m_kvmRun ) {
		return false;
	}

	return true;
}

bool
KvmCpu::SetupMemoryRegions( void )
{
	struct kvm_userspace_memory_region ramRegion;
	struct kvm_userspace_memory_region gpuRamRegion;
	struct kvm_userspace_memory_region romRegion;

	ramRegion.slot = 0;
	ramRegion.guest_phys_addr = 0x0UL;
	ramRegion.memory_size = g_mem->GetRamSize();
	ramRegion.userspace_addr = reinterpret_cast<unsigned long>(g_mem->GetRamAddr());

	if( !VmIoctl( KVM_SET_USER_MEMORY_REGION, &ramRegion ) ) {
		return false;
	}

	gpuRamRegion.slot = 1;
	gpuRamRegion.guest_phys_addr = 0xF0000000;
	gpuRamRegion.memory_size = g_mem->GetRamSize();
	gpuRamRegion.userspace_addr = reinterpret_cast<unsigned long>(g_mem->GetRamAddr());

	if( !VmIoctl( KVM_SET_USER_MEMORY_REGION, &gpuRamRegion ) ) {
		return false;
	}

	romRegion.slot = 2;
	romRegion.guest_phys_addr = 0xFF000000UL;
	romRegion.memory_size = g_mem->GetRomSize();
	romRegion.userspace_addr = reinterpret_cast<unsigned long>(g_mem->GetRomAddr());

	if( !VmIoctl( KVM_SET_USER_MEMORY_REGION, &romRegion ) ) {
		return false;
	}

	return true;
}

bool
KvmCpu::SetupRegisters( void )
{
	struct kvm_regs regs;
	struct kvm_sregs sregs;

	if( !VcpuIoctl( KVM_GET_REGS, &regs ) ) {
		EPRINT( "Can not get regs" );
		return false;
	}

	if( !VcpuIoctl( KVM_GET_SREGS, &sregs ) ) {
		EPRINT( "Can not get sregs" );
		return false;
	}

	regs.rax    = Cpu::GetInitialValue( CPU_EAX );
	regs.rbx    = Cpu::GetInitialValue( CPU_EBX );
	regs.rcx    = Cpu::GetInitialValue( CPU_ECX );
	regs.rdx    = Cpu::GetInitialValue( CPU_EDX );
	regs.rsi    = Cpu::GetInitialValue( CPU_ESI );
	regs.rdi    = Cpu::GetInitialValue( CPU_EDI );
	regs.rsp    = Cpu::GetInitialValue( CPU_ESP );
	regs.rbp    = Cpu::GetInitialValue( CPU_EBP );
	regs.rip    = Cpu::GetInitialValue( CPU_EIP );
	regs.rdx    = Cpu::GetInitialValue( CPU_EDX );
	regs.rflags = Cpu::GetInitialValue( CPU_EFLAGS );

	sregs.cr0       = Cpu::GetInitialValue( CPU_CR0 );
	sregs.cr2       = Cpu::GetInitialValue( CPU_CR2 );
	sregs.cr3       = Cpu::GetInitialValue( CPU_CR3 );
	sregs.cr4       = Cpu::GetInitialValue( CPU_CR4 );
	sregs.efer      = Cpu::GetInitialValue( CPU_EFER );
	sregs.apic_base = Cpu::GetInitialValue( CPU_APICBASE );

	sregs.cs.selector = Cpu::GetInitialValue( CPU_CSSELEC );
	sregs.cs.base     = Cpu::GetInitialValue( CPU_CSBASE );
	sregs.cs.limit    = Cpu::GetInitialValue( CPU_CSLIMIT );
	SetSegmentFlags( &sregs.cs, Cpu::GetInitialValue( CPU_CSFLAGS ) );
	sregs.cs.unusable = 0;

	sregs.ds.selector = Cpu::GetInitialValue( CPU_DSSELEC );
	sregs.ds.base     = Cpu::GetInitialValue( CPU_DSBASE );
	sregs.ds.limit    = Cpu::GetInitialValue( CPU_DSLIMIT );
	SetSegmentFlags( &sregs.ds, Cpu::GetInitialValue( CPU_DSFLAGS ) );
	sregs.ds.unusable = 0;

	sregs.es.selector = Cpu::GetInitialValue( CPU_ESSELEC );
	sregs.es.base     = Cpu::GetInitialValue( CPU_ESBASE );
	sregs.es.limit    = Cpu::GetInitialValue( CPU_ESLIMIT );
	SetSegmentFlags( &sregs.es, Cpu::GetInitialValue( CPU_ESFLAGS ) );
	sregs.es.unusable = 0;

	sregs.fs.selector = Cpu::GetInitialValue( CPU_FSSELEC );
	sregs.fs.base     = Cpu::GetInitialValue( CPU_FSBASE );
	sregs.fs.limit    = Cpu::GetInitialValue( CPU_FSLIMIT );
	SetSegmentFlags( &sregs.fs, Cpu::GetInitialValue( CPU_FSFLAGS ) );
	sregs.fs.unusable = 0;

	sregs.gs.selector = Cpu::GetInitialValue( CPU_GSSELEC );
	sregs.gs.base     = Cpu::GetInitialValue( CPU_GSBASE );
	sregs.gs.limit    = Cpu::GetInitialValue( CPU_GSLIMIT );
	SetSegmentFlags( &sregs.gs, Cpu::GetInitialValue( CPU_GSFLAGS ) );
	sregs.gs.unusable = 0;

	sregs.ss.selector = Cpu::GetInitialValue( CPU_SSSELEC );
	sregs.ss.base     = Cpu::GetInitialValue( CPU_SSBASE );
	sregs.ss.limit    = Cpu::GetInitialValue( CPU_SSLIMIT );
	SetSegmentFlags( &sregs.ss, Cpu::GetInitialValue( CPU_SSFLAGS ) );
	sregs.ss.unusable = 0;

	sregs.ldt.selector = Cpu::GetInitialValue( CPU_LDSELEC );
	sregs.ldt.base     = Cpu::GetInitialValue( CPU_LDBASE );
	sregs.ldt.limit    = Cpu::GetInitialValue( CPU_LDLIMIT );
	SetSegmentFlags( &sregs.ldt, Cpu::GetInitialValue( CPU_LDFLAGS ) );
	sregs.ldt.unusable = 0;

	sregs.tr.selector = Cpu::GetInitialValue( CPU_TRSELEC );
	sregs.tr.base     = Cpu::GetInitialValue( CPU_TRBASE );
	sregs.tr.limit    = Cpu::GetInitialValue( CPU_TRLIMIT );
	SetSegmentFlags( &sregs.tr, Cpu::GetInitialValue( CPU_TRFLAGS ) );
	sregs.tr.unusable = 0;

	sregs.gdt.base  = Cpu::GetInitialValue( CPU_GDTB );
	sregs.gdt.limit = Cpu::GetInitialValue( CPU_GDTL );

	sregs.idt.base  = Cpu::GetInitialValue( CPU_IDTB );
	sregs.idt.limit = Cpu::GetInitialValue( CPU_IDTL );

	if( !VcpuIoctl( KVM_SET_SREGS, &sregs ) ) {
		EPRINT( "Can not set sregs" );
		return false;
	}

	if( !VcpuIoctl( KVM_SET_REGS, &regs ) ) {
		EPRINT( "Can not set regs" );
		return false;
	}

	return true;
}

void
KvmCpu::SetSegmentFlags( struct kvm_segment *segment, u32 flags )
{
	segment->type    = (flags >> 8) & 0xf;
	segment->present = (flags >> 15) & 1;
	segment->dpl     = (flags >> 13) & 3;
	segment->db      = (flags >> 22) & 1;
	segment->s       = (flags >> 12) & 1;
	segment->l       = (flags >> 21) & 1;
	segment->g       = (flags >> 23) & 1;
	segment->avl     = (flags >> 20) & 1;
}

bool
KvmCpu::KvmIoctl( int request, void *value, int &ret )
{
	if( 0 == m_kvmFd )
		return false;

	if( (ret = ioctl( m_kvmFd, request, value )) < 0 )
		return false;

	return true;
}

bool
KvmCpu::VmIoctl( int request, void *value, int &ret )
{
	if( 0 == m_vmFd )
		return false;

	if( (ret = ioctl( m_vmFd, request, value )) < 0 )
		return false;

	return true;
}

bool
KvmCpu::VcpuIoctl( int request, void *value, int &ret )
{
	if( 0 == m_vcpuFd )
		return false;

	if( (ret = ioctl( m_vcpuFd, request, value )) < 0 )
		return false;

	return true;
}

bool
KvmCpu::ExtensionExists( int extension )
{
	int ret = 0;
	if( !KvmIoctl( KVM_CHECK_EXTENSION, (void*)extension, ret ) ) {
		return false;
	}
	DPRINT( "Extension %d returns %d", extension, ret );
	return (ret > 0);
}

