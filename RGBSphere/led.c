/*
 * led.c
 *
 * Created: 2020-12-09 6:46:48 PM
 *  Author: charl
 */ 

#include "led.h"

#include "gpio.h"
#include "HardwareDescriptor.h"

#include <avr/io.h>
#include <avr/interrupt.h>

uint8_t ucPwmCounter = 0;

Color_t xCurrentColor;

void vLedInit(void) {
	GPIO_CLRPIN(GPIO_PORT_LED_RED, GPIO_PIN_LED_RED);
	GPIO_CLRPIN(GPIO_PORT_LED_GREEN, GPIO_PIN_LED_GREEN);
	GPIO_CLRPIN(GPIO_PORT_LED_BLUE, GPIO_PIN_LED_BLUE);
	GPIO_CLRPIN(GPIO_PORT_LED_WHITE, GPIO_PIN_LED_WHITE);
	
	GPIO_SETOUT(GPIO_PORT_LED_RED, GPIO_PIN_LED_RED);
	GPIO_SETOUT(GPIO_PORT_LED_GREEN, GPIO_PIN_LED_GREEN);
	GPIO_SETOUT(GPIO_PORT_LED_BLUE, GPIO_PIN_LED_BLUE);
	GPIO_SETOUT(GPIO_PORT_LED_WHITE, GPIO_PIN_LED_WHITE);
	
	TCA0.SINGLE.CTRLA = TCA_SINGLE_CLKSEL_DIV64_gc | TCA_SINGLE_ENABLE_bm;
	TCA0.SINGLE.INTCTRL = TCA_SINGLE_OVF_bm;
	TCA0.SINGLE.PER = 0x001;
	
	xCurrentColor.raw = 0x00000000;
}

void vLedSetColorRaw(uint32_t colorRaw) {
	xCurrentColor.raw = colorRaw;
}

void vLedSetColors(uint8_t red, uint8_t green, uint8_t blue, uint8_t white) {
	xCurrentColor.red = red;
	xCurrentColor.green = green;
	xCurrentColor.blue = blue;
	xCurrentColor.white = white;
}

ISR(TCA0_OVF_vect) {
	TCA0.SINGLE.INTFLAGS = TCA_SINGLE_OVF_bm;
	
	ucPwmCounter++;
	
	if(xCurrentColor.red > ucPwmCounter) {
		GPIO_SETPIN(GPIO_PORT_LED_RED, GPIO_PIN_LED_RED);
		
	} else {
		GPIO_CLRPIN(GPIO_PORT_LED_RED, GPIO_PIN_LED_RED);
	}
	
	if(xCurrentColor.green > ucPwmCounter) {
		GPIO_SETPIN(GPIO_PORT_LED_GREEN, GPIO_PIN_LED_GREEN);
	} else {
		GPIO_CLRPIN(GPIO_PORT_LED_GREEN, GPIO_PIN_LED_GREEN);
	}
	
	if(xCurrentColor.blue > ucPwmCounter) {
		GPIO_SETPIN(GPIO_PORT_LED_BLUE, GPIO_PIN_LED_BLUE);
	} else {
		GPIO_CLRPIN(GPIO_PORT_LED_BLUE, GPIO_PIN_LED_BLUE);
	}
	
	if(xCurrentColor.white > ucPwmCounter) {
		GPIO_SETPIN(GPIO_PORT_LED_WHITE, GPIO_PIN_LED_WHITE);
	} else {
		GPIO_CLRPIN(GPIO_PORT_LED_WHITE, GPIO_PIN_LED_WHITE);
	}
}