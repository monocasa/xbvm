#ifndef XBVM_SMBUS_CONEXANTENCODER_H
#define XBVM_SMBUS_CONEXANTENCODER_H

#include <cstring>

#include "smbus/SmBusDevice.h"

class ConexantEncoder : public SmBusDevice
{
public:
	ConexantEncoder() { memset( m_registers, 0, sizeof(m_registers) ); }
	virtual ~ConexantEncoder() {}
	
	virtual const char* GetName() { return "Conexant Encoder"; }

	virtual bool ReadCommand8( u8 command, u8 *data );
	virtual bool WriteCommand8( u8 command, u8 data );

	virtual u8 GetAddr() { return 0x45; }
private:
	u8 m_registers[256];
};

extern ConexantEncoder *g_conexantEncoder;

#endif /*XBVM_SMBUS_CONEXANTENCODER_H*/

