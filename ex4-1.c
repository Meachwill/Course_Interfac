#define F_CP 11059200
#include <avr/io.h>
#include "lib/ASA_Lib.H"
#include <string.h>
#include <avr/iom128.h>
#include "./lib/ASA_Lib_HMI.h"
#include <math.h>

uint16_t k[70];
char i=0;
    int main()
{
    ASA_M128_set();//配置IO口为输出
    TCCR0 |= (1 << WGM00) | (1 << WGM01) | (1<<CS01)|  (1 << COM01);
    DDRB |= (1 << 4);
    uint16_t data = 256 * sizeof(uint16_t);
    uint16_t ocr_set = 0;
    for (int j = 0; j < 70; j++)
    {
        k[j] = (uint16_t)128 * (sin(j *2* 3.14 / 70)+1);
    }
    //TIM_SET();
    //sei();
    while (1)
    {
        for(int i=0;i<70;i++){
            OCR0 = k[i];
        }
    }
}