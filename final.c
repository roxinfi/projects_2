/*
 The Following is code is subject to ongoing development of project group 10 for ELNC6010 at Fanshawe College.
 The code is intended for educational purposes only, and does not represent a final product.
 The code is provided as-is, and may be used, distributed, or modified.
 The writers are not responsible for any damages or losses that may occur from the use of the code.
 Take this code as a reference and undermine the provided instructions and guidelines carefully.
 @ Fanshawe College, 2024
 */


/*=============================================================================
	File Name:	ELNC6010PROJECT_GROUP10.c  
	Author:		Vamseedhar Reddy Balapanuri, Vraj Patel
	Date:		21/10/2024
	Modified:	4/12/2024
	© Fanshawe College, 2024

Description: This program will configure I/Os to operate a temperature control system. 
	The program will read the temperature from a sensor and display it on a 7-segment display. 
	The program will also read the user input from a keypad to set the temperature limits. 
	The program will control the heater based on the temperature limits set by the user. 
	The program will also provide manual control options to start and stop the heater.
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
#include <string.h>
#include <delays.h>
#include "Display.h"

// Constants  =================================================================
#define TRUE	1	
#define FALSE	0
#define ON      1
#define OFF     0	
#define ADCCHAN 0
#define PB1     0xE0
#define LEVEL1  128
#define LEVEL2  256
#define LEVEL3  512
#define LEVEL4  768
#define PBMASK  0x3E
#define PB_CAL  0x3C
#define PB_SET  0x3A
#define PB_RESET 0x36
#define PB_MANSTART 0x2E
#define PB_MANSTOP  0x1E         
#define D1_RED    LATEbits.LATE0 // LED for heater
#define D2_BLUE   LATEbits.LATE1 // LED for Cooler
#define D3_GREEN  LATEbits.LATE2 // LED for calibration
#define LOWSETPOINT 10      // Default low setpoint
#define HIGHSETPOINT 30     // Default high setpoint
#define INPUT_MAX_LENGTH 16 // Maximum length of the input string
#define TIMEOUT_MS 5000     // Timeout in milliseconds
#define HOLD 10             // Define HOLD for 100 ms (10 x 10 ms)
#define FOSC 4000000        // Define your oscillator frequency (4 MHz)

#define COL0 LATCbits.LATC7  // Column 0
#define COL1 LATCbits.LATC6  // Column 1
#define COL2 LATBbits.LATB5  // Column 2
#define COL3 LATBbits.LATB4  // Column 3
#define ROW0 PORTBbits.RB3   // Row 0
#define ROW1 PORTBbits.RB2   // Row 1
#define ROW2 PORTBbits.RB1   // Row 2
#define ROW3 PORTBbits.RB0  // Row 3

#define PWMTRIS     TRISCbits.TRISC2 // PWM output on RC2 pin
#define INPUT       1
#define OUTPUT      0
#define TMR2FLAG    PIR1bits.TMR2IF // Timer 2 flag
#define SW_PWM  PORTAbits.RA4       // PWM switch
#define SW_STOP PORTAbits.RA5       // Stop switch
#define PWMSTATE LATCbits.LATC3     // PWM state
#define DCREG       CCPR1L          // PWM duty cycle register
#define DCREGLSB    CCP1CONbits.DC1B // PWM duty cycle LSB
#define DCS         10               // DC settings for 19.2X kHz in 10% increments, as ints
#define LOWSPEED    124              // Low speed
#define HIGHSPEED   350              // High speed
#define DEFAULTSPEED 206 // new default speed
#define PRESETCOUNT 3036 // Preset count for Timer 0
#define TMR0FLAG    INTCONbits.TMR0IF // Timer 0 flag
#define TMR0_PRESET (65536 - 10000) // Timer 0 preset count



// Global Variables  ==========================================================
const char keyMap[4][4] = 
{
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
};

typedef struct 
{
    char inputBuffer[INPUT_MAX_LENGTH + 1]; // To store the input string
    int inputLength;                        // Length of the input
    int finalized;                          // Flag to indicate if input is finalized
} KeypadInput;

typedef struct
{
	char lowerLimitSet;
	char lowerLimit[INPUT_MAX_LENGTH+1];
	char higherLimitSet;
	char higherLimit[INPUT_MAX_LENGTH+1];
	int  lowerLimit_i;
	int  higherLimit_i;
} tempLimits_t;

KeypadInput keypadInput;
tempLimits_t tempLimits;

// DC settings for 19.2X kHz in 10% increments, as ints
int dcTable[DCS];
unsigned char dcValue = 4;  // Default at 50% DC
char pwmEnabled = 0;        // PWM status
char masterControlPwm = 0;     // Master control flag for PWM

char lowBuff[] = "set low Temp";
char highBuff[] = "set High Temp";
char heaterBuff[]  = "Heater ON";
char buffer[30];
char count  = 0;


// Functions  =================================================================
void initKeypad();
char scanKeypad();
void clearKeypadInput(KeypadInput *input);
void getKeypadInput(KeypadInput *input);




/*>>> scanKeypad: ===========================================================
Author:		Vraj Patel
Date:		01/12/2024
Modified:	Vamseedhar Reddy Balapanuri
Desc:		This function will turn on the column by column and check the rows to determine which key is pressed.
			and return the key pressed.
Input: 		None
Returns:	char
 ============================================================================*/
char scanKeypad(void) 
{
    // Row 1
    COL0 = 1;// Pull Row 1 low
    if(COL0)
    {
        if (ROW0 && COL0) 
        {
		return '1';  
        }// Column 1
      if (ROW1 && COL0) 
        {
		
            return '2';  // Column 2
        }
       if (ROW2 && COL0) 
        {
			
             return '3';  // Column 3
        }
      if (ROW3 && COL0)
        { 
          return 'A';  // Column 4
        }
        COL0 = 0;  // Reset Row 1
		COL1 = 1;
    }
	if(COL1)
    {
        if(ROW0&&COL1) 
        {
            return '4';  // Column 1
        }
        if(ROW1&&COL1) 
        {
            return '5';  // Column 2
        }
         if(ROW2&&COL1) 
        {
            return '6';  // Column 3
        }
       if(ROW3&&COL1) 
        {
            return 'B';  // Column 4
        }
        COL1 = 0;  // Reset Row 2
		COL2 = 1;  // Pull Row 3 low
    }
    if (COL2)
	{
        
        if (ROW0&&COL2) 
        {
            return '7';  // Column 1
        }
         if (ROW1&&COL2) 
        {
            return '8';  // Column 2
        }
         if (ROW2&&COL2) 
        {
            return '9';  // Column 3
        }
        if (ROW3&&COL2) 
        {
            return 'C';  // Column 4
        }
        COL2 = 0;  // Reset Row 3
		COL3 = 1; 
    }
    if(COL3)
	{
        
        if (ROW0&&COL3) 
        {
            return '*';  // Column 1
        }
         if (ROW1&&COL3) 
        {
            return '0';  // Column 2
        }
         if (ROW2&&COL3)
        { 
            return '#';  // Column 3
        }
         if (ROW3&&COL3) 
        {
            return 'D';  // Column 4
        }
        COL3 = 0;  // Reset Row 4
    }
    return '\0';  // No key pressed
}// eo scanKeypad::


/*>>> clearKeypadInput: ===========================================================
Author:		Vraj Patel
Date:		01/12/2024
Modified:	None
Desc:		This function will clear the input buffer and reset the input length and finalized flag.
Input: 		KeypadInput *input
Returns:	None
 ============================================================================*/
void clearKeypadInput(KeypadInput *input) 
{
    int i; // Declare loop variable outside
    for (i = 0; i < INPUT_MAX_LENGTH + 1; i++) 
    {
        input->inputBuffer[i] = '\0';
    }
    input->inputLength = 0;
    input->finalized = 0;
}// eo clearKeypadInput::

/*>>> getKeypadInput: ===========================================================
Author:		Vraj Patel
Date:		01/12/2024
Modified:	Vamseedhar Reddy Balapanuri
Desc:		This function will get the input from the keypad and store it in the input buffer.
Input: 		KeypadInput *input
Returns:	None
 ============================================================================
*/
void getKeypadInput(KeypadInput *input) 
{
    char key;
    unsigned int inactivityCounter = 0;

    clearKeypadInput(input); // Clear the input buffer


    while (1) 
    {
        key = scanKeypad(); // Get keypress

		if(TMR0FLAG)
		{
			resetTMR0(PRESETCOUNT);
			D2_BLUE = ~D2_BLUE; // will blink too fast, need to find alternative __vamsee
		}
        if (key != 0) 
        { // If a key is pressed
            inactivityCounter = 0; // Reset inactivity counter
			
            if (key == '#') 
            { // Enter key
                input->finalized = 1; // Mark as finalized
				count = 0;
                break; // Exit input process
            } 
            else if (key == '*') 
            { // Handle delete
                if (input->inputLength > 0) 
                {
                    input->inputBuffer[--input->inputLength] = '\0'; // Remove last character
                }
            } 
            else 
            { // Handle normal input
                if (input->inputLength < INPUT_MAX_LENGTH) 
                {
                    input->inputBuffer[input->inputLength++] = key;  // Add key to input
                    input->inputBuffer[input->inputLength] = '\0';  // Null-terminate the string 
					Delay10KTCYx(20);
	                writeData(key);
    				//writeCMD(SHIFT_CSR_RIGHT);
	    			Delay10KTCYx(20);
					count++;
                }
            }
			if(!PB_SET)
			{
				input->finalized = 1; // added by vamsee
				count = 0;
				break;
			}
        } 
       /* else 
        {
            Delay10KTCYx(HOLD); // Delay for 100 ms
            inactivityCounter += 100; // Increment inactivity counter

            if (inactivityCounter >= TIMEOUT_MS) 
            {
                input->finalized = 1; // Mark as finalized due to timeout
                break; // Exit input process
            }
        }*/
    }
}// eo getKeypadInput::


/*>>> oscConfig: ===========================================================
Author:		Vamseedhar Reddy Balapanuri
Date:		25/11/2024
Modified:	None
Desc:		This function carries the calibration process for the temperature sensor to set the lower and higher limits.
Input: 		None.
Returns:	None.
 ============================================================================*/
void calibration()
{

	 if((!tempLimits.lowerLimitSet)&&(keypadInput.finalized))
	 {
		strcpy(tempLimits.lowerLimit,keypadInput.inputBuffer); // standard function to cpy string from one array to another 
		tempLimits.lowerLimit_i = atoi(tempLimits.lowerLimit); // standard function for converting string to int , doing this coz we are receving and storing values in string
	 	tempLimits.lowerLimitSet = TRUE;
		keypadInput.finalized = FALSE;
	 }
	 else if((!tempLimits.higherLimitSet)&&(keypadInput.finalized))
	 {
		strcpy(tempLimits.higherLimit,keypadInput.inputBuffer);
		tempLimits.higherLimit_i = atoi(tempLimits.higherLimit);
	 	tempLimits.higherLimitSet = TRUE;
		keypadInput.finalized = FALSE;					
	 }
	 /*if(tempLimits.lowerLimitSet && !tempLimits.higherLimitSet)
	 {
	 	displayString(highBuff); // function to write string data on LCD
		Delay10KTCYx(100);
		getKeypadInput(&keypadInput);
		
	 }*/
	 if(tempLimits.lowerLimitSet && tempLimits.higherLimitSet)
	 {
		D2_BLUE = FALSE;
		D3_GREEN = TRUE;
	 }
}

/*>>> oscConfig: ===========================================================
Author:		Vamseedhar Reddy Balapanuri
Date:		21/10/2024
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
} // eo oscConfig::

/*>>> ioConfig: ===========================================================
Author:		Vamseedhar Reddy Balapanuri
Date:		21/10/2024
Modified:	Vraj Patel
Desc:		This function will configure the I/O pins as input and output state according to conditions.
			it also sets un-used pins in safe configuration
Input: 		None.
Returns:	None.
 ============================================================================*/
void ioConfig(void)
{
	ANSELA = 0x01;
	LATA   = 0x00;
	TRISA  = 0x3F;

	ANSELB = 0x00;
	LATB   = 0x00;
	TRISB  = 0xCF;

	ANSELC = 0x00;
	LATC   = 0x00;
	TRISC  = 0x37;

	ANSELD = 0x00;
	LATD   = 0x00;
	TRISD  = 0x00; 

	ANSELE = 0x00;
	LATE   = 0x00;
	TRISE  = 0xF8;
} // eo ioConfig::


/*>>> ConfigADC: ===========================================================
Author:		Vamseedhar Reddy Balapanuri
Date:		21/10/2024
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
Date:		01/12/2024
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
Author:		Vamseedhar Reddy Balapanuri
Date:		21/10/2024
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

/*** configTMR2:: ****************************************************************
Author:     Vraj Patel
Date:       04/12/2024
Modified:   None
Desc:       Configures Timer2 for PWM operation.
Input:      None
Returns:    None
**********************************************************************************/
void configTMR2(void)
{
	PIR1bits.TMR2IF = 0; // Clear the Timer2 interrupt flag
         // Reset Timer2 counter
    T2CON = 0x04;      // Enable Timer2 with prescaler 1:1
	while(!PIR1bits.TMR2IF);
    
}

/*>>> configTMROFor20ms: ========================================
Author:		Vraj Patel
Date:		04/12/2024
Modified:	None
Desc:		This function will configure Timer0 for 20ms.
Input: 		None
Returns:	None.
 ============================================================================*/
void configTMR0For20ms(void) 
{
	INTCONbits.TMR0IF = 0;  // Clear Timer0 interrupt flag
    TMR0H = TMR0_PRESET >> 8;
    TMR0L = TMR0_PRESET & 0xFF;
	T0CON = 0x92;  // Timer0 ON, 16-bit mode, 1:8 prescaler
}


/*--- configPWM: ------------------------------------------------------------
Author:		CTalbot
Date:		07/03/2020
Modified:	S.Roch , Vraj Patel, Vamseedhar Reddy Balapanuri
Desc:		Configures Timer 2 and the PWM module for use.  19.xkHz output.
			Duty cycle is set to a default of 50%.
			*Designed assuming 4MHz Fosc.
Input: 		None
Returns:	None
------------------------------------------------------------------------------*/
void configPWM(  )
{
	int index = 0;
						// Setup the PWM registers for operation
	CCPTMRS0 = 	0x00;	// Select Timer2 for PWM
	CCP1CON = 	0x0C;	// PWM mode, LSb = 00, Active High
	PR2 = 		0x66;	// Period Setting, Frequency Selection, p.182 of datasheet

	// ***		
	// DC = (CCP1L:CCP1CON(5:4) / [4*( PR2+1 )]
	// Least significant bits of the 10 bit value are within the CCP1CON 5:4 bits
	// PB with a 10% per press can be used and changing the DC will require some shifting.
	// Initialize the DC table to total period setting broken into 10% increments.
	/*	for( index=0; index<DCS; index++)
		{
			dcTable[ index ] = (int) (PR2+1) * 4; 	// Insert total period
			dcTable[ index ] /=  DCS;  				// Reduce to a tenth
			dcTable[ index ] *= ( index+1 ); 		// Increase by index+1 tenths

		}// eo for loop			
		*/
		// ***

		// In this program DC = 0.[dcValue+1], the auto-table calculation ensures such.
		// To change Duty Cycle increment dcValue by 1, when the value is equal or above "DCS" reset to 0.	
		// Write into the DC registers, using the DC lookup table.  10 bit system.
	/*
		DCREG = dcTable[ dcValue ] >> 2; 			// Shift down two bits, 10bit -> 8bit
		DCREGLSB = dcTable[ dcValue ];				// Two LSb will write into register, only.
	*/
	DCREG = 206 >> 2;
	DCREGLSB = 206;
	// ***
	// This process needs to be repeated in order to change the DC after a PB press.
	// Configuring Timer 2 after PWM registry changes.
	configTMR2();		// Timer2 on, PSV: 1

	// Engage PWM output.
	PWMTRIS = OUTPUT;	// Enable the PWM pin, PWM should be active.
	
}// eo configPWM::


/*>>> pwmSet: ========================================
Author:		Vraj Patel
Date:		04/12/2024
Modified:	None
Desc:		This function will set the duty cycle of the PWM output.
Input: 		int setvalue, to set the duty cycle of the PWM output.
Returns:	None.
 ============================================================================*/
void pwmSet(int setvalue)
{
    DCREG = setvalue >> 2; // Update 8 MSBs of duty cycle
    DCREGLSB = setvalue;  // Update 2 LSBs of duty cycle
    configTMR2(); // Apply updated duty cycle
    PWMTRIS = OUTPUT; // Enable PWM output
}// eo pwmSet::

// pwmset(LOWSPEED) -> 30% duty cycle // 
// pwmset(HIGHSPEED) -> 85% duty cycle // 

/*>>> systemInitialization: ========================================
Author:		Vamseedhar Reddy Balapanuri
Date:		21/10/2024
Modified:	None
Desc:		This function will be called in the main for intialization
			of clock, I/Os, and ADC  configuration functions.
Input: 		None.
Returns:	None.
 ============================================================================*/
void systemInitialization(void)
{
	oscConfig();
	ioConfig();
	ConfigADC();
	configDisp();
	configPWM();
	configTMR0(PRESETCOUNT);
	
} // eo systemInitialization::

/*=== MAIN: FUNCTION ==========================================================
 ============================================================================*/
void main( void )
{

	int adcResult = 0, adcResult1 = 0;
    char pbStatus = 0, calib = 0, count = 0, arr[17], heater = 0,cooler = 0;
	tempLimits.lowerLimit_i = LOWSETPOINT;
    tempLimits.higherLimit_i = HIGHSETPOINT;
	tempLimits.lowerLimitSet = 0;
    tempLimits.higherLimitSet = 0;

    tempLimits.lowerLimit_i = 0;
    tempLimits.higherLimit_i = 0;
	systemInitialization();
	D2_BLUE = ON;
	writeCMD(CLEAR);
	writeCMD(HOME);
	writeCMD(CURSOR_ON);
	writeCMD(BLINK_ON);
	PWMSTATE = FALSE;
	PWMTRIS = INPUT; 
	while(1)
	{
		
		pbStatus = PORTA & PBMASK;
		
		// implemeted s1_cal, s2_set and s3_rest, you guys need to work on s4, s5 
		// Main Switch Logic
		switch(pbStatus)
		{
			case PB_CAL:
				 calib = TRUE; 
				 displayString(lowBuff);
			  	 writeCMD(SET_CURSOR_SECOND_LINE);
				 getKeypadInput(&keypadInput);
				 calibration();
				break;

			case PB_SET:
				 displayString(highBuff);
			  	 writeCMD(SET_CURSOR_SECOND_LINE);
				 getKeypadInput(&keypadInput);
				 calibration();
				break;
			
			case PB_RESET:
				 D2_BLUE = TRUE;
				 tempLimits.lowerLimitSet = FALSE;
				 tempLimits.higherLimitSet = FALSE;
				 displayString(lowBuff);
			  	 writeCMD(SET_CURSOR_SECOND_LINE);
				 getKeypadInput(&keypadInput);
				 calibration();
				break;
			
			case PB_MANSTART:
                 masterControlPwm = 1;
                 pwmSet(DEFAULTSPEED);
			 	break;
			
			case PB_MANSTOP:
				 masterControlPwm = 0;
                 PWMSTATE = FALSE;
                 PWMTRIS = INPUT;
				break; 
		}

		// Master Control Logic for PWM
        if(masterControlPwm)
        {
            pwmSet(DEFAULTSPEED);
            continue;
        }
        
		// 20 ms delay, measuring and displaying temperature every 0.5 sec
		// indeifinite loop for to display the temperature on LCD
		Delay10KTCYx(2);
		count ++;
		if(count >= 25)
		{
			adcResult1 = getADCSample(ADCCHAN);
			adcResult =	 ((float)(adcResult1)/1024) * 100;
			sprintf(arr, "Temp: %d", adcResult);
			count = 0;
			displayString(arr);
			if(heater)
			{
			displayString2(heaterBuff);
			}
		}


		// heater part converting adc values to percentage for ease of comparision --> Having problem with this part
        // Heater Control Logic
        if ((adcResult <= (tempLimits.lowerLimit_i + (tempLimits.lowerLimit_i / 10))) && !heater) {
            D1_RED = TRUE;  // Turn on heater (Red LED)
            heater = 1;
        } else if ((adcResult > (tempLimits.lowerLimit_i + ((tempLimits.lowerLimit_i / 10) * 3))) && heater) {
            D1_RED = FALSE;  // Turn off heater
            heater = 0;
        }

        // Fan Control Logic
        if (adcResult > tempLimits.higherLimit_i) 
	    {
            PWMSTATE = TRUE;
            pwmSet(HIGHSPEED);  // High speed
            cooler = 1;
        } 
        else if ((adcResult > (tempLimits.higherLimit_i - (tempLimits.higherLimit_i / 10))) && !cooler) 
        {
            PWMSTATE = TRUE;
            pwmSet(LOWSPEED);  // Low speed
            cooler = 1;
        } 
        else if ((adcResult < (tempLimits.higherLimit_i - ((tempLimits.higherLimit_i / 10) * 3))) && cooler) 
        {
            PWMSTATE = FALSE;
            PWMTRIS = INPUT;  // Disable PWM output
            cooler = 0;
        }

		// this condition i will provide to you		
	}// eo while loop
	
} // eo main::