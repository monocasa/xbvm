#include "superio/DmaController.h"
#include "Mem.h"

DmaController *g_dma0;
DmaController *g_dma1;

DmaController::DmaController( u16 basePort, bool doubleStep ) : HostDevice(),
	m_basePort( basePort )
{
	int interval = doubleStep ? 2 : 1;
	for( int i = 0; i < 16; i++ ) {
		g_mem->AddDeviceToIoMap( this, m_basePort + (i *interval) );
	}
}

DmaController::~DmaController()
{
}

bool
DmaController::IoWrite8( u16 port, u8 &data )
{
	// Ignore all writes for now, I like to think that no software is 
	//   depending on ISA DMA controllers.
	DPRINT( "Write of %02x to DMA%02x:%02x", data, m_basePort, port );
	return true;
}

