/*
 * necir.c
 *
 * Created: 2020-11-29 6:06:42 PM
 *  Author: charl
 */ 

#include "necir.h"

#include <avr/interrupt.h>
#include <avr/io.h>
#include "gpio.h"

#include "led.h"
#include "SmallRemoteCommands.h"
#include "ColorPresets.h"
#include "chaser.h"

#include "HardwareDescriptor.h"

#define CPU_FREQUENCY 16000000 / 2

uint32_t ulIrData = 0;

uint16_t usCycleCount;

uint8_t ucBitCount = 0;

#define NECIR_BREAK_US	4500
#define NECIR_0_US		540
#define NECIR_1_US		1500

#define NECIR_COUNTER_BITPERIOD_US	8
#define NECIR_PULSE_TOLERANCE_BITS	50

NecIRMessage_t xIrMessage;

void vNecIRInit(void) {
	GPIO_SETUP_INPUT(GPIO_PORT_IR_INPUT, GPIO_PIN_IR_INPUT, pdFALSE, pdFALSE, PORT_ISC_INTDISABLE_gc);
	
	TCA0.SINGLE.CTRLA = TCA_SINGLE_CLKSEL_DIV64_gc | TCA_SINGLE_ENABLE_bm;

	TCB0.CTRLA = TCB_CLKSEL_CLKTCA_gc | TCB_ENABLE_bm;
	TCB0.CTRLB = TCB_CNTMODE_PW_gc;
	TCB0.EVCTRL = TCB_CAPTEI_bm;
	TCB0.INTCTRL = TCB_CAPT_bm;
	
	EVSYS.ASYNCCH0 = (EVSYS_ASYNCCH0_PORTA_PIN0_gc + GPIO_PIN_IR_INPUT);
	EVSYS.ASYNCUSER0 = EVSYS_ASYNCUSER0_ASYNCCH0_gc;
}

void vNecIrDecoder(NecIRMessage_t message) {
	switch(message.data) {
		case SMALLREMOTE_BRIGHTNESSUP:
			vChaserIncreaseBrightness();
			break;
		case SMALLREMOTE_BRIGHTNESSDOWN:
			vChaserDecreaseBrightness();
			break;
		case SMALLREMOTE_OFF:
			vChaserSetStaticColor(COLOR_BLACK);
			break;
		case SMALLREMOTE_RED:
			vChaserSetStaticColor(COLOR_RED);
			break;
		case SMALLREMOTE_GREEN:
			vChaserSetStaticColor(COLOR_GREEN);
			break;
		case SMALLREMOTE_BLUE:
			vChaserSetStaticColor(COLOR_BLUE);
			break;
		case SMALLREMOTE_WHITE:
			vChaserSetStaticColor(COLOR_WHITE);
			break;
		case SMALLREMOTE_REDORANGE:
			vChaserSetStaticColor(COLOR_REDORANGE);
			break;
		case SMALLREMOTE_LIGHTGREEN:
			vChaserSetStaticColor(COLOR_LIGHTGREEN);
			break;
		case SMALLREMOTE_ROYALBLUE:
			vChaserSetStaticColor(COLOR_ROYALBLUE);
			break;
		case SMALLREMOTE_FLASH:
			vChaserSetFlash();
			break;
		case SMALLREMOTE_ORANGE:
			vChaserSetStaticColor(COLOR_ORANGE);
			break;
		case SMALLREMOTE_CYAN:
			vChaserSetStaticColor(COLOR_CYAN);
			break;
		case SMALLREMOTE_DEEPPURPLE:
			vChaserSetStaticColor(COLOR_DEEPPURPLE);
			break;
		case SMALLREMOTE_STROBE:
			vChaserSetStrobe();
			break;
		case SMALLREMOTE_LIGHTORANGE:
			vChaserSetStaticColor(COLOR_LIGHTORANGE);
			break;
		case SMALLREMOTE_TEAL:
			vChaserSetStaticColor(COLOR_TEAL);
			break;
		case SMALLREMOTE_PURPLE:
			vChaserSetStaticColor(COLOR_PURPLE);
			break;
		case SMALLREMOTE_FADE:
			vChaserSetFade();
			break;
		case SMALLREMOTE_YELLOW:
			vChaserSetStaticColor(COLOR_YELLOW);
			break;
		case SMALLREMOTE_DEEPBLUE:
			vChaserSetStaticColor(COLOR_DEEPBLUE);
			break;
		case SMALLREMOTE_PINK:
			vChaserSetStaticColor(COLOR_PINK);
			break;
		case SMALLREMOTE_SMOOTH:
			vChaserSetSmooth();
			break;
		default:
			break;
	}
}

ISR(TCB0_INT_vect) {
	
	TCB0.INTFLAGS = TCB_CAPT_bm;
	
	usCycleCount = TCB0.CNT;
	
	if(ucBitCount == 0) {
		if((usCycleCount < (NECIR_BREAK_US / NECIR_COUNTER_BITPERIOD_US) + NECIR_PULSE_TOLERANCE_BITS) && (usCycleCount > (NECIR_BREAK_US / NECIR_COUNTER_BITPERIOD_US) - NECIR_PULSE_TOLERANCE_BITS)) {
			// Break detected
			ucBitCount++;
		}
	} else if(ucBitCount > 0) {
		if((usCycleCount < (NECIR_0_US / NECIR_COUNTER_BITPERIOD_US) + NECIR_PULSE_TOLERANCE_BITS) && (usCycleCount > (NECIR_0_US / NECIR_COUNTER_BITPERIOD_US) - NECIR_PULSE_TOLERANCE_BITS)) {
			// 1 detected
			ucBitCount++;
			ulIrData <<= 1;
		} else if((usCycleCount < (NECIR_1_US / NECIR_COUNTER_BITPERIOD_US) + NECIR_PULSE_TOLERANCE_BITS) && (usCycleCount > (NECIR_1_US / NECIR_COUNTER_BITPERIOD_US) - NECIR_PULSE_TOLERANCE_BITS)) {
			// 0 detected
			ucBitCount++;
			ulIrData <<= 1;
			ulIrData |= 1;
		}
	}
	
	if(ucBitCount == 33) {
		ucBitCount = 0;
		xIrMessage.raw = ulIrData;
		ulIrData++;
		if((xIrMessage.data == (xIrMessage.dataInv ^ 0xFF))) {
			// Valid Message Received
			vNecIrDecoder(xIrMessage);
		} else {
			// Invalid Message
		//	PORTA.OUT = 0x00;
		//	GPIO_SETPIN(GPIO_PORT_LED_RED, GPIO_PIN_LED_RED);
		}
	}
	
}