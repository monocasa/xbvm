#ifndef XBVM_NV2A_H
#define XBVM_NV2A_H

#include "PciDevice.h"
#include "Types.h"

#define NV2A_REG_PMC_BOOT_0          (0x000000)
#define NV2A_REG_PMC_0010B0          (0x0010B0)
#define NV2A_REG_PMC_0010B4          (0x0010B4)
#define NV2A_REG_PMC_0010B8          (0x0010B8)
#define NV2A_REG_PMC_0010BC          (0x0010BC)
#define NV2A_REG_PMC_0010C4          (0x0010C4)
#define NV2A_REG_PMC_0010C8          (0x0010C8)
#define NV2A_REG_PMC_0010CC          (0x0010CC)
#define NV2A_REG_PMC_0010D4          (0x0010D4)
#define NV2A_REG_PMC_0010D8          (0x0010D8)
#define NV2A_REG_PMC_0010DC          (0x0010DC)
#define NV2A_REG_PMC_0010E8          (0x0010E8)
#define NV2A_REG_PMC_001210          (0x001210)
#define NV2A_REG_PMC_001214          (0x001214)
#define NV2A_REG_PMC_001218          (0x001218)
#define NV2A_REG_PMC_001220          (0x001220)
#define NV2A_REG_PMC_001228          (0x001228)
#define NV2A_REG_PMC_00122C          (0x00122C)
#define NV2A_REG_PMC_001230          (0x001230)
#define NV2A_REG_PMC_001234          (0x001234)
#define NV2A_REG_PMC_001238          (0x001238)
#define NV2A_REG_PMC_00123C          (0x00123C)
#define NV2A_REG_PMC_001240          (0x001240)
#define NV2A_REG_PMC_001244          (0x001244)
#define NV2A_REG_PMC_001248          (0x001248)
#define NV2A_REG_PMC_00124C          (0x00124C)
#define NV2A_REG_PMC_001250          (0x001250)
#define NV2A_REG_PMC_001264          (0x001264)

#define NV2A_MMIO_SIZE (16*1024*1024)
#define NV2A_ENGINE_PAGE_SIZE (4096)
#define NV2A_NUM_ENGINE_MMIO_PAGES (NV2A_MMIO_SIZE / NV2A_ENGINE_PAGE_SIZE)

#define NV2A_MMIO_BASE_UNKNOWN  (0x000000)  // Catches all unknown engine mmio accesses
#define NV2A_MMIO_BASE_PMC      (0x000000)  // Card Master Control
#define NV2A_MMIO_BASE_PBUS     (0x001000)  // Bus Control
#define NV2A_MMIO_BASE_PFIFO    (0x002000)  // FIFO
#define NV2A_MMIO_BASE_PRMA     (0x007000)  // BAR0/1 Access From Real Mode
#define NV2A_MMIO_BASE_PVIDEO   (0x008000)  // Video Overlay
#define NV2A_MMIO_BASE_PTIMER   (0x009000)  // Time management and alarms
#define NV2A_MMIO_BASE_PCOUNTER (0x00A000)  // Performance monitoring
#define NV2A_MMIO_BASE_PRM      (0x0A0000)  // Alias VGA memory window
#define NV2A_MMIO_BASE_PRMVIO   (0x0C0000)  // Alias VGA sequencer regs
#define NV2A_MMIO_BASE_PFB      (0x100000)  // Memory interface
#define NV2A_MMIO_BASE_PSTRAPS  (0x101000)  // Straps readout / override
#define NV2A_MMIO_BASE_PGRAPH   (0x400000)  // Graphics engine
#define NV2A_MMIO_BASE_PCRTC    (0x600000)  // More CRTC controls 
#define NV2A_MMIO_BASE_PRMCIO   (0x601000)  // Aliases VGA CRTC and attribute controller registers
#define NV2A_MMIO_BASE_PRAMDAC  (0x680000)  // RAMDAC, cursor, and PLL control
#define NV2A_MMIO_BASE_PRMDIO   (0x681000)  // Aliases VGA palette registers
#define NV2A_MMIO_BASE_PRAMIN   (0x700000)  // RAMIN Access
#define NV2A_MMIO_BASE_USER     (0x800000)  // PFIFO MMIO and DMA submission area

class BusControlEngine;
class Engine;
class MasterControlEngine;
class MemoryInterfaceEngine;
class RamDacEngine;
class CrtcEngine;
class StrapsEngine;
class UnknownEngine;
class VgaEmulationEngine;
class VgaSequencerEngine;
class VideoOverlayEngine;

class Nv2a : public PciDevice
{
public:
	Nv2a();
	virtual ~Nv2a();

	virtual bool MmioWrite32( u32 addr, u32 data );
	virtual bool MmioWrite8( u32 addr, u8 data );
	virtual u32 MmioRead32( u32 addr );
	virtual u8 MmioRead8( u32 addr );

	virtual	const char* GetName( void ) { return "NV2A GPU"; }

	virtual bool WriteBAR( int num, u32 &data );

	void RegisterEngineMmio( u32 pageAddr, Engine *engine ) {
		m_engineMmioTable[ pageAddr / NV2A_ENGINE_PAGE_SIZE ] = engine;
	}

	bool IsVideoOverlayValid();
	u32 GetOverlayAddress();

private:
	Engine* GetEngineForAddress( u32 address );

	u32  m_mmioBase;
	bool m_mmioBaseAssigned;

	Nv2a( Nv2a& );
	const Nv2a& operator=( Nv2a& );

	Engine* m_engineMmioTable[ NV2A_NUM_ENGINE_MMIO_PAGES ];

	UnknownEngine *m_unknownEngine;
	BusControlEngine *m_bus;
	MasterControlEngine *m_mc;
	MemoryInterfaceEngine *m_fb;
	RamDacEngine *m_ramdac;
	CrtcEngine *m_crtc;
	StrapsEngine *m_straps;
	VgaEmulationEngine *m_vga;
	VgaSequencerEngine *m_vgaSeq;
	VideoOverlayEngine *m_video;
};

extern Nv2a *g_nv2a;

#endif /*XBVM_NV2A_H*/

