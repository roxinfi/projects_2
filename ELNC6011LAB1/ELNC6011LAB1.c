/*Use of AI / Cognitive Assistance Software is not allowed in any evaluation, assessment or exercise.*/
/*=============================================================================
	File Name:	ELNC6011LAB1.c  
	Author:		Vraj Patel
	Date:		05/13/2025
	Modified:	None
	© Fanshawe College, 2025

	Description: Purppose of the code is it will allow the operator to collect samplaing from multiple sensors on
				on a timed interval. Furthermore, the code will also collect and filter and average the collected data 
				for appropriate system action. 
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

// Constants  =================================================================
#define TRUE	1	
#define FALSE	0
#define TMR0FLAG    INTCONbits.TMR0IF // Timer 0 flag
#define PRESENTCOUNT 3036 // Preset count for Timer 0


// Global Variables  ==========================================================

// Functions  =================================================================


/*>>> OSConfig: ===========================================================
Author:		Patel Vraj
Date:		13/05/2025
Modified:	None
Desc:		This function will configure internal oscillator of PIC18F45K22 to
			4MHz and waits for it to be stablized
Input: 		None.
Returns:	None.
 ============================================================================*/
void oscConfig(void)
{
	OSCCON = 0x52; 
	while(!OSCCONbits.HFIOFS);
} // eo OSConfig::


/*>>> IOConfig: ===========================================================
Author:		Patel Vraj
Date:		05/13/2025
Modified:	None
Desc:		This function will configure the I/O pins as input and output state according to conditions.
			it also sets un-used pins in safe configuration
Input: 		None.
Returns:	None.
 ============================================================================*/
void IOConfig(void)
{
	ANSELA = 0x07;
	LATA   = 0x00;
	TRISA  = 0xFF;

	ANSELB = 0x00;
	LATB   = 0x00;
	TRISB  = 0xFF;

	ANSELC = 0x00;
	LATC   = 0x00;
	TRISC  = 0xFF;

	ANSELD = 0x00;
	LATD   = 0x00;
	TRISD  = 0xFF; 

	ANSELE = 0x00;
	LATE   = 0x00;
	TRISE  = 0xFF;
} // eo ioConfig::

/*>>> ConfigADC: ===========================================================
Author:		vraj Patel
Date:		05/13/2025
Modified:	None
Desc:		This functions configures the ADC module to 12TAD, right justified , Fosc/8
			and standard voltage references.
Input: 		None.
Returns:	None.
 ============================================================================*/
void ConfigADC(void)
{
	ADCON0 = 0x01;
	ADCON1 = 0x00;
	ADCON2 = 0xA9;
} // eo ConfigADC::


/*>>> resetTMR0: ===========================================================
Author:		Vraj Patel
Date:		05/13/2025
Modified:	None
Desc:		This function will reset the Timer0 counter.
Input: 		setcount, to set the count value for the timer.
Returns:	None.
 ============================================================================*/
void resetTMR0(int setcount)
{
	TMR0FLAG = FALSE;
	TMR0H = setcount >> 8;
	TMR0L = setcount;
} // eo resetTMR0::


/*>>> configTMR0: ===========================================================
Author:		Vraj Patel
Date:		01/12/2024
Modified:	None
Desc:		This function will configure the Timer0 for 1:8 prescaler and 16-bit mode.
Input: 		setcount, to set the count value for the timer.
Returns:	None.
 ============================================================================*/
void configTMR0(int setcount)
{
	resetTMR0(setcount);
	T0CON = 0x92;
} // eo configTMR0::



/*>>> getADCSample: ========================================
Author:		Vraj Patel
Date:		05/13/2025
Modified:	None
Desc:		This function will get the analoge input from channel configured by input paramater
			passed to this function and returns the ADC result.
Input: 		char adcChnl, to select the pin for getting the ADC sampling.
Returns:	int ADRES, ADRES is where the ADC conversion results are stored.
 ============================================================================*/
int getADCSample(char adcChnl)
{
	ADCON0bits.CHS = adcChnl;
	ADCON0bits.GO  = TRUE;
	
	while(ADCON0bits.GO);
	return ADRES;
} // eo getADCSample::



/*>>> SystemInitialization: ========================================
Author:		Vraj Patel
Date:		05/13/2025
Modified:	None
Desc:		This function will be called in the main for intialization
			of clock, I/Os, and ADC  configuration functions.
Input: 		None.
Returns:	None.
 ============================================================================*/
void SystemInitialization(void)
{
    oscConfig(); // Configure the oscillator
    ioConfig(); // Configure I/O pins
    ConfigADC(); // Configure ADC Conversion
    configTMR0(PRESETCOUNT); // Configure Timer0
} // eo SystemInitialization::




/*=== MAIN: FUNCTION ==========================================================
 ============================================================================*/
void main( void )
{
	
} // eo main::