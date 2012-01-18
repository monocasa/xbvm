#ifndef XBVM_HOSTDEVICE_H
#define XBVM_HOSTDEVICE_H

#include "Device.h"

class HostDevice : public Device
{
public:
	HostDevice() {}
	virtual ~HostDevice() {}

	virtual bool IoRead32( u16 port, u32 &data );
	virtual bool IoRead16( u16 port, u16 &data );
	virtual bool IoRead8( u16 port, u8 &data );

	virtual bool IoWrite32( u16 port, u32 &data );
	virtual bool IoWrite16( u16 port, u16 &data );
	virtual bool IoWrite8( u16 port, u8 &data );

	virtual bool MmioWrite32( u32 addr, u32 data );
	virtual bool MmioWrite16( u32 addr, u16 data );
	virtual bool MmioWrite8( u32 addr, u8 data );

	virtual u32 MmioRead32( u32 addr );
	virtual u16 MmioRead16( u32 addr );
	virtual u8 MmioRead8( u32 addr );
};

#endif /*XBMV_HOSTDEVICE_H*/

