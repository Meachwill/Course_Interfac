#define F_CPU 11059200UL
#include <avr/io.h>
#include "lib/ASA_Lib.H"
#include <string.h>
#include <avr/iom128.h>
#include "lib/DAC00/ASA_Lib_DAC00.h"
#include <math.h>
int main() {
    ASA_M128_set();
    printf("ASA_DAC example code. by LiYu 2017.12.21\n");
  
    char ID_DAC = 1;
    uint16_t data [100];
    for(uint8_t i =0;i<100;i++){
      data[i]=(uint16_t)2045*sin(i*2*3.14/100)+2045;
      printf("%u\n",data[i] );
    }
    ASA_DAC00_set(ID_DAC, 200, 0x80, 7, 0x01); // 單通道非同步模式
    ASA_DAC00_set(ID_DAC, 200, 0x30, 4, 0x00); // 輸出通道1 S1S2

    while (1) { 
      for(uint8_t i =0;i<100;i++){
        ASA_DAC00_put(ID_DAC, 0, 2, data+i); //DAC卡輸出波型
      }
    }

    return 0;
}