/*
 * pressure.h
 *
 * Created: 13.08.2015 17:25:41
 *  Author: tonik_000
 */ 


#ifndef PRESSURE_H_
#define PRESSURE_H_

#include "compilers_4.h"
#include "ADC.h"
#include "lcd_lib_2.h"
#include "buttons.h"

#define ConvToAtm(pressure) ((((float)pressure / 204.8 - 0.2) / 0.006429) * 0.00986923267)

uint16_t middle_of_3(uint16_t a, uint16_t b, uint16_t c);
uint16_t pressure_midle ();
void Set_Pressure (uint16_t _param,uint16_t _menu,uint16_t _old_menu);


#endif /* PRESSURE_H_ */