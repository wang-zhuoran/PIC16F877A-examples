/**
* Title: I2C.c 
* Platform: PICmicro PIC16F877A @ 4 Mhz
* Written by: WZR
*
* Date: 11/11/2021
*
*/
// CONFIG
#pragma config FOSC = XT // Oscillator Selection bits (XT oscillator)
#pragma config WDTE = OFF // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = OFF // Brown-out Reset Enable bit (BOR disabled)
#pragma config LVP = OFF // Low-Voltage (Single-Supply) In-Circuit Serial 
// Programming Enable bit (RB3 is digital I/O, HV
// on MCLR must be used for programming)
#pragma config CPD = OFF // Data EEPROM Memory Code Protection bit 
//(Data EEPROM code protection off)
#pragma config WRT = OFF // Flash Program Memory Write Enable bits (Write
// protection off; all program memory may be 
// written to by EECON control)
#pragma config CP = OFF // Flash Program Memory Code Protection bit 
//(Code protection off)
#include <xc.h> // Include standard PIC library
#include <stdio.h> 
#include "I2C_EE302.h"
#ifndef _XTAL_FREQ
// Unless already defined assume 4MHz system frequency
// This definition is required to calibrate the delay functions, __delay_us() and __delay_ms()
#define _XTAL_FREQ 4000000
#endif
#define hi 0x11
#define lo 0x55
//prototypes
void setup(void);
void loop(void);
void Write_data(void);
void Send_data(void);
void main(void)
{
    setup(); // do initialisation
    Write_data();
    __delay_ms(1000);
    for(;;) // endless loop 
    {
        loop();
        __delay_ms(1000); // Superloop delay of 1s
    } 
}

void loop(void) // setup stuff
{
    Send_data(); // output Hello via USART
    
}

void setup(void) // setup stuff
{
    TRISC = 0b1101100; //RC6 and RC7 must be set to inputs for USART. 
    TXSTA = 0x24; //Set TXEN bit to enable transmit.
                    //Set BRGH bit for Baud Rate table selection.
    RCSTA = 0x90; //Set CREN bit for continuous read.
    //Set SPEN bit to enable serial port.
    SPBRG = 0x19; //Set Baud Rate to 9600
    i2c_init();
}

void Write_data(void)
{
    unsigned char address_hi = hi;
    unsigned char address_lo = lo;
    unsigned char data[12] = "Testing I2C";
    int i = 0;
    while(i<=10){
        write_ext_eeprom(address_hi,address_lo,data[i]);
        address_lo++;
        i++;
    }
}
void Send_data(void){
    unsigned char address_hi = hi;
    unsigned char address_lo = lo;
    int i = 0;
    while(i<=10){
        while(!TXIF);
        TXREG = read_ext_eeprom(address_hi,address_lo);
        address_lo++;
        i++;
        __delay_us(500);
    }
}
