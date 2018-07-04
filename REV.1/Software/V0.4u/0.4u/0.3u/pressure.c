/*
 * pressure.c
 *
 * Created: 13.08.2015 17:25:50
 *  Author: tonik_000
 */ 

#include "pressure.h"


static volatile uint16_t pressure_mass[window];
static volatile uint32_t pressure_temp = 0;

void pressure_Init()
{
	for (uint8_t i =0; i<window; i++)
	{
		pressure_mass[i] = pressure_midle();
	}
}

uint16_t middle_of_3(uint16_t a, uint16_t b, uint16_t c)
{
	uint16_t middle;
	
	if ((a <= b) && (a <= c)){
		middle = (b <= c) ? b : c;
	}
	else{
		if ((b <= a) && (b <= c)){
			middle = (a <= c) ? a : c;
		}
		else{
			middle = (a <= b) ? a : b;
		}
	}
	
	return middle;
}

uint16_t pressure_midle ()
{
	uint16_t pressure1[3];
	uint16_t pressure2[3];
	uint16_t pressure_mid;
	for (uint8_t i = 0; i<3;i++)
	{
		
		for (uint8_t i = 0; i<3;i++)
		{
			StartConvAdc();
			while (bit_is_set(ADCSRA, ADSC));
			uint8_t low  = ADCL;
			uint8_t high = ADCH;
			pressure1[i] = ((uint16_t)low)|((uint16_t)high<<8);
		}
		pressure2[i] = middle_of_3(pressure1[0],pressure1[1],pressure1[2]);
	}
	return pressure_mid = middle_of_3(pressure2[0],pressure2[1],pressure2[2]);
}

uint16_t cur_pressure_SMA(uint16_t _current_pressure)
{
	
	for (uint8_t i = 1; i < window; i++)
	{
		pressure_mass[i-1] = pressure_mass[i];
	}
	
	pressure_mass[window-1] = _current_pressure;
	
	pressure_temp = 0;
	
	for (uint8_t i = 0; i<window; i++)
	{
		pressure_temp+=(uint32_t)pressure_mass[i-1]*((uint32_t)(i+1));
	}
	return (uint16_t)((uint32_t)pressure_temp/(uint16_t)del);
}

