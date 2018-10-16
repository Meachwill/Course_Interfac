#define F_CP 11059200
#include <avr/io.h>
#include <string.h>
#include <avr/iom128.h>
#include "lib/ASA_Lib.H"
#include <math.h>

#define write 1
#define read 0
#define START_ACK 0x08
#define tca9534a_inputmode 0x00
#define tca9534a_otputmode 0x01
#define tca9534a_configermode 0x03
#define slave_address 0x38

void TWI_sck_init(void);
void TWI_START(void);
void TWI_WRITE_ADDRESS(uint8_t data,uint8_t mode);
void TWI_WRITE_DATA(uint8_t data);
void TWI_STOP(void);

uint8_t slave_data=0x22;
uint8_t mode = 0;

int main(void){
    ASA_M128_set();
    ASA_ID_init();
    ASA_ID_set(1);          //ASA ID SET
    TWI_sck_init();
    TWI_START();
    TWI_WRITE_ADDRESS(slave_address, write);
    TWI_WRITE_DATA(tca9534a_configermode);
    TWI_WRITE_DATA(0x00);
    TWI_STOP();

    while(1){
        _delay_ms(1);
        printf("slave data:\n");
        scanf("%d",&slave_data);
        printf("data=%x\n",slave_data);
        TWI_START();
        TWI_WRITE_ADDRESS(slave_address, write);
        TWI_WRITE_DATA(tca9534a_otputmode);
        TWI_WRITE_DATA(slave_data);
        TWI_STOP();
        }
        printf("End===========================\n");
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




void TWI_sck_init(void){
    TWBR = 0x20; //set bit rate
    TWSR |= (1 << TWPS0); //(TWPS1,TWPS0)set prescalar bits   SCL freq = F_CPU / (16 + 2(TWBR).4 ^ TWPS)
}

void TWI_START(void){
    //must clear TWINT!!!or TWI will not operate
    //TWSTA TWI START condition bit
    //TWEN TWI enable bit
    TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
    while (!(TWCR & (1 << TWINT))); //CHECK TWINT ENABLE
    if ((TWSR & 0xF8) != 0x08)
        printf("%x\n",(TWSR & 0xF8) != 0x08); //TWSR(TWI status register) to check the TWINT and START
}

void TWI_WRITE_ADDRESS(uint8_t data, uint8_t mode)
{
    if(mode){
        TWDR = ((data << 1) & ~(1)); //Address &read instruction
    }
    else{
        TWDR = ((data << 1) | (1)); //Address &write instruction
    }
    TWCR = (1<<TWINT)|(1<<TWEN); //clear TWI interrupt flag ,Enable TWI
    while (!(TWCR & (1 << TWINT))); //CHECK TWINT ENABLE
    _delay_ms(1);
    if(mode){
        if ((TWSR & 0xF8) != 0x18){
            printf("Write address Status%x\n", (TWSR & 0xF8)); //TWSR(TWI status register) to check
        }
    }else{
        if ((TWSR & 0xF8) != 0x40){
            printf("Read address Status%x\n", (TWSR & 0xF8)); //TWSR(TWI status register) to check
        }
    }
    
}
void TWI_WRITE_DATA(uint8_t data)
{
    TWDR = data;                       // put data in TWDR
    TWCR = (1 << TWINT) | (1 << TWEN) | (0 << TWEA);                               // Clear TWI interrupt flag,Enable TWI
    while (!(TWCR & (1 << TWINT))); // Wait till complete TWDR byte transmitted
    _delay_ms(1);
    if ((TWSR & 0xF8) != 0x28) printf("%x\n",TWSR & 0xF8); // Check for the acknoledgement
}
void TWI_STOP(void)
{
    // Clear TWI interrupt flag, Put stop condition on SDA, Enable TWI
    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
}
