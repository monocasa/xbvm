#ifndef XBVM_IDE_NULLDISKIMAGE_H
#define XBVM_IDE_NULLDISKIMAGE_H

#include <cstring>

#include "ide/DiskImage.h"

class NullDiskImage : public DiskImage
{
public:
	NullDiskImage( u64 imageSize ) : DiskImage()
	{
		m_imageSize = imageSize;
	}

	virtual bool FillBuffer( u8 *buffer, u64 offset, int size )
	{
		if( (offset + size) > m_imageSize ) {
			return false;
		}
		memset( buffer, 0, size );
		if( offset == 0x600 ) {
			buffer[ 0 ] = 'B';
			buffer[ 1 ] = 'R';
			buffer[ 2 ] = 'F';
			buffer[ 3 ] = 'R';
		}
		return true;
	}
};

#endif /*XBVM_IDE_NULLDISKIMAGE_H*/

