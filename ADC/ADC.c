// 
 /**
* Title:		
* C File        	
* Platform:     PICmicro PIC16F877A @ 4 Mhz	
* Written by:	Zhuoran Wang	
*
* Date:			01/10/2021
*
* Function:		A sample c file to show how to use the LCD display onboard
*               the PIC DIP-40 board. 
*
*/


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

#include <xc.h>
#include <stdio.h>      // Include Standard I/O header file
#include "ee302lcd.h"	// Include LCD header file. This file must be in same
                        // directory as source main source file.

#ifndef _XTAL_FREQ
 // Unless already defined assume 4MHz system frequency
 // This definition is required to calibrate the delay functions, __delay_us() and __delay_ms()
 #define _XTAL_FREQ 4000000
#endif 


// Definitions____________________________________________________________

#define SW1 RB0			// Assign Label SW1 to PortB bit 0 (RB0)
#define G_led RC0
#define R_led RC1
#define CLOSED 0
#define OPEN 1
#define HIGH 1
#define LOW 0



// globals _____________________________________________________

    unsigned char new_value = 0;
    unsigned char ad_value = 0;
    int value1 = 0;
    int value2 = 0;
    int thr = 1;
	unsigned char gOutString[16];

// Prototypes_____________________________________________________________

void setup(void);		// Declare setup function
void loop(void);        // Declare loop function
void readLDR_value(void); //Declare LDR value function
void Light();	// Declare light function
void lcdTitle(void);	// Declare title to LCD function
void clear_outString(void);  // Declare outString clear function


//Main program
void main(void)
{

	setup();				// Call initialisation
    Lcd8_Clear();
	//lcdTitle();			// Call LCDTitle
    
    lcdTitle();		// Write data to LCD
    __delay_ms(1000);


//Superloop
	for(;;)
	{
        loop();
	}
}

void setup(void)
	{
	Lcd8_Init();		// Required initialisation of LCD to 8-bit mode
//	TRISB = 0b00000001;				// set PORTB bits 0 as inputs
//	PORTB = 0b00000001;				// set PORTB bits 0 inputs to 1
//	TRISC = 0b00000000;				// set PORTC bits 0 1 as outputs
//	PORTC = 0b00000000;				// set PORTC bits 0 inputs to 0 as default
//    TRISA = 0b00000100;             // set TRISA bits 3 as input  RA2
//    PORTA = 0b00000000;             // set PORTA all as outputs(default)
//    TRISE = 0b
//    PORTE = 0b
    
    //Set the ACD registers
	TRISB = 0b00000001;			   // Set PORTB bit 0 as input
    TRISA = 0b00000100;            // Set PORTA bit 2 is output
    TRISC = 0b00000000;            // Set PORTC bit 1 and 0 as output
    PORTC = 0b00000010;           
    ADCON0 = 0b01010001;          // Set FOSC/8,RA2 as analog input and A/D converter module is powered up
    ADCON1 = 0b00000010;           // Set Left justified
    OPTION_REG &= 0b01111111;
	}


void readLDR_value(void){
    if(SW1==CLOSED){
        __delay_ms(150);
        while(1){
            __delay_us(50);
            GO_nDONE = 1;
            while(GO_nDONE){
                continue;
            }
            if(ADRESH!=new_value){
                ad_value = ADRESH;
                value1 = (ad_value*5/255);
                value2 = (ad_value*10*5/255)%10;
                Lcd8_Clear();
                sprintf(gOutString,"ADC Voltage is");
                Lcd8_Write_String(gOutString);
                Lcd8_Set_Cursor(2,4);
                sprintf(gOutString,"%d.%d volts",value1,value2);
                Lcd8_Write_String(gOutString);
            }
            
            Light();
            new_value = ad_value;
           __delay_ms(100);
        }
    }
}

void loop(void)
    {
    readLDR_value();
    
    }

void lcdTitle(void)
	{
    Lcd8_Set_Cursor(1,0);
	Lcd8_Write_String("Laboratory 3");		// print "Laboratory 2" on line 1 of LCD
    
	Lcd8_Set_Cursor(2,0);				// select line 2
	Lcd8_Write_String("EE302 - ADC");		// print "EE302" on line 2 of LCD
	}

void Light(){
    if(value1>thr){
        R_led = LOW;
        G_led = HIGH;
    }else if(value1<thr){
        R_led = HIGH;
        G_led = LOW;
    }
}

void clear_outString(void)
{
    int i;
    for (i=0; i<16; i++)
    {
        gOutString[i] = 0x00;
    }
}
