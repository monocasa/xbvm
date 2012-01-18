#ifndef XBVM_IDE_DVDDRIVE_H
#define XBVM_IDE_DVDDRIVE_H

#include "ide/IdeDevice.h"

class DvdDrive : public IdeDevice
{
public:
	DvdDrive() : IdeDevice()
	{
		m_sectorSize = 2048;
	}

	virtual ~DvdDrive() {}

	virtual const char* GetName() { return "IDE DVD Drive"; }

	virtual void OnAtapiSoftReset();
	virtual void OnIdentifyPacketDevice();

	void PopulateIdSector( u16 *buffer );
};

#endif /*XBVM_IDE_DVDDRIVE_H*/

