/**
* Title:		LAB2.c    	
* Platform:		PICmicro PIC16F877 @ 4 Mhz	
* Written by:	Zhuoran Wang		
*
* Date:			
*
* Platform		PIC DIP-40
*
* Function:		Read status of three switches Inc (RB0) Dec (RB1) and Reset (RB2).
*				Inc increments a button press counter on
*				the LCD display up to a max of 20. One increment per single button press.
*				Dec deccrements a button press counter on
*				the LCD display down to min of 0.  One deccrement per single button press.
*				Reset, sets the counter to 0.
*
*/

#include <pic.h>		// Include header file for PIC microcontroller
#include <stdio.h>		// Include Standard IO header file (this file includes functions such as "sprintf.."
#include "ee302lcd.h"	// Include required header file for LCD functions

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

#define Closed 0		// Define "Closed" as 0 to correspond to Switch action
#define Inc RB0			// Label RB0 as Inc
#define Dec RB1			// Label RB1 as Dec
#define Reset RB2		// Label RB2 as Reset

// globals _____________________________________________________

unsigned char buttonpress;		//button press counter


// Prototypes_____________________________________________________________

void init(void);			//Declare init() function
void LCDButtonCount(void);	//Declare LCDButtonCount() function
void LCDTitle(void);		//Declare LCDTitle() function


// main entry point to application

void main(void)
{

	init();								//do intialisation
	LCDTitle();							//display a title on LCD

// superloop...
	for(;;)
	{
		LCDButtonCount();				//main function to detect button presses and increment/decrement
										//counter appropriately, and display result to LCD
	}
}


//Initialisation
void init(void)
	{
	Lcd8_Init();				// Required initialisation of LCD to 8-bit mode
	TRISB=0x07;					// Set PORTB bit 0 as input
	TRISA=0x01;					// Set PORTA bit 0 as input
	buttonpress = 0;			// Initialise buttonpress variable to 0
	}

void LCDButtonCount(void)
	{
	unsigned char outString[16];	//character array for LCD string. 16 char corresponds to one line of LCD

		if ((Inc == Closed) && (buttonpress < 20))	// if SW1 closed AND buttonpress less than 99
		{	
			buttonpress++;							//increment button press count
			Lcd8_Clear();							//clear LCD
			sprintf(outString,"  Button Press");	//load character array "outString" with characters to be displayed
			Lcd8_Write_String(outString);					//diplay contents of "outString
			Lcd8_Set_Cursor(2,0);							//select line 2 of LCD
			sprintf(outString,"      = %d",buttonpress);	//connvert "buttonpress" value to a character
			Lcd8_Write_String(outString);					//diplay contents of "outString"

			while (Inc == Closed);					//wait for button to be released before continuing 
			
			__delay_ms(100);							//switch debounce delay
		}

		if ((Dec == Closed) && (buttonpress > 0))	// if SW2 closed AND buttonpress greater than 0
		{	
			buttonpress--;							//deccrement button press count
			Lcd8_Clear();							//clear LCD
			sprintf(outString,"  Button Press");	//load character array "outString" with characters to be displayed
			Lcd8_Write_String(outString);					//diplay contents of "outString
			Lcd8_Set_Cursor(2,0);							//select line 2 of LCD
			sprintf(outString,"      = %d",buttonpress);	//connvert "buttonpress" value to a character
			Lcd8_Write_String(outString);					//diplay contents of "outString"

			while (Dec == Closed);					//wait for button to be released before continuing
			
			__delay_ms(100);							//switch debounce delay

		}

		if (Reset == Closed)	// if Reset pressed
		{	
			buttonpress = 0;							//resett button press count to 0
			Lcd8_Clear();							//clear LCD
			sprintf(outString,"  Button Press");	//load character array "outString" with characters to be displayed
			Lcd8_Write_String(outString);					//diplay contents of "outString
			Lcd8_Set_Cursor(2,0);							//select line 2 of LCD
			sprintf(outString,"      = %d",buttonpress);	//connvert "buttonpress" value to a character
			Lcd8_Write_String(outString);					//diplay contents of "outString"

			while (Reset == Closed);					//wait for button to be released before continuing
			
			__delay_ms(100);							//switch debounce delay

		}

	}

//Display title on LCD
void LCDTitle(void)
	{
	Lcd8_Write_String("Laboratory 2");	// display "Laboratory 2" on first line of LCD
	Lcd8_Set_Cursor(2,0);				// select line 2 of LCD
	Lcd8_Write_String("EE302");			// display "EE302" on second line of LCD
	}
