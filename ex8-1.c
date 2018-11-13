#define F_CP 11059200
#include <avr/io.h>
#include <string.h>
#include <avr/iom128.h>
#include "lib/ASA_Lib.H"
#include <math.h>
void ADC_SET();
void TIM_SET();
uint16_t ADC_READ();
uint16_t k[100];
uint16_t res = 0;
char i = 0, fts = 0;
void main()
{
    ASA_M128_set();
    uint16_t data = 100 * sizeof(uint16_t);
    int ftp = 0;
    ASA_M128_set();
    ADC_SET();
    printf("srart---------!\n");
    while (1)
    {
        res = ADC_READ();
        printf("%d\n", res);
        // if(res>1000){
        //     printf("open!!!!\n");
        // }
        // else{
        //     printf("shutdown\n");
        // }
        // for(i=0;i<100;i++){
        //     k[i] = res;
        //     fts++;
        // }
        // if(fts<10){
        //     M128_HMI_put(data, 1, &k);
        // }

    }
}

void ADC_SET()
{
    DDRF=0x00;
    //設定模式:外部電壓及腳位
    ADMUX |= (1 << REFS0) | (1 << MUX0);
    //除頻設定16,致能ADC
    ADCSRA |= (1 << ADPS0) | (1 << ADEN);
}
uint16_t ADC_READ()
{
    ADCSRA |= (1 << ADSC);
    // while (ADCSRA & (1 << ADSC))
    //     ;
    return (ADCL+(ADCH<<8));
}