#include "clock.h"
static volatile int64_t microtime = 0;
void clock_init(){
	//TODO: this might slow down, due to the fact that each 256 instruction we run interrupt
    sbi(TCCR0B, _BV(CS00));    
    sbi(TIMSK0, _BV(TOIE0));
    sei();
}

ISR(TIMER0_OVF_vect){
    microtime+=256;
}

int32_t millis(){
    return microtime/1000;
}
int64_t micros(){
    return microtime|TCNT0;
}
void delay(int32_t ms){
    int64_t cur = millis();
    while(millis()-cur<ms);
}
void delayMicroseconds(int64_t time){
    int64_t cur = micros();
    while(micros()-cur<time);
}
