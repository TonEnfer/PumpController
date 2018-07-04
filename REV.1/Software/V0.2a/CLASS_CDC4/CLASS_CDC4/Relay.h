/*
 * Relay.h
 *
 * Created: 13.08.2015 0:33:03
 *  Author: tonik_000
 */ 


#ifndef RELAY_H_
#define RELAY_H_

#include "VirtualSerial.h"

#define relay1_DDR DDRD
#define relay1_PORT PORTD
#define relay1_PIN 3
#define relay1_Init() sbi(relay1_DDR,relay1_PIN)
#define relay1_On() sbi(relay1_PORT,relay1_PIN)
#define relay1_Off() cbi(relay1_PORT,relay1_PIN)
#define relay1_IsOn() bit_is_set(relay1_PORT,relay1_PIN)

#define relay2_DDR DDRD
#define relay2_PORT PORTD
#define relay2_PIN 5
#define relay2_Init() sbi(relay2_DDR,relay2_PIN)
#define relay2_On() sbi(relay2_PORT,relay2_PIN)
#define relay2_Off() cbi(relay2_PORT,relay2_PIN)
#define relay2_IsOn() bit_is_set(relay2_PORT,relay2_PIN)

#endif /* RELAY_H_ */