/*
 * ds18b20.c
 * CPU: ATMega328P
 * F_CPU: 16MHz
 * Temperature sensor: DS18B20
 *
 */ 

#include "../pins.h"
#include "ds18b20.h"

uint8_t crc8( const uint8_t * addr, uint8_t len )
{
	uint8_t crc = 0;
	while ( len-- )
	{
		uint8_t inbyte = *addr++;
		for ( uint8_t i = 8; i; i-- )
		{
			uint8_t mix = ( ( crc ^ inbyte ) & 0x01 );
			crc >>= 1;
			if ( mix )
			{
				crc ^= 0x8C;
			}

			inbyte >>= 1;
		}
	}

	return crc;
}

uint8_t Reset( void )
{
	uint8_t Retries = 128;
	PIN_MODE_INPUT(OW_PORT, OW_DDR, OW_PIN);
	
	do {
		if ( --Retries == 0 )
		{
			return 0;
		}

		_delay_us( 2 );
	} while ( ! PIN_READ(OW_IN, OW_PIN) );
	
	PIN_WRITE( OW_PORT, OW_PIN, LOW_LEVEL);
	PIN_MODE_OUTPUT(OW_DDR, OW_PIN);
	_delay_us( 480 );
	
	PIN_MODE_INPUT(OW_PORT, OW_DDR, OW_PIN);
	_delay_us( 70 );
	
	uint8_t state = !PIN_READ(OW_IN, OW_PIN);
	_delay_us( 410 );

	return state;
}

void Write_Bit( uint8_t bit )
{
	PIN_WRITE( OW_PORT, OW_PIN, LOW_LEVEL);
	PIN_MODE_OUTPUT(OW_DDR, OW_PIN);

	if( bit & 1 )
	{
		_delay_us( 10 );
		PIN_WRITE( OW_PORT, OW_PIN, HIGH_LEVEL);
		_delay_us( 55 );
	}
	else
	{
		_delay_us( 65 );
		PIN_WRITE( OW_PORT, OW_PIN, HIGH_LEVEL);
		_delay_us( 5 );
	}
}

uint8_t Read_Bit( void )
{
	PIN_MODE_OUTPUT(OW_DDR, OW_PIN);
	PIN_WRITE( OW_PORT, OW_PIN, LOW_LEVEL);
	_delay_us( 3 );
	
	PIN_MODE_INPUT(OW_PORT, OW_DDR, OW_PIN);
	_delay_us( 10 );
	
	uint8_t bit = PIN_READ(OW_IN, OW_PIN);
	_delay_us( 53 );

	return bit;
}

void Write_Byte( const uint8_t byte )
{
	for( uint8_t mask = 0x01; mask; mask <<= 1 )
	{
		Write_Bit( ( mask & byte ) ? 1 : 0 );
	}
}

uint8_t Read_Byte( void )
{
	uint8_t byte = 0;
	
	for( uint8_t mask = 0x01; mask; mask <<= 1 )
	{
		if( Read_Bit( ) )
		{
			byte |= mask;
		}
	}

	return byte;
}

void Read( uint8_t * buffer, uint8_t size )
{
	for( uint8_t i = 0; i < size; i++ )
	{
		buffer[ i ] = Read_Byte( );
	}
}

void Skip_Rom( void )
{
	Write_Byte( OW_ROM_SKIP );
}

void Match_ROM( uint8_t *buffer )
{
	if ( !Reset( ) )
	{
		return;
	}

	Write_Byte( OW_ROM_MATCH );

	for( uint8_t i = 0; i < 8; i++ )
	{
		Write_Byte( buffer[ i ] );
	}
}

void StartConvertTempForAllSensor( void )
{
	Reset( );
	Skip_Rom( );
	Write_Byte( OW_START_CONVERTATION );
}

bool GetStatusDevice1( uint8_t status[ 9 ] )
{
	Reset( );
	Skip_Rom( );
	Write_Byte( OW_SCRATCHPAD_READ );
	Read( status, sizeof( uint8_t ) * 9 );
	Reset( );

	if( crc8( status, OW_ROM_CRC_CODE_SIZE ) != status[ REG_CRC ] )
	{
		return false;
	}

	return true;
}

bool GetStatusDevice2( uint8_t sensor_address[ 8 ], uint8_t status[ 9 ] )
{
	uint8_t ROM[ 9 ];
	Reset( );
	Match_ROM( sensor_address );
	Write_Byte( OW_SCRATCHPAD_READ );
	Read( ROM, sizeof( ROM ) );
	Reset( );

	for( int i = 0; i < 9; i++ )
	{
		status[ i ] = ROM[ i ];
	}
	
	if( crc8( status, OW_ROM_CRC_CODE_SIZE ) != status[ REG_CRC ] )
	{
		return false;
	}

	return true;
}

void GetTemperature( uint8_t LSB, uint8_t MSB, bool* _sign, uint8_t* mantis, uint8_t* exp )
{
	*_sign = ( MSB & ( 0x01 << 4 ) );
	uint8_t m = 0;
	
	m = ( LSB >> 4 );
	m |= ( MSB << 4 );

	if( LSB & ( 0x01 << 3 ) )
	{
		*exp = 5;
	}
	else
	{
		*exp = 0;
	}

	if( *_sign )
	{
		m = ~ m;
		*mantis = m;
	}
	else
	{
		*mantis = m;
	}
}

#ifndef ADDRESS_IN_EEPROM
struct scratch_pad_struct
{
	uint8_t temp_lsb;
	uint8_t temp_msb;
	int8_t temp_high;
	uint8_t temp_low;
	uint8_t conf_register;
	uint8_t res1;
	uint8_t res2;
	uint8_t res3;
	uint8_t crc;
} scratch_pad;

void Read( scratch_pad_struct& data, uint8_t size )
{
	uint8_t* ptr = ( uint8_t* ) &data;
	for( uint8_t i = 0; i < size; i++ )
	{
		*( ptr + i ) = Read_Byte( );
	}
}

uint8_t Search_Rom( uint8_t * bitPattern, uint8_t lastDeviation )
{
	uint8_t currentBit = 1;
	uint8_t newDeviation = 0;
	uint8_t bitMask = 0x01;
	uint8_t bitA;
	uint8_t bitB;

	Write_Byte( OW_ROM_SEARCH );
	
	while ( currentBit <= 64 )
	{
		bitA = Read_Bit( );
		bitB = Read_Bit( );

		if ( bitA && bitB )
		{
			newDeviation = OW_ROM_SEARCH_FAILED;
			return -1;
		}
		else if ( bitA ^ bitB )
		{
			if ( bitA )
			{
				( *bitPattern ) |= bitMask;
			}
			else
			{
				( *bitPattern ) &= ~bitMask;
			}
		}
		else
		{
			if ( currentBit == lastDeviation )
			{
				( *bitPattern ) |= bitMask;
			}
			else if ( currentBit > lastDeviation )
			{
				( *bitPattern ) &= ~bitMask;
				newDeviation = currentBit;
			}
			else if ( !( *bitPattern & bitMask ) )
			{
				newDeviation = currentBit;
			}
			else
			{
				
			}
		}
		
		if ( ( *bitPattern ) & bitMask )
		{
			Write_Bit( 1 );
		}
		else
		{
			Write_Bit( 0 );
		}

		currentBit++;

		bitMask <<= 1;
		if ( !bitMask )
		{
			bitMask = 0x01;
			bitPattern++;
		}
	}

	return newDeviation;
}

uint8_t Search_Devices( struct OW_Device *devices, uint8_t len )
{
	uint8_t presence;
	uint8_t *newID;
	uint8_t *currentID;
	uint8_t currentBus;
	uint8_t lastDeviation;
	uint8_t numDevices;
	
	for ( uint8_t i = 0; i < len; i++ )
	{
		for ( uint8_t j = 0; j < 8; j++ )
		{
			devices[ i ].id[ j ] = 0x00;
		}
	}
	
	presence = Reset( );
	
	numDevices = 0;
	newID = devices[ 0 ].id;
	
	for ( currentBus = 0x01; currentBus; currentBus <<= 1 )
	{
		lastDeviation = 0;
		currentID = newID;
		if ( currentBus & presence )
		{
			do
			{
				memcpy( newID, currentID, 8 );
				Reset();
				lastDeviation = Search_Rom( newID, lastDeviation );
				currentID = newID;
				numDevices++;
				newID = devices[ numDevices ].id;
			}  while( lastDeviation != OW_ROM_SEARCH_FINISHED );
		}
	}

	for ( uint8_t i = 0; i < numDevices; i++ )
	{
		if( crc8( devices[ i ].id, 8 ) != 0 )
		{
			return OW_ROM_SEARCH_CRC_ERROR;
		}
	}

	return numDevices;
}

uint8_t InitDevice( uint8_t sensor_address[8], uint8_t TH, uint8_t TL, uint8_t CONFIG )
{
	Match_ROM( sensor_address );
	Write_Byte( OW_SCRATCHPAD_WRITE );
	Write_Byte( TH );
	Write_Byte( TL );
	Write_Byte( CONFIG );
	GetStatusDevice( sensor_address, scratch_pad );
	if ( scratch_pad.temp_high != TH || scratch_pad.temp_low != TL || scratch_pad.conf_register != CONFIG )
	{
		return -1;
	}

	Match_ROM( sensor_address );
	Write_Byte( OW_SCRATCHPAD_COPY );
	_delay_ms( 15 );
	return Reset( );
}

bool GetStatusDevice( uint8_t sensor_address[ 8 ], scratch_pad_struct* status )
{
	Reset( );
	Match_ROM( sensor_address );
	Write_Byte( OW_SCRATCHPAD_READ );
	Read( *status, sizeof( *status ) / sizeof( uint8_t ) );
	Reset( );

	if( crc8( ( uint8_t* ) status, OW_ROM_CRC_CODE_SIZE ) != status->crc )
	{
		return false;
	}

	return true;
}
#endif