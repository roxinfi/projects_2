/*=============================================================================
	File Name:	ELNC6007AKJLAB4.c
	Author:		Arayan Jagirdar
	Date:		22/10/2024
	Modified:	None
	@Fanshawe College, 2024

	Description: The perpose of this program is to read an analog input from a potentiometer
	             and turn on LEDs based on the level of the analog input. The program
	             uses the ADC module to sample the analog input and compares it against
	             predefined levels to control the state of the LEDs. 
=============================================================================*/

/* Preprocessor ===============================================================
   Hardware Configuration Bits ==============================================*/
#pragma config FOSC		= INTIO67
#pragma config PLLCFG	= OFF
#pragma config PRICLKEN = ON
#pragma config FCMEN	= OFF
#pragma config IESO		= OFF
#pragma config PWRTEN	= OFF 
#pragma config BOREN	= ON
#pragma config BORV		= 285 
#pragma config WDTEN	= OFF
#pragma config PBADEN	= OFF
#pragma config LVP		= OFF
#pragma config MCLRE	= EXTMCLR

// Libraries ==================================================================
#include <p18f45k22.h>
#include <stdio.h>
#include <stdlib.h>

// Prototypes =================================================================
void setOSC4MHz(void);
void configIO(void);

// Constants  =================================================================
#define TRUE		1	
#define FALSE		0
#define ON			1	
#define OFF			0

#define LED1 		LATDbits.LATD3
#define LED2		LATDbits.LATD2
#define LED3        LATDbits.LATD1
#define LED4        LATDbits.LATD0
#define LEDS 		LATD
#define LEVEL1 		128
#define LEVEL2 		256
#define LEVEL3 		512
#define LEVEL4 		768
#define ADCCHANNEL	0
#define PBMASK		0XF0
#define PB1_PRESS	0XE0
#define PB2_PRESS	0XD0
#define PB3_PRESS	0XB0
#define PB4_PRESS	0X70	

// Global Variables  ========================================================

// Functions  ===============================================================
/*>>> setOSC4Mhz: ===========================================================
Author:		Aryan Jagirdar
Date:		04/06/2024	
Modified:	None
Desc:	    This function sets the internal oscillator to 4 MHz.
Input: 		None
Returns:	None 
 ============================================================================*/
void setOSC4MHz()
{  
      
        OSCCON = 0x52;
        while(!OSCCONbits.HFIOFS);
	
} // eo setOSC4MHz::

/*>>> configIO(): ===========================================================
Author:	 Aryan Jagirdar
Date:	04/06/2024
Modified:	None
Desc:		This function configures the I/O pins for inputs and outputs in desired states
			and rest are set in safe configuration.
Input: 		None 
Returns:	None 
 ============================================================================*/
void configIO()
{  
        ANSELA = 0x01;
        LATA =   0x00;
        TRISA =  0xFF;

        ANSELB = 0x00;
        LATB =   0x00;
        TRISB =  0xFF;
        
        ANSELC = 0x00;
        LATC =   0x00;
        TRISC =  0xFF;

        ANSELD = 0x00;
        LATD =   0x00;
        TRISD =  0xF0;

        ANSELE = 0x00;
        LATE =   0x00;
        TRISE =  0xFF;

   
} // eo configIO::


/*>>> getADCSample(char chID): ===============================================
Author:		Aryan Jagirdar
Date:		04/06/2024
Modified:	None
Desc:		ADC channel and innitiate analog-to-digital conversion
Input: 		char chID - The identifire of the ADC channel to read from. 
Returns:	int- The result of ADC conversion.
 ============================================================================*/
int getADCSample(char characterID)
{
	ADCON0bits.CHS	= characterID; //select the ADC channel to sample 
	ADCON0bits.GO	= TRUE; //start the ADC conversion

	while(ADCON0bits.GO); //wait untill the conversion is compelete

	return ADRES;	 //return to the 10-bit ADC result
	
}

/*>>> getconfigADC(): =========================================================
Author:		Aryan Jagirdar
Date:		04/06/2024
Modified:	None
Desc:		This function configures the ADC module for analog sampling.
Input: 		None 
Returns:	None 
 ============================================================================*/

void configADC(void)
{
	ADCON0 = 0x01;   //Turn on the ADC module (ADON bit0) and select channel 0
	ADCON1 = 0x00;   //Configures all pins as analog inputs
	ADCON2 = 0xAC;   //set ADC clock, time and result format
	
} // eo configADC::

/*>>> systemInitialization(): =========================================================
Author:	 Aryan Jagirdar
Date:	04/06/2024
Modified:	None
Desc:		This function initializes the system by setting the oscillator frequency,
			configuring I/O pins, and configuring the ADC module for analog sampling.
Input: 		None 
Returns:	None 
 ============================================================================*/

void systemInitialization(void)
{
	setOSC4MHz();    //set the internal oscillator to 4 MHz
	configIO();      //configures I/O pins for inputs and outputs
	configADC();     //configure ADC module for analog sampling
	
} // eo systemInitialization::

/*=== MAIN: FUNCTION ==========================================================
 ============================================================================*/
void main( void )
{
	char pbon;        //variable to store push button status

	int adcResult=0; //variable to store ADC result

	systemInitialization(); // function call to configure the system
	
	while(1)
	{
	
		pbon=PORTA&PBMASK; //read the state of the  pushbuttons from PORTA 


		if (pbon==PB1_PRESS)//if push button 1 is pressed
		{
			LEDS=FALSE; //turn off all LEDs

			adcResult=getADCSample(ADCCHANNEL);  //Get the ADC result from the specified channel

			if (adcResult>LEVEL1) //Turn on LED1 if the result is greater than LEVEL1
			{
				LED1=TRUE;
			}
			if(adcResult>LEVEL2)  //Turn on LED1 and LED2 if the result is greater than LEVEL2
			{
				LED1=TRUE;
				LED2=TRUE;
			}
			if(adcResult>LEVEL3)  //Turn on LED1, LED2, LED3 if the result is greater than LEVEL3
			{
				LED1=TRUE;
				LED2=TRUE;
				LED3=TRUE;
			}
			if(adcResult>LEVEL4)  //Turn on LED1, LED2, LED3, LED4 if the result is greater than LEVEL4
			{
				LED1=TRUE;
				LED2=TRUE;
				LED3=TRUE;
				LED4=TRUE;
			}
		}
	

		if(pbon==PB2_PRESS)
				{
				LED1=FALSE;
				LED2=FALSE;
				LED3=FALSE;
				LED4=FALSE;
				}

	}

} // eo main::