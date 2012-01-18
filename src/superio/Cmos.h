#ifndef XBVM_SUPERIO_CMOS_H
#define XBVM_SUPERIO_CMOS_H

#include "HostDevice.h"

class Cmos : public HostDevice
{
public:
	Cmos();
	virtual ~Cmos();

	virtual const char* GetName( void ) { return "CMOS"; }

	virtual bool IoRead8( u16 port, u8 &data );
	virtual bool IoWrite8( u16 port, u8 &data );

private:
	bool ProcessRead( u8 &data );
	bool ProcessWrite( u8 data );

	u8 m_reg;
	u8 m_statusRegA;
	u8 m_statusRegB;
	u8 m_statusRegC;
	u8 m_statusRegD;
};

extern Cmos *g_cmos;

#endif /*XBVM_SUPERIO_CMOS_H*/

