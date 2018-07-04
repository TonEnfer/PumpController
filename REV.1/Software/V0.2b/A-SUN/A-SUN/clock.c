#include "clock.h"
static volatile int64_t microtime =0;
void clock_init(){
	//TODO: this might slow down, due to the fact that each 256 instruction we run interrupt
    sbi(TCCR3B, _BV(CS30));    
    sbi(TIMSK3, _BV(OCIE3A));
	OCR3A = 16;
    sei();
}

ISR(TIMER3_COMPA_vect){
    microtime+=1;
}

int32_t millis(){
    return microtime/1000;
}
int64_t micros(){
    return microtime|TCNT1;
}
void delay(int32_t ms){
    int64_t cur = millis();
    while(millis()-cur<ms);
}
void delayMicroseconds(int64_t time){
    int64_t cur = micros();
    while(micros()-cur<time);
}
