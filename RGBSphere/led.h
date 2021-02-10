/*
 * led.h
 *
 * Created: 2020-12-09 6:46:56 PM
 *  Author: charl
 */ 


#ifndef LED_H_
#define LED_H_

#include "tinylibdef.h"

typedef struct {
	union {
		struct {
			uint8_t red;
			uint8_t green;
			uint8_t blue;
			uint8_t white;
		};
		uint32_t raw;
	};
} Color_t;

void vLedInit(void);
void vLedSetColorRaw(uint32_t colorRaw);
void vLedSetColors(uint8_t red, uint8_t green, uint8_t blue, uint8_t white);

#endif /* LED_H_ */