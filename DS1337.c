#include "DS1337.h"

#include "twi.h"

#define DEVICE_ID 0x68

uint8_t toBcd(uint8_t value)
{
	return ((value / 10) << 4) + (value % 10);
}

//uint8_t fromBcd(uint8_t value)
//{
	//return (value >> 4) * 10 + (value & 0x0F);
//}

//void rtc_init(void (*callbackFunction)(uint8_t*))
//{
	//twi_init(100000L);
	//uint8_t data[1] = { 0b00000100 };
	//twi_write(DEVICE_ID, 0x0E, data, 1, callbackFunction);
//}

void rtc_setTime(uint8_t h, uint8_t m, uint8_t s, void (*callback)(uint8_t*))
{
	if(s < 0 || s > 59
	|| m < 0 || m > 59
	|| h < 0 || h > 23) {
		callback(0);
	} else {
		uint8_t data[3] = { toBcd(s), toBcd(m), toBcd(h) };
		twi_write(DEVICE_ID, 0x00, data, 3, callback);
	}
}

void rtc_setDate(uint8_t y, uint8_t m, uint8_t d, void (*callback)(uint8_t*))
{
	if(d < 0 || d > 31
	|| m < 1 || m > 12
	|| y < 0 || y > 99) {
		callback(0);
	} else {
		uint8_t data[3] = { toBcd(d), toBcd(m), toBcd(y) };
		twi_write(DEVICE_ID, 0x04, data, 3, callback);
	}
}

void rtc_getTime(void (*callback)(uint8_t *))
{
	twi_read(DEVICE_ID, 0x00, 3, callback);
}

void rtc_getDate(void (*callback)(uint8_t *))
{
	twi_read(DEVICE_ID, 0x04, 3, callback);
}

void rtc_getStatus(void (*callback)(uint8_t *))
{
	twi_read(DEVICE_ID, 0x0F, 1, callback);
}