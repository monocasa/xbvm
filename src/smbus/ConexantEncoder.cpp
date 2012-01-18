#include "smbus/ConexantEncoder.h"

ConexantEncoder *g_conexantEncoder;

bool
ConexantEncoder::ReadCommand8( u8 command, u8 *data )
{
	*data = m_registers[ command ];
	return true;
}

bool
ConexantEncoder::WriteCommand8( u8 command, u8 data )
{
	m_registers[ command ] = data;
	return true;
}

