#ifndef XBVM_DEVICE_H
#define XBVM_DEVICE_H

#include "Types.h"

class Device
{
public:
	Device() {}
	virtual ~Device() {}
	virtual const char* GetName( void ) = 0;
};

#endif /*XBVM_DEVICE_H*/

