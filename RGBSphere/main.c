/*
 * RGBSphere.c
 *
 * Created: 2020-11-29 3:37:18 PM
 * Author : charl
 */ 

#include <avr/io.h>

#include "gpio.h"
#include "HardwareDescriptor.h"
#include <util/delay.h>
#include "clockcontroller.h"
#include "necir.h"
#include "led.h"
#include "chaser.h"
#include "wdt.h"

#define FRAMES_PER_SECONDS	45
#define FRAME_PERIOD_MS		1000 / FRAMES_PER_SECONDS

int main(void)
{
	vTinyMainClockSetup(eDivider2);
	
	vLedInit();
	
	vChaserInit();
	
	vNecIRInit();
	
	vChaserInit();
	
	ENABLE_INTERRUPTS();
	
	TINYWATCHDOG_ENABLE(WDT_PERIOD_8KCLK_gc);
	
    while (1) 
    {
		_delay_ms(FRAME_PERIOD_MS);
		TINYWATCHDOG_RESET();
		vChaserExec();
    }
}

