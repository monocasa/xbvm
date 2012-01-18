#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "audio/Ac97Codec.h"
#include "audio/Apu.h"
#include "bridges/AgpBridge.h"
#include "bridges/DramController.h"
#include "bridges/HostBridge.h"
#include "bridges/IsaBridge.h"
#include "graphics/Nv2a.h"
#include "ide/IdeController.h"
#include "interp/InterpreterCpu.h"
#include "kvm/KvmCpu.h"
#include "net/NetworkAdapter.h"
#include "smbus/SmBusController.h"
#include "superio/Cmos.h"
#include "superio/DmaController.h"
#include "superio/Pic.h"
#include "superio/Pit.h"
#include "superio/SuperIoController.h"
#include "usb/OhciController.h"
#include "Cpu.h"
#include "Log.h"
#include "Mem.h"
#include "PciBus.h"
#include "Sdl.h"

char tokens [32][256];

void
CliPrint()
{
	if( strcmp( tokens[1], "r" ) == 0 ) {
		g_cpu->PrintRegs();
	}

	else if( strcmp( tokens[1], "pw" ) == 0 ) {
		u32 addr = strtol( tokens[2], NULL, 16 );
		u32 data = g_mem->ReadPhysU32( addr );
		DPRINT( "%08x:  %08x", addr, data );
	}

	else if( strcmp( tokens[1], "vw" ) == 0 ) {
		CpuSystemRegs sregs;
		u32 addr = strtol( tokens[2], NULL, 16 );
		if( !g_cpu->GetSystemRegs( sregs ) ) {
			EPRINT( "Can not get system regs" );
			return;
		}
		u32 pdebase = sregs.cr3 & 0xFFFFF000;
		u32 pde = g_mem->ReadPhysU32( pdebase + ((addr >> 22 ) << 2) );
		if( (pde & 1) != 1 ) {
			EPRINT( "PDE Not Present" );
			return;
		}
		u32 pte = g_mem->ReadPhysU32( (pde & 0xFFFFF000) | (((addr >> 12) << 2) & 0x0000FFF) );
		if( (pte & 1) != 1 ) {
			EPRINT( "PTE Not Present" );
			return;
		}
		u32 value = g_mem->ReadPhysU32( (pte & 0xFFFFF000) | (addr & 0x00000FFF) );
		DPRINT( "%08x:  %08x", addr, value );
		return;
	}

	else if( strcmp( tokens[1], "sr" ) == 0 ) {
		CpuSystemRegs sregs;
		if( !g_cpu->GetSystemRegs( sregs ) ) {
			EPRINT( "Can not get system regs" );
			return;
		}
		DPRINT( "CR0: %08x  CR2: %08x  CR3: %08x  CR4: %08x", 
		        sregs.cr0, sregs.cr2, sregs.cr3, sregs.cr4 );
	}

	else if( strcmp( tokens[1], "sg" ) == 0 ) {
		DPRINT( "~~SEGMENTS~~" );
	}

	else if( strcmp( tokens[1], "f" ) == 0 ) {
		u32 value = strtol( tokens[2], NULL, 16 );
		std::vector<u32> addresses = g_mem->FindAlignedDwords( value );
		for( size_t i = 0; i < addresses.size(); i++ ) {
			DPRINT( "%08x:  %08x", addresses[i], value );
		}
	}

	else if( strcmp( tokens[1], "pg" ) == 0 ) {
		CpuSystemRegs sregs;
		if( !g_cpu->GetSystemRegs( sregs ) ) {
			EPRINT( "Can not get system regs" );
			return;
		}
		u32 pdebase = sregs.cr3 & 0xFFFFF000;
		for(int i = 0; i < 1024; i++) {
			u32 pdeEntry = g_mem->ReadPhysU32( pdebase + (i * sizeof(u32)) );
			if( pdeEntry & 1 ) {
				DPRINT( "%08x:->%08x %s", i << 22, pdeEntry & 0xFFFFF000, (pdeEntry & 0x80) ? "4MiB" : "4KiB" );
			}
		}
	}

	else {
		EPRINT( "Unknown p arg \"%s\"", tokens[1] );
	}
}

void
CliDump()
{
	if( strcmp ( tokens[1], "range" ) == 0 ) {
		int base = atoi( tokens[2] );
		int len = atoi( tokens[3] );
		char *filename = tokens[4];
		 
		DPRINT( "dumping phys range %08x->%08x to file %s", base, base + len, filename );
	}

	else {
		EPRINT( "Unknown d arg \"%s\"", tokens[1] );
	}
}

void
CliRunUntil()
{
	u32 breakpoint = strtol( tokens[1], NULL, 16 );
	do {
		g_cpu->SingleStep();
	} while( g_cpu->GetReg( CPU_EIP ) != breakpoint );
}

int
RunCli( void )
{
	char *line = NULL;
	bool running = true;
	size_t len = 0;

	while( running )
	{
		printf( ">" );
		if( getline( &line, &len, stdin ) == -1 )
			break;

		char* pch;
		int numtokens = 0;

		//printf( "Splitting string \"%s\" into tokens:\n", line );
		//pch = strtok( line, " \n" );
		//while( pch != NULL ) {
		//   printf( "\"%s\"\n", pch );
		//   pch = strtok( NULL, " \n" );
		//}

		pch = strtok( line, " \n" );
		for( int i = 0; i < 32; i++ ) {
			if( pch == NULL )
				break;
			strncpy( &tokens[i][0], pch, 256 );
			numtokens++;
			pch = strtok( NULL, " \n" );
		}

		if( strcmp( "quit", tokens[0] ) == 0 ) {
			running = false;
		}

		else if( strcmp( "exit", tokens[0] ) == 0 ) {
			running = false;
		}

		else if( strcmp( "r", tokens[0] ) == 0 ) {
			g_cpu->Run();
		}

		else if( strcmp( "ru", tokens[0] ) == 0 ) {
			CliRunUntil();
		}

		else if( strcmp( "s", tokens[0] ) == 0 ) {
			g_cpu->SingleStep();
			DPRINT( "%08x", g_cpu->GetReg( CPU_EIP ) );
		}

		else if( strcmp( "p", tokens[0] ) == 0 ) {
			CliPrint();
		}

		else if( strcmp( "d", tokens[0] ) == 0 ) {
			CliDump();
		}

		else {
			EPRINT( "Unknown command \"%s\"", tokens[0] );
		}
	}

	if( line )
		free( line );

	return 0;
}

int
main()
{
	g_mem = new Mem();
	if( !g_mem->Initialize() ) {
		return -1;
	}

	g_superIo = new SuperIoController();

	g_cmos = new Cmos();

	g_pic = new Pic();

	g_pit = new Pit();

	g_dma0 = new DmaController( 0x00, false );
	g_dma1 = new DmaController( 0xC0, true );

	g_pciBus = new PciBus();

	g_hostBridge = new HostBridge();
	g_pciBus->AddPciDevice( g_hostBridge );

	g_dramController = new DramController();
	g_pciBus->AddPciDevice( g_dramController );

	g_isaBridge = new IsaBridge();
	g_pciBus->AddPciDevice( g_isaBridge );

	g_smBus = new SmBusController();
	g_pciBus->AddPciDevice( g_smBus );

	g_net = new NetworkAdapter();
	g_pciBus->AddPciDevice( g_net );

	g_usb0 = new OhciController( 2 );
	g_pciBus->AddPciDevice( g_usb0 );

	g_usb1 = new OhciController( 3 );
	g_pciBus->AddPciDevice( g_usb1 );

	g_apu = new Apu();
	g_pciBus->AddPciDevice( g_apu );

	g_ac97 = new Ac97Codec();
	g_pciBus->AddPciDevice( g_ac97 );

	g_ide = new IdeController();
	g_pciBus->AddPciDevice( g_ide );

	g_agpBridge = new AgpBridge();
	g_pciBus->AddPciDevice( g_agpBridge );

	g_nv2a = new Nv2a();
	g_pciBus->AddPciDevice( g_nv2a );

	g_cpu = new KvmCpu();
	if( g_cpu->Initialize() < 0 ) {
		DPRINT( "Could not initialize Cpu" );
		return -2;
	}

	StartSdlThread();

	return RunCli();
}

