/*
 * TEST_PORT.cpp
 *
 * Created: 12.08.2015 21:56:18
 *  Author: tonik_000
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
	//disable JTAG in case it’s on
	uint8_t mcucp = MCUCR | (1 << JTD);
	//it takes two writes as per datasheet
	MCUCR = mcucp;
	MCUCR = mcucp;
	DDRF = 0xFF;
	PORTF = 0xFF;
    while(1)
    {
		
        //TODO:: Please write your application code 
    }
}