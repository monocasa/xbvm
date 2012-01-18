#ifndef XBVM_IDE_HARDDRIVE_H
#define XBVM_IDE_HARDDRIVE_H

#include "ide/IdeDevice.h"
#include "ide/NullDiskImage.h"

class HardDrive : public IdeDevice
{
public:
	HardDrive() : IdeDevice()
	{
		m_diskImage = new NullDiskImage( 19541088L * 512 );
		m_sectorSize = 512;
	}

	virtual ~HardDrive()
	{
		delete m_diskImage;
	}

	virtual const char* GetName() { return "IDE Hard Drive"; }

	virtual void OnIdentifyCommand();
	virtual void OnSecurityUnlockCommand();

	void PopulateIdSector( u16 *sector );
};

#endif /*XBVM_IDE_HARDDRIVE_H*/

