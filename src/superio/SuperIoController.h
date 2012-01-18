#ifndef XBVM_SUPERIO_SUPERIOCONTROLLER_H
#define XBVM_SUPERIO_SUPERIOCONTROLLER_H

#include "HostDevice.h"

class SuperIoController : public HostDevice
{
public:
	SuperIoController();
	virtual ~SuperIoController();

	virtual bool IoRead8( u16 port, u8 &data );
	virtual bool IoWrite8( u16 port, u8 &data );

	virtual const char* GetName( void ) { return "Super I/O Controller"; }

private:
	u8 m_port2E;
	u8 m_port2F;
	u8 m_port4E;
	u8 m_port4F;
	u8 m_port61;
};

extern SuperIoController *g_superIo;

#endif /*XBVM_SUPERIO_SUPERIOCONTROLLER_H*/

