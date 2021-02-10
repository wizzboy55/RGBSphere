/*
 * necir.h
 *
 * Created: 2020-11-29 6:06:35 PM
 *  Author: charl
 */ 


#ifndef NECIR_H_
#define NECIR_H_

#include <tinylibdef.h>

typedef struct __attribute__((packed)) {
	union {
		uint32_t raw;
		struct {
			uint8_t data;
			uint8_t dataInv;
			uint8_t address;
			uint8_t addressInv;
		};	
	};
} NecIRMessage_t;

void vNecIRInit(void);

#endif /* NECIR_H_ */