#ifndef DS1337_H_
#define DS1337_H_

#include <avr/io.h>

void rtc_init(void (*)(uint8_t*));
void rtc_setTime(uint8_t h, uint8_t m, uint8_t s, void (*)(uint8_t*));
void rtc_setDate(uint8_t y, uint8_t m, uint8_t d, void (*)(uint8_t*));
void rtc_getTime(void (*)(uint8_t*));
void rtc_getDate(void (*)(uint8_t*));
void rtc_getStatus(void (*)(uint8_t*));

#endif /* DS1337_H_ */