/**
* Title: SendA.c 
* Platform: PICmicro PIC16F877A @ 4 Mhz
* Written by: 
*
* Date: 09/11/2021
*
* Function: Sending a byte using the USART on board the PIC16F877A
* This program will continuously (every 1s) asynchronously send the 
* letter 'A' out the USART Tx pin (RC6).
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
#ifndef _XTAL_FREQ
// Unless already defined assume 4MHz system frequency
// This definition is required to calibrate the delay functions, __delay_us() and __delay_ms()
#define _XTAL_FREQ 4000000
#endif
#define LED1 RD7
#define LED2 RD6
#define LED3 RD5
#define LED4 RD4
#define HIGH 0
#define LOW 1
//prototypes
void setup(void);
void loop(void);
void send_Hello(void);
void send_Ready(void);
void Receive_data(void);
void main(void)
{
    setup(); // do initialisation
    send_Ready();
    __delay_ms(1000);
    for(;;) // endless loop 
    {
        loop();
        __delay_ms(1000); // Superloop delay of 1s
        
    } 
}

void loop(void) // setup stuff
{
    //Receive_data(); // output Hello via USART
    //send_Hello();
    //send_Ready();
    Receive_data();
}

void setup(void) // setup stuff
{
    PORTD = 0b11111111;
    TRISD = 0b00000000;
    TRISC = 0xC0; //RC6 and RC7 must be set to inputs for USART. 
    TXSTA = 0x24; //Set TXEN bit to enable transmit.
                    //Set BRGH bit for Baud Rate table selection.
    RCSTA = 0x90; //Set CREN bit for continuous read.
    //Set SPEN bit to enable serial port.
    SPBRG = 0x19; //Set Baud Rate to 9600
}

void send_Hello(void)
{
    while(!TXIF); // set when transmit register (TXREG) is empty !TXIF = ??
    TXREG = 0x48; // 0x41 is the ASCII value for the letter ?H?
                  // or you can use TXREG = ?A?
                  // Once TXREG is loaded the byte is automatically
                  // transmitted by the USART peripheral
    __delay_us(500);
    while(!TXIF);
    TXREG = 0x65;
    __delay_us(500);
    while(!TXIF);
    TXREG = 0x6C;
    __delay_us(500);
    while(!TXIF);
    TXREG = 0x6C;
    __delay_us(500);
    while(!TXIF);
    TXREG = 0x6F;
}

void send_Ready(void)
{
    while(!TXIF); // set when transmit register (TXREG) is empty !TXIF = ??
    TXREG = 0x52; // 0x41 is the ASCII value for the letter ?H?
                  // or you can use TXREG = ?A?
                  // Once TXREG is loaded the byte is automatically
                  // transmitted by the USART peripheral
    __delay_us(500);
    while(!TXIF);
    TXREG = 0x65;
    __delay_us(500);
    while(!TXIF);
    TXREG = 0x61;
    __delay_us(500);
    while(!TXIF);
    TXREG = 0x64;
    __delay_us(500);
    while(!TXIF);
    TXREG = 0x79;
}

void Receive_data(void){
    RCIF=0;
    while(!RCIF);
    if(RCREG == '1'){
        LED1 = HIGH;
    }else if(RCREG == '2'){
        LED2 = HIGH;
    }else if(RCREG == '3'){
         LED3 = HIGH;
    }else if(RCREG == '4'){
        LED4 = HIGH;
    }else if(RCREG == '0'){
        LED1 = LOW;
        LED2 = LOW;
        LED3 = LOW;
        LED4 = LOW;
    }
    RCIF=0;
    //RCREG = 0;
}
