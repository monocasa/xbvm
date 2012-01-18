#ifndef XBVM_IDE_DISKIMAGE_H
#define XBVM_IDE_DISKIMAGE_H

#include "Types.h"

class DiskImage 
{
public:
	DiskImage() :
	  m_imageSize( 0 )
	{
	}

	virtual ~DiskImage() {}

	virtual bool FillBuffer( u8* buffer, u64 offset, int size ) = 0;

	u64 GetImageSize() { return m_imageSize; }

protected:
	u64 m_imageSize;
};

#endif /*XBVM_IDE_DISKIMAGE_H*/

