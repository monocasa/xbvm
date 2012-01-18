#ifndef XBVM_PIC_H
#define XBVM_PIC_H

#include "smbus/SmBusDevice.h"

class PIC16L : public SmBusDevice
{
public:
	PIC16L() :
	   m_version( VERSION_STRING_RETAIL_1_0 ),
	   m_versionCurrentOffset(0),
	   m_ledSequence( 0 ),
	   m_powerFanSpeed( 0 ),
	   m_resetOnEject( true ),
	   m_interruptEnabled( false ),
	   m_scratchBuffer( 0 ),
	   m_unknownReg0B( 0 ),
	   m_unknownReg12( 0 ),
	   m_unknownReg13( 0 )
	{
	}

	virtual ~PIC16L() {}

	virtual const char* GetName( void ) { return "PIC"; }

	virtual bool WriteCommand8( u8 command, u8 data );

	virtual bool ReadCommand8( u8 command, u8 *data );

	virtual u8 GetAddr( void ) { return 0x10; }
private:
	static const char *VERSION_STRING_DEBUG;			
	static const char *VERSION_STRING_RETAIL_1_0;
	static const char *VERSION_STRING_RETAIL_1_1;

	const char *m_version;
	int m_versionCurrentOffset;

	u8 m_ledSequence;
	u8 m_powerFanSpeed;

	bool m_resetOnEject;
	bool m_interruptEnabled;

	u8 m_scratchBuffer;

	u8 m_unknownReg0B;
	u8 m_unknownReg12;
	u8 m_unknownReg13;

	PIC16L( PIC16L& );
	const PIC16L& operator=( PIC16L& );

	void WriteVersion( u16 data );
	void WriteLEDSequence( u8 data );
};

extern PIC16L *g_pic16l;

#endif /*XBVM_PIC_H*/

