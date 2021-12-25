/*------------------------------
|Name: Basic alarm system |
|Date: 2021.9.27 |
|Version: 1.0 |
|Author: Zhuoran Wang |
|Hardware Platform: PIC16F877A |
--------------------------------*/
//Configuration Bits
#pragma config FOSC = XT // Oscillator Selection bits (XT oscillator)
#pragma config WDTE = OFF // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = OFF // Brown-out Reset Enable bit (BOR disabled)
#pragma config LVP = OFF // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCL
#pragma config CPD = OFF // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to
#pragma config CP = OFF // Flash Program Memory Code Protection bit (Code protection off)
// #pragma config statements should precede project file includes.
#include <xc.h>
#ifndef _XTAL_FREQ
// Unless already defined assume 4MHz system frequency
// This definition is required to calibrate the delay functions, __delay_us() and __delay_ms()
#define _XTAL_FREQ 4000000
#endif
#define LED1 RC0 //bit 0 of PORTC
#define LED2 RC1
#define SPK RC2
#define pressed 0
#define unpressed 1
#define ON 1
#define OFF 0
#define ARM RB0
#define ALARM RB1
#define TAMPER RB2
//***Prototypes***
void init(void);
void Tone(); // 1 cycle of 1khz squarewave output
void doAlarm(void);
//***Main Superloop***
main(void)
{
    init(); //call initialisation function
    for(;;) //start of Superloop
    {
        doAlarm(); //Alarm
    }
}
//***Initialisation***
void init(void)
{
  // [Use comments to explain your initialization in detail, e.g...]
    TRISB = 0b00000111; // set PORTB bits 0 and 1 and 2 as inputs
    PORTB = 0b00000111; // set PORTB bits 0 1 2 inputs to 1
    TRISC = 0b00000000; // set all PORTC bits as outputs
    PORTC = 0b00000000; // set all PORTC outputs to 0
}
//Functions
void Tone() // 1 cycle of 1khz squarewave output
{
    for(;;)
    {
        SPK = ON;
        __delay_us(500); //500us delay
        SPK = ON;
        __delay_us(500); //500us delay
    }
}
//***Other Functions***
void doAlarm(void)
{
    if (TAMPER == pressed)
    {
        LED1 = ON;
        LED2 = ON;
        Tone();
    }else if((TAMPER == unpressed) && ((ARM || ALARM) == unpressed))
    {
        LED1 = OFF;
        LED2 = ON;
        Tone();
    }else
    {
        LED1 = ON;
        LED2 = OFF;
        SPK = OFF;
    }
}
