/*
 * buttons.h
 *
 * Created: 13.08.2015 17:45:43
 *  Author: tonik_000
 */ 


#ifndef BUTTONS_H_
#define BUTTONS_H_

#include "VirtualSerial.h"

#define but_minus_DDR DDRB
#define but_minus_PORT PORTB
#define but_minus_PINS PINB
#define but_minus_PIN 6

#define but_plus_DDR DDRC
#define but_plus_PORT PORTC
#define but_plus_PINS PINC
#define but_plus_PIN 6

#define but_ok_DDR DDRC
#define but_ok_PORT PORTC
#define but_ok_PINS PINC
#define but_ok_PIN 7

#define delay_but 150

#define but_plus_Init() do{cbi(but_plus_DDR,but_plus_PIN);sbi(but_plus_PORT,but_plus_PIN);}while(0)
#define but_ok_Init() do{cbi(but_ok_DDR,but_ok_PIN);sbi(but_ok_PORT,but_ok_PIN);}while(0)
#define but_minus_Init() do{cbi(but_minus_DDR,but_minus_PIN);sbi(but_minus_PORT,but_minus_PIN);}while(0)

#define but_Init() do{but_plus_Init();but_ok_Init();but_minus_Init();}while(0)

#define but_plus_IsSet() bit_is_clear(but_plus_PINS,but_plus_PIN)
#define but_ok_IsSet() bit_is_clear(but_ok_PINS,but_ok_PIN)
#define but_minus_IsSet() bit_is_clear(but_minus_PINS,but_minus_PIN)


#endif /* BUTTONS_H_ */