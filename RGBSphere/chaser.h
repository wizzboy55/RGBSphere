/*
 * chaser.h
 *
 * Created: 2020-12-09 9:27:40 PM
 *  Author: charl
 */ 


#ifndef CHASER_H_
#define CHASER_H_

#include "tinylibdef.h"

typedef enum {CHASEMODE_STATIC, CHASEMODE_RAINBOW, CHASEMODE_STROBE, CHASEMODE_FLASH, CHASEMODE_FADE} ChaseMode_t;

void vChaserInit(void);
void vChaserExec(void);
void vChaserSetStaticColor(uint32_t rawColor);

void vChaserIncreaseBrightness(void);
void vChaserDecreaseBrightness(void);

void vChaserSetFlash(void);
void vChaserSetStrobe(void);
void vChaserSetFade(void);
void vChaserSetSmooth(void);

#endif /* CHASER_H_ */