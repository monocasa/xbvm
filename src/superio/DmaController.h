#ifndef SUPERIO_DMACONTROLLER_H
#define SUPERIO_DMACONTROLLER_H

#include "HostDevice.h"

class DmaController : public HostDevice
{
public:
	DmaController( u16 basePort, bool doubleStep );
	virtual ~DmaController();

	virtual const char* GetName() { return "DMA Controller"; }

	virtual bool IoWrite8( u16 port, u8 &data );

private:
	u16 m_basePort;
};

extern DmaController *g_dma0;
extern DmaController *g_dma1;

#endif /*SUPERIO_DMACONTROLLER_H*/

