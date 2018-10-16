#define F_CP 11059200
#include <avr/io.h>
#include "lib/ASA_Lib.H"
#include <string.h>
#include <avr/iom128.h>
void spi_init_master(void) //to initialize SPI as a master
{
    DDRB = (1 << PB2) | (1 << PB1) | (1 << PB0);   //Set MOSI, SCK, SS as Output
    SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0); //Enable SPI, Set as Master
}

uint8_t SPI_send_receive(uint8_t data)
{
    SPDR = data; // transmit the byte to be sent
    uint8_t count = 0;

    // wait for the transfer to complete. if wait time > 1us, then break
    while (!(SPSR & (1 << SPIF)))
    {
        // _delay_us(1);
        // count++;
        ;
    };
    return SPDR; // then return the byte the slave just returned
}

void ASA_ID_set(uint8_t data);
void ASA_ID_init();
void ASA_cs_init();
void ASA_cs(uint8_t en);

int main(void)
{
    ASA_M128_set();
    double TEM;
    uint8_t H, L;
    printf("start!-----------------------------\n");
    spi_init_master();
    ASA_ID_init();
    ASA_ID_set(1);
    ASA_cs_init();

    while (1)
    {
        _delay_ms(250);                               //轉換時間長，SCK腳位開始傳輸時脈後須維持 250ms才能完成一次轉換
        ASA_cs(1);                            //致能
        H = SPI_send_receive(0);                    //receive the High byte from TM121
        L = SPI_send_receive(0);                      //receive the Low  byte from TM121
        ASA_cs(0);                                    //禁能

        TEM = (double)(((H << 8) + L)>>3) * 0.0625; //translate 2byte data to temptature
        printf("TEMP = %f\n", TEM);
    }
    return 0;
}

void ASA_ID_set(uint8_t data)
{
    if (data > 7)
    {
        return;
    }
    PORTB &= ~(7 << PB5);
    PORTB |= (data << PB5);
};

void ASA_ID_init()
{
    DDRB |= (1 << PB7) | (1 << PB6) | (1 << PB5);
};

void ASA_cs_init()
{
    DDRF |= (1 << PF4);
}

void ASA_cs(uint8_t en)
{
    if (en > 1)
    {
        return;
    }
    if (en == 0)
    {
        PORTF &= ~(1 << PF4);
    }
    else
    {
        PORTF |= (1 << PF4);
    }
}