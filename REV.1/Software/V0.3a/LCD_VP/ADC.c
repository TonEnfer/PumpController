/*
 * ADC.c
 *
 * Created: 13.08.2015 0:47:28
 *  Author: tonik_000
 */ 

#include "ADC.h"

void ADC_Init(void)
{
	ADMUX = 0b01000001;
	ADCSRA = (1<<ADEN)|(0<<ADSC)|(0<<ADATE)|(0<<ADIE)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
	ADCSRB |= (1 << MUX5);
}

