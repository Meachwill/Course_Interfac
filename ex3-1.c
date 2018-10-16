#define F_CP 11059200
#include <avr/io.h>
#include "lib/ASA_Lib.H"
#include <string.h>
#include <avr/iom128.h>
#include "./lib/ASA_Lib_HMI.h"

void adc_init();
// read adc value
uint16_t adc_read();
float vrs=10.0/1024.0;
float ans;
int main()
{
    uint16_t k[100];
    uint16_t data=100*sizeof(uint16_t);
    int ftp=0;
    ASA_M128_set();
    adc_init();
    printf("srart---------!\n");

    uint16_t res;
    while (1)
    {
        res = adc_read();
        ans=res*vrs;
        printf("ans:\n");
        printf("%d\n", res);
        printf("adc:\n");
        printf("%f\n",ans);
        printf("re?Yes:1  NO:2\n");
        scanf("%d",&ftp);
        if(ftp==2){
            return;
        }
    }
}
//for(int i=0;i<100;i++){
//res = adc_read();
//k[i]=res;
//}
//M128_HMI_put(data,1,&k);
void adc_init()
{
    // 使用VCC作為參考電壓Vref
    ADMUX |= (1 << REFS0);
    ADMUX |= 0x01;
    ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
}
uint16_t adc_read(){
    ADCSRA |= (1 << ADSC);
    while (ADCSRA & (1 << ADSC));
    return (ADC);
}