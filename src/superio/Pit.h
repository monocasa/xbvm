#ifndef XBVM_SUPERIO_H
#define XBVM_SUPERIO_H

#include "HostDevice.h"

class Pit : public HostDevice
{
public:
	Pit();
	virtual ~Pit();

	virtual const char* GetName( void ) { return "Programmable Interval Timer"; }

	virtual bool IoWrite8( u16 port, u8 &data );
private:
	bool WriteToMode( u8 data );

	bool m_highLowToggle;
	u16 m_chan0Ctc;
};

extern Pit *g_pit;

#endif /*XBVM_SUPERIO_H*/

