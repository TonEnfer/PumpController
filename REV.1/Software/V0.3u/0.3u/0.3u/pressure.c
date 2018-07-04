/*
 * pressure.c
 *
 * Created: 13.08.2015 17:25:50
 *  Author: tonik_000
 */ 

#include "pressure.h"




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


