#define F_CP 11059200
#include <avr/io.h>
#include "lib/ASA_Lib.H"
#include <string.h>
#include <avr/iom128.h>
#include "./lib/ASA_Lib_HMI.h"

void tim_int();
void adc_init();
void sd_set(uint16_t *a);
uint16_t adc_read();
uint16_t k[100];
uint16_t res = 0;
char i=0;
char fts=0;
unsigned char ASA_ID = 4, Mask = 0xff, Shift = 0, Setting = 0x05;

ISR(TIMER0_COMP_vect)
{
    res = adc_read();
    k[i] = res;
    i++;
    fts=1;
    PORTA^=0x01;
}

int main()
{
    DDRA |= ~(DDRA);
    char check = 0;
    uint16_t data = 100 * sizeof(uint16_t);
    int ftp = 0;
    ASA_M128_set();
    adc_init();
    tim_int();
    sei();
    printf("srart---------!\n");
    while (1)
    {  
        PORTA=0x00;
        if (fts == 1)
        {
            k[i] = res;
            fts=0;
            if(i>100){
                cli();
                sd_set(&check); 
                if (check != 0)
                {
                    printf("Debug point 1, error code <%d>\n", check);
                    return;
                }
                printf("[Start] Write data to file with overlap(or new file) mode !!\n");
                for (int j = 0; j < 100; j++)
                {
                    ftp=ASA_SDC00_put(ASA_ID, 0,2 , k+j);
                    printf("%d\n",ftp);
                }
                printf("%d\n",sizeof(k[1]));
                M128_HMI_put(data, 1,&k);
                i = 0;
                check = ASA_SDC00_set(ASA_ID, 200, 0x01, Shift, 0x00);
                if (check != 0)
                {
                    printf("Debug point 2, error code <%d>\n", check);
                    return;
                }
                else{
                    printf("[Finish] Write data to file with overlap mode!!\n");
                    return;
                }
                
            }
            printf("%d\n", i);
        }
}
}

void tim_int()
{
    TIMSK = (1 << OCIE0);
    TCCR0 = (1<<WGM21)|(1 << CS01)|(1<<CS00);
    OCR0 = 0x10;
    TCNT0 = 0x00;
}
void adc_init()
{
    ADMUX |= (1 << REFS0); //選擇外部電壓做參考電壓
    ADMUX |= 0x01; //ADC1做輸入(PF1)
    ADCSRA = (1 << ADEN) | (1 << ADPS2);
    //ADC致能  除頻值=16
}
uint16_t adc_read()
{
    ADCSRA |= (1 << ADSC);
    while (ADCSRA & (1 << ADSC));
    return (ADC);
}
void sd_set(uint16_t *a){
    ASA_SDC00_put(ASA_ID, 64, 8, "test5");
    ASA_SDC00_put(ASA_ID, 72, 3, "txt");
    *a = ASA_SDC00_set(ASA_ID, 200, Mask, Shift, 0x05);
}