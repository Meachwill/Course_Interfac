#define F_CP 11059200
#include <avr/io.h>
#include <string.h>
#include <avr/iom128.h>
#include "lib/ASA_Lib.H"
#include <math.h>

#define write 1
#define read 0
#define tmp175_configermode 0x01
#define slave_address 0x48

void TWI_sck_init(void);
void TWI_START(void);
void TWI_WRITE_ADDRESS(uint8_t data, uint8_t mode);
void TWI_WRITE_DATA(uint8_t data);
void TWI_STOP(void);
uint8_t TWI_READ_DATA();
uint8_t TWI_READNAK_DATA();
float data_all;
uint8_t data_H,data_L;
uint8_t slave_data=0;
uint8_t mode = 0;
uint8_t config=0;

int main(void)
{
    ASA_M128_set();
    ASA_ID_init();
    ASA_ID_set(1); //ASA ID SET
    TWI_sck_init();
    //test=================
    TWI_START();
    printf("ACK start:%x\n", (TWSR & 0xf8));
    TWI_WRITE_ADDRESS(slave_address, write);
    printf("ACK address:%x\n", (TWSR & 0xf8));
    TWI_WRITE_DATA(0x00);
    printf("ACK SET:%x\n", (TWSR & 0xf8));
    TWI_STOP();
    printf("=====================start\n");
    while (1)
    {
        printf("choose mode:\n");
        scanf("%d",&mode);
        switch(mode){
            //configer mode
            case 0:{
                printf("input config data:\n");
                scanf("%d",&config);
                TWI_START();
                printf("ACK start:%x\n", (TWSR & 0xf8));
                TWI_WRITE_ADDRESS(slave_address, write);
                printf("ACK address:%x\n", (TWSR & 0xf8));
                TWI_WRITE_DATA(tmp175_configermode);
                printf("ACK config:%x\n", (TWSR & 0xf8));
                TWI_WRITE_DATA(config);
                printf("ACK SET:%x\n", (TWSR & 0xf8));
                TWI_STOP();
                break;
            }
            //get data
            case 1:{
                TWI_START();
                printf("ACK start:%x\n", (TWSR & 0xf8));
                TWI_WRITE_ADDRESS(slave_address, write);
                printf("ACK address:%x\n", (TWSR & 0xf8));
                TWI_WRITE_DATA(0x00);
                printf("ACK SET:%x\n", (TWSR & 0xf8));
                TWI_STOP();

                printf("start recieve=================\n");

                TWI_START();
                printf("ACK start:%x\n", (TWSR & 0xf8));
                TWI_WRITE_ADDRESS(slave_address, read);
                printf("ACK address:%x\n", (TWSR & 0xf8));
                data_H=TWI_READ_DATA();
                printf("ACK SET:%x\n", (TWSR & 0xf8));
                data_L = TWI_READ_DATA();
                printf("ACK SET:%x\n", (TWSR & 0xf8));
                TWI_STOP();
                _delay_ms(1);
                data_all=(((data_H<<8)+data_L)>>4)*0.0625;
                printf("tmp=%f\n",data_all);
                break;
            }
        }

        
    }
    printf("End===========================\n");
    return 0;
}
//ASA ID SET=======================================
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
//===================================================

//twi================================================
//Bit rate set
void TWI_sck_init(void)
{
    TWBR = 0x20;          //set bit rate
    TWSR |= (1 << TWPS0); //(TWPS1,TWPS0)set prescalar bits   SCL freq = F_CPU / (16 + 2(TWBR).4 ^ TWPS)
}

void TWI_START(void)
{
    //must clear TWINT!!!or TWI will not operate
    //TWSTA TWI START condition bit
    //TWEN TWI enable bit
    TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
    while (!(TWCR & (1 << TWINT)))
        ; //CHECK TWINT ENABLE
    if ((TWSR & 0xF8) != 0x08)
        printf("%x\n", (TWSR & 0xF8) != 0x08); //TWSR(TWI status register) to check the TWINT and START
}

void TWI_WRITE_ADDRESS(uint8_t data, uint8_t mode) //mode 1:read , mode 2:write
{
    if (mode)
    {
        TWDR = ((data << 1) & ~(1)); //Address &write instruction
    }
    else
    {
        TWDR = ((data << 1) | (1)); //Address &read instruction
    }
    TWCR = (1 << TWINT) | (1 << TWEN); //clear TWI interrupt flag ,Enable TWI
    while (!(TWCR & (1 << TWINT))); //CHECK TWINT ENABLE
    _delay_ms(1);
    if (mode)
    {
        if ((TWSR & 0xF8) != 0x18)
        {
            printf("Write address Status%x\n", (TWSR & 0xF8)); //TWSR(TWI status register) to check
        }
    }
    else
    {
        if ((TWSR & 0xF8) != 0x40)
        {
            printf("Read address Status%x\n", (TWSR & 0xF8)); //TWSR(TWI status register) to check
        }
    }
}
void TWI_WRITE_DATA(uint8_t data)
{
    TWDR = data;                                     // put data in TWDR
    TWCR = (1 << TWINT) | (1 << TWEN) | (0 << TWEA); // Clear TWI interrupt flag,Enable TWI
    while (!(TWCR & (1 << TWINT))); // Wait till complete TWDR byte transmitted
    _delay_ms(1);
    if ((TWSR & 0xF8) != 0x28)
        printf("%x\n", TWSR & 0xF8); // Check for the acknoledgement
}
uint8_t TWI_READ_DATA()
{
    TWCR=(1<<TWINT)|(1<<TWEN)|(1<<TWEA);
    while (!(TWCR & (1 << TWINT)));
    return TWDR;
}
uint8_t TWI_READNAK_DATA()
{
    TWCR = (1 << TWINT) | (1 << TWEN);
    while (!(TWCR & (1 << TWINT)));
    return TWDR;
}
void TWI_STOP(void)
{
    // Clear TWI interrupt flag, Put stop condition on SDA, Enable TWI
    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
}
