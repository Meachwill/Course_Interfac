#define F_CP 11059200
#include <avr/io.h>
#include "lib/ASA_Lib.H"
#include <string.h>
#include <avr/iom128.h>

void main(){
    ASA_M128_set();
    TCCR0 |= (1 << WGM00) | (1 << WGM01) | (1 << CS00) | (1 << CS01) | (1 << COM01);
    DDRB |= (1 << 4);
    char k=0;
    while (1)
    {
        printf("OCR0=\n");
        scanf("%d",&k);
        if(k>255){
            printf("error!!\n");
        }
        else{
            OCR0=k;
        }
    }
}