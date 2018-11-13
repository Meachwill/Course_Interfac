#define F_CP 11059200
#include <avr/io.h>
#include "lib/ASA_Lib.H"
#include <string.h>
#include <avr/iom128.h>
#include "./lib/ASA_Lib_HMI.h"

void ADC_SET();
uint16_t ADC_READ();
void TIM_SET();
uint16_t k[100];
uint16_t res=0;
char i=0,fts=0;

ISR(TIMER0_COMP_vect){
    res = ADC_READ();
    k[i] = res;
    i++;
    fts = 1;
}

void main(){
    DDRA=0x01;
    ASA_M128_set();
    uint16_t data = 100 * sizeof(uint16_t);
    int ftp = 0;
    ASA_M128_set();
    ADC_SET();
    TIM_SET();
    sei();
    printf("srart---------!\n");
    while(1){
        printf("%d\n", i);
        if(fts==1){
            fts = 0;
            if(i>100){
                if(ftp==0){
                    M128_HMI_put(data, 1, &k);
                    i = 0;
                    ftp=1;
                     return;
                }
            }
        }
    }
}

void ADC_SET(){
    //設定模式及腳位
    ADMUX |= (1<<REFS0)|(1<<MUX0);
    //除頻設定16,致能ADC
    ADCSRA |= (1 << ADPS0) | (1 << ADEN);
}
uint16_t ADC_READ(){
    ADCSRA |= (1 << ADSC);
    while (ADCSRA & (1 << ADSC));
    return (ADC);
}
void TIM_SET(){
    TIMSK |= (1<<OCIE0);
    TCCR0 = (1 << WGM01) | (1 << CS02) ; //除頻
    TCNT0 = 0x00;
    OCR0 = 0x00;
}