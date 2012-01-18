#ifndef XBVM_SMBUSDEVICE_H
#define XBVM_SMBUSDEVICE_H

#include "Device.h"

class SmBusDevice : public Device
{
public:
	SmBusDevice() {}
	virtual ~SmBusDevice() {}

	virtual u8 GetAddr() = 0;

	virtual bool WriteCommand8( u8 command, u8 data );
	virtual bool WriteCommand16( u8 command, u16 data );

	virtual bool ReadCommand8( u8 command, u8 *data );
};

#endif /*XBVM_SMBUSDEVICE_H*/

