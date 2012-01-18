#ifndef XBVM_KVMCPU_H
#define XBVM_KVMCPU_H

#include <linux/kvm.h>

#include "Cpu.h"
#include "Types.h"

class KvmCpu : public Cpu
{
public:
	KvmCpu();
	virtual ~KvmCpu();

	virtual int Initialize( void );
	virtual int Run( void );
	virtual int SingleStep( void );
	virtual int PrintRegs( void );
	virtual u32 GetReg( CpuRegisterEnum reg );

	virtual bool GetRegs( CpuRegs &regs );
	virtual bool GetSystemRegs( CpuSystemRegs &sregs );

private:
	int m_kvmFd;
	int m_vmFd;
	int m_vcpuFd;
	struct kvm_run *m_kvmRun;
	int m_kvmRunSize;

	KvmCpu( KvmCpu& );
	const KvmCpu& operator=( KvmCpu& );

	bool OpenKvmInterface( void );
	bool MmapKvmRun( void );
	bool SetupMemoryRegions( void );
	bool SetupRegisters( void );
	void SetSegmentFlags( struct kvm_segment *segment, u32 flags );

	bool KvmIoctl( int request, void *value, int &ret );
	bool VmIoctl( int request, void *value, int &ret );
	bool VcpuIoctl( int request, void *value, int &ret );

	bool ExtensionExists( int extension );

	bool  KvmIoctl( int request, void *value ) { int ret; return  KvmIoctl( request, value, ret ); }
	bool   VmIoctl( int request, void *value ) { int ret; return   VmIoctl( request, value, ret ); }
	bool VcpuIoctl( int request, void *value ) { int ret; return VcpuIoctl( request, value, ret ); }
};

#endif /*XBVM_KVMCPU_H*/

