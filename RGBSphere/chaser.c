/*
 * chaser.c
 *
 * Created: 2020-12-09 9:27:35 PM
 *  Author: charl
 */ 

#include "chaser.h"
#include "led.h"

#include "ColorPresets.h"
#include <avr/io.h>

ChaseMode_t xCurrentChaseMode = CHASEMODE_STATIC;
Color_t xCurrentStaticColor;

uint16_t usChaseCounter = 0;

uint8_t ucCurrentBrightness = 0xFF;

Color_t xCurrentChaserColor;

uint8_t ucCurrentFadeIndex = 0;

#define INCREMENT 25

#define FADE_INTERVAL		1000
#define FADE_PERIOD			151

uint8_t wave_sinepulseshort[] = {98,236,255,236,98,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
uint8_t wave_sinepulselong[] = {50,98,142,180,212,236,250,255,250,236,212,180,142,98,50,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
/*
uint32_t pColorPallette[] = {COLOR_REDORANGE, COLOR_LIGHTGREEN, COLOR_ROYALBLUE,
							COLOR_ORANGE, COLOR_CYAN, COLOR_DEEPPURPLE,
							COLOR_LIGHTORANGE, COLOR_TEAL, COLOR_PURPLE,
							COLOR_YELLOW, COLOR_DEEPBLUE, COLOR_PINK};
*/
Color_t ulChaserApplyBrightness(Color_t xColorIn, uint8_t brightness) {
	if(brightness != 0xFF) {
		Color_t xColorOut;
		xColorOut.red = (uint8_t)(((uint32_t)xColorIn.red * (uint32_t)brightness) / 255);
		xColorOut.green = (uint8_t)(((uint32_t)xColorIn.green * (uint32_t)brightness) / 255);
		xColorOut.blue = (uint8_t)(((uint32_t)xColorIn.blue * (uint32_t)brightness) / 255);
		xColorOut.white = (uint8_t)(((uint32_t)xColorIn.white * (uint32_t)brightness) / 255);
		return xColorOut;
	}
	
	return xColorIn;
}

Color_t WheelRainbow(uint8_t WheelPos) {
	Color_t color;
	WheelPos = 255 - WheelPos;
	if(WheelPos < 85) {
		color.red = 255 - WheelPos * 3;
		color.green = 0;
		color.blue = WheelPos * 3;
		return color;
	}
	if(WheelPos < 170) {
		WheelPos -= 85;
		color.red = 0;
		color.green = WheelPos * 3;
		color.blue = 255 - WheelPos * 3;
		return color;
	}
	WheelPos -= 170;
	color.red = WheelPos * 3;
	color.green = 255 - WheelPos * 3;
	color.blue = 0;
	return color;
}
/*
Color_t xChaserFade(Color_t xColorIn, Color_t xColorOut, uint8_t step, uint8_t period) {
	Color_t xOutputColor;
	uint8_t counterStep = period - step;
	xOutputColor.red = (uint8_t)((((uint16_t)xColorIn.red * step) / period) + ((xColorOut.red * counterStep) / period));
	xOutputColor.green = (uint8_t)((((uint16_t)xColorIn.green * step) / period) + ((xColorOut.green * counterStep) / period));
	xOutputColor.blue = (uint8_t)((((uint16_t)xColorIn.blue * step) / period) + ((xColorOut.blue * counterStep) / period));
	xOutputColor.white = (uint8_t)((((uint16_t)xColorIn.white * step) / period) + ((xColorOut.white * counterStep) / period));
	
	return xOutputColor;
}*/

void vChaserInit(void) {
	xCurrentChaserColor.raw = COLOR_BLACK;
}

void vChaserExec(void) {
	usChaseCounter++;	
	
	if(xCurrentChaseMode == CHASEMODE_STATIC) {
		xCurrentChaserColor = xCurrentStaticColor;
	} else if(xCurrentChaseMode == CHASEMODE_RAINBOW) {
		xCurrentChaserColor = WheelRainbow((uint8_t)(usChaseCounter >> 2));
	} else if(xCurrentChaseMode == CHASEMODE_FLASH) {
		xCurrentChaserColor = ulChaserApplyBrightness(xCurrentStaticColor, wave_sinepulselong[(usChaseCounter) % sizeof(wave_sinepulselong)]);
	} else if(xCurrentChaseMode == CHASEMODE_STROBE) {
		xCurrentChaserColor = ulChaserApplyBrightness(xCurrentStaticColor, wave_sinepulseshort[(usChaseCounter) % sizeof(wave_sinepulseshort)]);
	} else if(xCurrentChaseMode == CHASEMODE_FADE) {
		if((usChaseCounter % FADE_INTERVAL) < FADE_PERIOD) {
			ucCurrentFadeIndex++;
		}
		xCurrentChaserColor = WheelRainbow(ucCurrentFadeIndex);
	}
	
	vLedSetColorRaw(ulChaserApplyBrightness(xCurrentChaserColor, ucCurrentBrightness).raw);
}

void vChaserSetStaticColor(uint32_t rawColor) {
	xCurrentStaticColor.raw = rawColor;
	xCurrentChaseMode = CHASEMODE_STATIC;
}

void vChaserIncreaseBrightness(void) {
/*	if(ucCurrentBrightness < (0xFF - INCREMENT)) {
		ucCurrentBrightness += INCREMENT;
	} else {
		ucCurrentBrightness = 0xFF;
	}*/
}

void vChaserDecreaseBrightness(void) {
/*	if(ucCurrentBrightness > INCREMENT) {
		ucCurrentBrightness -= INCREMENT;
	} else {
		ucCurrentBrightness = 0x00;
	}*/
}

void vChaserSetFlash(void) {
	xCurrentChaseMode = CHASEMODE_FLASH;
}

void vChaserSetStrobe(void) {
	xCurrentChaseMode = CHASEMODE_STROBE;
}

void vChaserSetFade(void) {
	xCurrentChaseMode = CHASEMODE_FADE;
}

void vChaserSetSmooth(void) {
	xCurrentChaseMode = CHASEMODE_RAINBOW;	
}