/**
 * Title:		Timer0 Interrupt Example   	
 * Platform:		PICmicro PIC16F877A @ 4 Mhz	
 * Compiler:     Microchip XC8
 * Written by:	Zhuoran Wang		
 *
 * Date:			22/11/2019
 *
 * Function:		In this example code above the main program will be
 * 				in an endless loop toggling bit 1 on PORTD. However, 
 *				every ~2ms the program counter (PC) will jump to the 
 *				isr(void) to service the Timer0 interrupt and toggle
 *				bit 2 of PORTD.
 *
 */

#include <xc.h>


// 'C' source line config statements

// CONFIG
#pragma config FOSC = XT        // Oscillator Selection bits (XT oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable bit (BOR disabled)
#pragma config LVP = OFF        // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#ifndef _XTAL_FREQ
// Unless already defined assume 4MHz system frequency
// This definition is required to calibrate the delay functions, __delay_us() and __delay_ms()
#define _XTAL_FREQ 4000000
#endif 


// Definitions____________________________________________________________

#define LED1 RD0		//define RD0 as LED1
#define LED2 RD1		//define RD1 as LED2

// globals _____________________________________________________
int i = 0;
char ch = 0;

// Prototypes_____________________________________________________________

void setup(void);
void loop(void);
void send(char *str);
char receive(void);

void main(void) {

    setup(); // Call initialization function
    send("Begin");
    /* super loop */

    for (;;) {
        loop(); //Call loop function

        __delay_ms(500); //Superloop delay of 500ms
    }
}

void setup(void) {
    /* setup stuff */

    TRISC = 0xd8;
    TRISD = 0x00;
    PORTD = 0xff;

    TXSTA = 0x24;
    RCSTA = 0x90;
    SPBRG = 0x19; // set baud rate

    T1CON = 0x21;
    INTCON = 0xc0;
    PIE1 = 0x21;
    PIR1 = 0x00;
}

void loop(void) {
    ch=receive();
    if (ch == 'F') {
        T1CON = 0x21;
        send("Fast Mode");
    } else if (ch == 'N') {
        T1CON = 0x31;
        send("Normal Mode");
    }
    ch = 0;
}

/* Interrupt Service Routine */

void __interrupt() // Interrupt identifier
isr(void) // Here be interrupt function - the name is
// unimportant.
{
    if (TMR1IF) {
        TMR1IF = 0;
        i++;
        if (i == 2) {
            LED1 ^= 1;
            LED2 ^= 1;
            i = 0;
        } else {
            LED2 ^= 1;
        }
    } else if (RCIF) {
        RCIF = 0;
        ch = RCREG;
    }
}

// used to send string?

void send(char *str) {
    int index = 0;
    char ch = *str;

    while (ch != '\0') {
        ch = *(str + index);
        index++;
        while (!TXIF)
            ;
        TXREG = ch;
    }
}
char receive(void){
    while(!RCIF);
    return RCREG;
}
