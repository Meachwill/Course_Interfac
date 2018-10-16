#define F_CPU 11059200UL
#include <avr/io.h>
#include "lib/ASA_Lib.H"
#include <string.h>
#include <avr/iom128.h>
#include "lib/DAC00/ASA_Lib_DAC00.h"
#include <math.h>
char ftp=0;
char i=0;
char ID_DAC = 1;
uint16_t data[100];
uint16_t data2[100];
ISR(TIMER0_OVF_vect){
        ASA_DAC00_set(ID_DAC, 200, 0x30, 4, 0x01);
        ASA_DAC00_put(ID_DAC, 0, 2, data2 + i); //DAC卡輸出波型
        ASA_DAC00_set(ID_DAC, 200, 0x30, 4, 0x00);
        ASA_DAC00_put(ID_DAC, 0, 2, data + i); //DAC卡輸出波型
    i++;
    if(i>99){
        i=0;
    }
}

int main()
{
    ASA_M128_set();
    ASA_DAC00_set(ID_DAC, 200, 0x80, 7, 0x01);// 單通道非同步模式
    for (uint8_t i = 0; i < 100; i++)
    {
        data[i] = (uint16_t)2045*sin(i * 2 * 3.14 / 100) + 2045;
        printf("%u\n", data[i]);
    }
    for (uint8_t i = 0; i < 100; i++)
    {
        data2[i] = (uint16_t)2045*cos(i * 2 * 3.14 / 100) +2045;
        //printf("%u\n", data2[i]);
    }
    TIMSK=(1<<TOIE0);
    TCNT0=0x00;
    TCCR0=(1<<CS00);
    sei();
    
    while (1)
    {
    }

    return 0;
}
