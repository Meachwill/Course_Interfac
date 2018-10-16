#define F_CPU 11059200
#include <avr/io.h>
#include "lib/ASA_Lib.H"
#include <string.h>
#include <avr/iom128.h>

void TIM_SET();

ISR(TIMER0_COMP_vect){
    PORTA ^=0x01;

}
int main(){
    ASA_M128_set();
    DDRA |= ~(DDRA);
    TIM_SET();
    sei();
    while(1){
        
       //int a=1;
    }

}

void TIM_SET(){
    TIMSK |= (1<<OCIE0);
    TCCR0 |= (1 << WGM01) | (1 << CS01);
    TCNT0=0x00;
    OCR0=0x30;
}