#ifndef _CLOCK_H_
#define _CLOCK_H_
#include "VirtualSerial.h"

void clock_init();
int32_t millis();
int64_t micros();
void delay(int32_t ms);
void delayMicroseconds(int64_t time);

#endif
