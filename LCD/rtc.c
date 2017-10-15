/*
 * rtc.c
 * CPU: ATMega328P
 * F_CPU: 16MHz
 * RTC: DS1307
 *
 * Created: 15.10.2017 21:44:52
 *  Author: Nikolay
 */ 

 #include "rtc.h"

 static const PROGMEM uint8_t daysinmonth []  = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

 void rtc_init( void )
 {
	 i2c_init();
	 _delay_us(10);

	 uint8_t value = 0;

	 value |= ( 1 << SQWE );
	 i2c_start( DS1307Z_WRITE );
	 
	 i2c_write( SQWE_OUT_REG );
	 i2c_write( value );
	 
	 i2c_stop();
 }

 uint8_t dec2bcd( uint8_t val )
 {
	 return ( val + 6 * ( val / 10 ) );
 }

 static uint8_t bcd2dec( uint8_t val )
 {
	 return ( val - 6 * ( val >> 4 ) );
 }

 static uint16_t date2days( uint8_t year, uint8_t month, uint8_t day )
 {
	 uint16_t days = day;
	 for ( uint8_t i = 1; i < month; ++i )
	 {
		 days += pgm_read_byte( daysinmonth + i - 1 );
	 }
	 
	 if ( month > 2 && year % 4 == 0 )
	 {
		 ++days;
	 }

	 return ( days + 365 * year + ( year + 3 ) / 4 - 1 );
 }

 uint8_t getdayofweek( uint8_t year, uint8_t month, uint8_t day )
 {
	 return ( ( date2days( year, month, day ) + 6 ) % 7 );
 }

 uint8_t rtc_setdate( struct tm* cur_time )
 {
	 if ( cur_time->tm_sec < 0 || cur_time->tm_sec > 59 ||
	 cur_time->tm_min < 0 || cur_time->tm_min > 59 ||
	 cur_time->tm_hour < 0 || cur_time->tm_hour > 23 ||
	 cur_time->tm_mday < 1 || cur_time->tm_mday > 31 ||
	 cur_time->tm_mon < 1 || cur_time->tm_mon > 12 ||
	 cur_time->tm_year < 0 || cur_time->tm_year > 99 )
	 {
		 return 8;
	 }

	 if( cur_time->tm_mday > pgm_read_byte( daysinmonth + cur_time->tm_mon - 1 ) )
	 {
		 return 1;
	 }

	 uint8_t dayofweek = getdayofweek( cur_time->tm_year, cur_time->tm_mon, cur_time->tm_mday );

	 i2c_start( DS1307Z_WRITE );
	 i2c_write( 0x00 );
	 i2c_write( dec2bcd( cur_time->tm_sec ) );
	 i2c_write( dec2bcd( cur_time->tm_min ) );
	 i2c_write( dec2bcd( cur_time->tm_hour ) );
	 i2c_write( dec2bcd( dayofweek ) );
	 i2c_write( dec2bcd( cur_time->tm_mday ) );
	 i2c_write( dec2bcd( cur_time->tm_mon ) );
	 i2c_write( dec2bcd( cur_time->tm_year ) );
	 i2c_write( 0x00 );
	 i2c_stop();

	 return 0;
 }

 void rtc_getdate( struct tm* cur_time )
 {
	 i2c_start( DS1307Z_WRITE );
	 i2c_write( 0x00 );
	 
	 i2c_start( DS1307Z_READ );
	 cur_time->tm_sec = bcd2dec( i2c_read_ack() & 0x7F );
	 cur_time->tm_min = bcd2dec( i2c_read_ack() );
	 cur_time->tm_hour = bcd2dec( i2c_read_ack() );
	 i2c_read_ack();
	 cur_time->tm_mday = bcd2dec( i2c_read_ack() );
	 cur_time->tm_mon = bcd2dec( i2c_read_ack() );
	 cur_time->tm_year = bcd2dec( i2c_read_nack() );
	 i2c_write( 0x00 );
	 i2c_stop();
 }