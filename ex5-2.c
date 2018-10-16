#define F_CP 11059200
#include <avr/io.h>
#include "lib/ASA_Lib.H"
#include <string.h>
#include <avr/iom128.h>
void spi_init_master(void);
uint8_t SPI_send_receive(uint8_t data);
void ASA_ID_set(uint8_t data);
void ASA_ID_init();
void ASA_cs_init();
void ASA_cs(uint8_t en);
void LED_ADDRESS_init();
void led_display(uint8_t adr,uint8_t *led);

uint8_t led_set_temp[5] = {0,5,0x07,1,1}; //decode,Intensity,scan limit,shotdown,desipaly test
uint8_t led_address[14] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0f};
uint8_t IMAGES_F[8]={0B11111110,0B01100010,0B01101000,0B01111000,0B01101000,0B01100000,0B11110000,0B00000000};
uint8_t IMAGES_U[8]={0B11001100,0B11001100,0B11001100,0B11001100,0B11001100,0B11001100,0B11111100,0B00000000};
uint8_t IMAGES_C[8]={0B00111100,0B01100110,0B11000000,0B11000000,0B11000000,0B01100110,0B00111100,0B00000000};
uint8_t IMAGES_K[8]={0B11100110,0B01100110,0B01101100,0B01111000,0B01101100,0B01100110,0B11100110,0B00000000};

int main(void)
{
    uint8_t s=0x11;
    ASA_M128_set();
    printf("start!-----------------------------\n");
    spi_init_master(); 
    ASA_ID_init();
    ASA_ID_set(1);
    ASA_cs_init();
    LED_ADDRESS_init();
    while (1)
    {
        _delay_ms(500);
        for (int i = 1; i < 9; i++)
        {
            led_display(i,&IMAGES_F[i-1]);
        }
        _delay_ms(500);
        for (int i = 1; i < 9; i++)
        {
            led_display(i, &IMAGES_U[i - 1]);
        }
        _delay_ms(500);
        for (int i = 1; i < 9; i++)
        {
            led_display(i, &IMAGES_C[i - 1]);
        }
        _delay_ms(500);
        for (int i = 1; i < 9; i++)
        {
            led_display(i, &IMAGES_K[i - 1]);
        }
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

uint8_t SPI_send_receive(uint8_t data)
{
    SPDR = data; // transmit the byte to be sent
    uint8_t count = 0;

    // wait for the transfer to complete. if wait time > 1us, then break
    while (!(SPSR & (1 << SPIF)))
        ;
    //printf("%d",(SPSR & (1<<WCOL)));
    return SPDR; // then return the byte the slave just returned
}
void spi_init_master(void) //to initialize SPI as a master
{
    DDRB = (1 << PB2) | (1 << PB1) | (1 << PB0);   //Set MOSI, SCK, SS as Output
    SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0); //Enable SPI, Set as Master
}

void LED_ADDRESS_init(){
    ASA_cs(1);
    SPI_send_receive(led_address[9]); //DECODE MODE SET
    SPI_send_receive(led_set_temp[0]);
    ASA_cs(0);
    _delay_ms(2);
    ASA_cs(1);
    SPI_send_receive(led_address[10]); //Intensity mode set
    SPI_send_receive(led_set_temp[1]);
    ASA_cs(0);
    _delay_ms(2);
    ASA_cs(1);
    SPI_send_receive(led_address[11]); //scan limit mode set
    SPI_send_receive(led_set_temp[2]);
    ASA_cs(0);
    _delay_ms(2);
    ASA_cs(1);
    SPI_send_receive(led_address[12]); //shotdown mode set
    SPI_send_receive(led_set_temp[3]);
    ASA_cs(0);
    _delay_ms(2);
    ASA_cs(1);
    SPI_send_receive(led_address[13]); //desipaly test
    SPI_send_receive(led_set_temp[4]);
    ASA_cs(0);
    _delay_ms(2);
    ASA_cs(1);
    SPI_send_receive(led_address[13]); //desiplay test close
    SPI_send_receive(0x00);
    ASA_cs(0);
}
void led_display(uint8_t adr,uint8_t *led){
    switch(adr){
        case 0:{
            ASA_cs(1);
            SPI_send_receive(led_address[0]); 
            SPI_send_receive(*led);
            ASA_cs(0);
            break;
        }
        case 1:
        {
            ASA_cs(1);
            SPI_send_receive(led_address[1]); 
            SPI_send_receive(*led);
            ASA_cs(0);
            break;
        }
        case 2:
        {
            ASA_cs(1);
            SPI_send_receive(led_address[2]);
            SPI_send_receive(*led);
            ASA_cs(0);
            break;
        }
        case 3:
        {
            ASA_cs(1);
            SPI_send_receive(led_address[3]);
            SPI_send_receive(*led);
            ASA_cs(0);
            break;
        }
        case 4:
        {
            ASA_cs(1);
            SPI_send_receive(led_address[4]);
            SPI_send_receive(*led);
            ASA_cs(0);
            break;
        }
        case 5:
        {
            ASA_cs(1);
            SPI_send_receive(led_address[5]);
            SPI_send_receive(*led);
            ASA_cs(0);
            break;
        }
        case 6:
        {
            ASA_cs(1);
            SPI_send_receive(led_address[6]);
            SPI_send_receive(*led);
            ASA_cs(0);
            break;
        }
        case 7:
        {
            ASA_cs(1);
            SPI_send_receive(led_address[7]);
            SPI_send_receive(*led);
            ASA_cs(0);
            break;
        }
        case 8:
        {
            ASA_cs(1);
            SPI_send_receive(led_address[8]);
            SPI_send_receive(*led);
            ASA_cs(0);
            break;
        }
    }
}