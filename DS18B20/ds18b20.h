/*
 * ds18b20.h
 * CPU: ATMega328P
 * F_CPU: 16MHz
 * Temperature sensor: DS18B20
 *
 * Created: 27.06.2017 22:25:01
 *  Author: Nikolay
 */ 

#ifndef DS18B20_H_
#define DS18B20_H_

#include "../constants.h"

// ============== DS18B20 Port settings ==============
#define OW_PIN									PD3
#define OW_PORT									PORTD
#define OW_DDR									DDRD
#define OW_IN									PIND
// ===================================================

#define ADDRESS_IN_EEPROM

#define OW_DS18B20_CODE							0x28

#define OW_POWER_LEASED_LINE					1
#define OW_POWER_FROM_DATA_LINE					0

// ROM
#define OW_ROM_CRC_CODE_SIZE					8
#define OW_ROM_SKIP								0xCC
#define OW_ROM_READ								0x33
#define OW_ROM_MATCH							0x55
#define OW_ROM_SEARCH							0xF0
#define OW_ROM_ALARM_SEARCH						0xEC


// Return code
#define OW_ROM_SEARCH_FINISHED					0x00
#define OW_ROM_SEARCH_FAILED					0xFF
#define OW_ROM_SEARCH_SUCCESSFUL				0x00
#define OW_ROM_SEARCH_CRC_ERROR					0x01

#define OW_SEARCH_FIRST							0xFF
#define OW_PRESENCE_ERROR						0xFF
#define OW_DATA_ERROR							0xFE
#define OW_LAST_DEVICE							0x00

// Convertation
#define OW_START_CONVERTATION					0x44

// Scratchpad
#define OW_SCRATCHPAD_READ						0xBE
#define OW_SCRATCHPAD_WRITE						0x4E
#define OW_SCRATCHPAD_COPY						0x48

#define OW_RACALL_E2							0xB8

#define REG_LSB									0
#define REG_MSB									1
#define REG_TH									2
#define REG_TL									3
#define REG_CONFIG								4

#define REG_CRC									8

#define PRECISION_0_5							0x1F
#define PRECISION_0_25							0x3F
#define PRECISION_0_125							0x5F
#define PRECISION_0_0625						0x7F

struct OW_Device
{
	uint8_t id[8];
};

void StartConvertTempForAllSensor();
bool GetStatusDevice1(uint8_t status[ 9 ]);
bool GetStatusDevice2(uint8_t sensor_address[ 8 ], uint8_t status[ 9 ]);
void GetTemperature(uint8_t LSB, uint8_t MSB, bool* _sign, uint8_t* mantis, uint8_t* exp);

#ifndef ADDRESS_IN_EEPROM
	uint8_t InitDevice(uint8_t sensor_address[8], uint8_t TH, uint8_t TL, uint8_t CONFIG);
	bool GetStatusDevice(uint8_t sensor_address[8], scratch_pad_struct* status);
#endif


#endif /* DS18B20_H_ */