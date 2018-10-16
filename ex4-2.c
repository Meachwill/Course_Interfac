#define F_CP 11059200
#include <avr/io.h>
#include "lib/ASA_Lib.H"
#include <string.h>
#include <avr/iom128.h>
#include "./lib/ASA_Lib_HMI.h"
#include <math.h>
uint16_t k[100];

int main()
{
    ASA_M128_set(); //配置IO口为输出
    TCCR1A = 0;
    TCCR1B = 0;
    TCCR0 |= (1 << WGM00) | (1 << WGM01) | (1 << CS01)|(1<<COM01);
    TCCR1A |= (1 << WGM10) | (1 << COM1A1);
    TCCR1B |= (1<<WGM12)|(1<<CS11);
    DDRB |= (1 << 4)|(1<<5);
    printf("%x, %x\n", TCCR1A, TCCR1B);
    for(int i=0;i<500;i++){
        k[i]=0;
    }
    for (int j = 0; j < 100; j++)
    {
        k[j] = (uint16_t)128 * (sin(j * 2 * 3.141592 / 200) + 1);
    }
    while (1)
    {
        OCR0 = 0;
        for(int i=0;i<100;i++){
            OCR1A = k[i];
        }
        OCR1A=0;
        for (int l = 0; l < 100; l++)
        {
            OCR0 = k[l];
        }
    }
}