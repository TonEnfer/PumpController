/*
 * Relay.c
 *
 * Created: 13.08.2015 0:16:01
 *  Author: tonik_000
 */ 

#include "Relay.h"

inline void Relay1_Init(void)
{
	sbi(relay1_DDR,relay1_PIN);
}

inline void Relay2_Init(void)
{
	sbi(relay2_DDR,relay2_PIN);
}

inline void Relay1_On(void)
{
	sbi(relay1_PORT,relay1_PIN);
	
}

inline void Relay1_Off(void)
{
	cbi(relay1_PORT,relay1_PIN);
}

inline void Relay2_On(void)
{
	sbi(relay2_PORT,relay2_PIN);
}

inline void Relay2_Off(void)
{
	cbi(relay2_PORT,relay2_PIN);
}