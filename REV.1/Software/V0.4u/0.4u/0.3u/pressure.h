/*
 * pressure.h
 *
 * Created: 13.08.2015 17:25:41
 *  Author: tonik_000
 */ 


#ifndef PRESSURE_H_
#define PRESSURE_H_

#include "VirtualSerial.h"
#include "ADC.h"
#include "lcd_lib_2.h"
#include "buttons.h"

#define window 16
#define del 136

#define ConvToAtm(pressure) ((float)pressure * 0.00749566 - 0.30702232602)

void pressure_Init();
uint16_t middle_of_3(uint16_t a, uint16_t b, uint16_t c);
uint16_t pressure_midle ();
uint16_t cur_pressure_SMA(uint16_t _current_pressure);


#endif /* PRESSURE_H_ */