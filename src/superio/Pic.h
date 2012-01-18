#ifndef XBVM_SUPERIO_PIC_H
#define XBVM_SUPERIO_PIC_H

#include "HostDevice.h"

//Actually both pics, but put together for convenience sake
class Pic : public HostDevice 
{
public:
	Pic();
	virtual ~Pic();

	virtual bool IoWrite8( u16 port, u8 &data );

	virtual const char* GetName( void ) { return "Programmable Interrupt Controller"; }

private:
	bool CommandWrite( u8 command, bool isMaster );
	bool DataWrite( u8 command, bool isMaster );
};

extern Pic *g_pic;

#endif /*XBVM_SUPERIO_PIC_H*/

