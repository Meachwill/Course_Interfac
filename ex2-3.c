#define F_CPU 11059200UL
#include <avr/io.h>
#include "lib/ASA_Lib.H"
#include <string.h>
#include <avr/iom128.h>
#include "lib/DAC00/ASA_Lib_DAC00.h"
#include "lib/SDC00/ASA_lib_SDC00.h"
#include <math.h>

void TIM_INT();
char ftp = 0;
char i = 0;
char ID_DAC = 1;
char check=0;
float data[100];
uint16_t data3[100];
float data_back[100];
unsigned char ASA_ID = 4, Mask = 0xFF, Shift = 0, Setting = 0xFF;
uint16_t data2[100] ;
uint16_t bytes_ans = 100 * sizeof(float);

ISR(TIMER0_OVF_vect)
{
    ASA_DAC00_set(ID_DAC, 200, 0x30, 4,0x00);
    ASA_DAC00_put(ID_DAC, 0, 2, data2 + i); //DAC卡輸出波型
    i++;
    if (i > 99)
    {
        i = 0;
    }
    //ftp+=1;
}

void main(void)
{

    ASA_M128_set();
    ASA_DAC00_set(ID_DAC, 200, 0x80, 7, 0x01);

    char check = 0; 
    ASA_SDC00_put(ASA_ID, 64, 8, "ex2_3test");
    ASA_SDC00_put(ASA_ID, 72, 3, "txt");
    Setting = 0x05;
    check = ASA_SDC00_set(ASA_ID, 200, Mask, Shift, Setting);
    if (check != 0)
    { 
        printf("Debug point 1, error code <%d>\n", check);
        return;
    }
    printf("[Start] Write data to file with overlap(or new file) mode !!\n");

    M128_HMI_get(bytes_ans, &data);
    //printf("%d\n",check);
    for (int k = 0; k < 100; k++)
    {
        data2[k]=(uint16_t)2045*data[k]+2045;
        //data2[k]=255*data[k]+255;
        //printf("%f\n", data[k]);
    }
    
    printf("%d\n", sizeof(data2[1]));

    for (int i = 0; i < 100; i++)
        {
            //printf("%d\n",i);
            ftp=ASA_SDC00_put(ASA_ID, 0, 2, data2+i);
            //printf("%d\n",ftp);
        }
        printf("[Finish] Write data to file with overlap mode!!\n");
        
        check = ASA_SDC00_set(ASA_ID, 200, 0x01, Shift, 0x00);
        if (check != 0)
        {
            printf("Debug point 2, error code <%d>\n", check);
            return;
        }
        
        TIM_INT();
        sei();
        cli();
        ASA_SDC00_put(ASA_ID, 64, 8, "ex2_3test");
        ASA_SDC00_put(ASA_ID, 72, 3, "txt");
        check = ASA_SDC00_set(ASA_ID,200,0x03,0,0x01);
        printf("abc\n");
        if (check != 0)
        {
            printf("Debug point 2, error code <%d>\n", check);
            return;
        }
        int rec=0;
        for(int i=0;i<4000;i++){
            rec = ASA_SDC00_get(ASA_ID,0,2,data3+i);
            /*if(rec){
                for (int i = 0; i < 100; i++)
                {
                    printf("%d\n", data3[i]);
                    break;
                }
            }*/
        }
        for (int i = 0; i < 100; i++)
        {
            printf("%d\n", data3[i]);
        }
        return 0;
}

void TIM_INT(){
    TIMSK = (1 << TOIE0);
    TCNT0 = 0x60;
    TCCR0 = (1 << CS00);
}



