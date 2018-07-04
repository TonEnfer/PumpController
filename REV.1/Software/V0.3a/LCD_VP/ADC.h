/*
 * ADC.h
 *
 * Created: 13.08.2015 0:47:16
 *  Author: tonik_000
 */ 


#ifndef ADC_H_
#define ADC_H_

#include "compilers_4.h"

#define pressure_DDR DDRD
#define pressure_PORT PORTD
#define pressure_PIN 6



#define StartConvAdc()  ADCSRA |= (1<<ADSC)

void ADC_Init(void);





#endif /* ADC_H_ */