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
#define SAMPLE_SIZE 10 // Number of samples to be taken
#define SENSORCOUNT 3 // Number of sensors
#define ON 0xFF
#define OFF 0x00



// Global Variables  ==========================================================

typedef struct
{
    int sample[SAMPLE_SIZE]; // Array to store samples
    int insert; // Index for inserting new sample
    int avgReady; // Flag to indicate if average is ready
    int Llimit; // Lower limit for sensor
    int Hlimit; // Upper limit for sensor
    int average; // Average of samples
} sensor_t;

sensor_t sensorCh[SENSORCOUNT]; // Array of sensor structures
sensor_t *sensorChPtr; // Pointer to sensor structure

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
void OSConfig(void)
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
	T0CON = 0x91;
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


/*>>> InitializeSensor: ========================================
Author:		Vraj Patel
Date:		05/13/2025
Modified:	None
Desc:		This function will calculate the average of the samples taken from the sensor.
Input: 		sensorCh_t *sensorCh, pointer to the sensor channel structure.
Returns:	int, the average of the samples.
 ============================================================================*/
void InitializeSensor(sensor_t *sensorCh)
{
    int index;
    for(index = 0; index < SAMPLE_SIZE; index++)
    {
        sensorCh->sample[index] = 0; // Initialize sample array to 0
    }
    sensorCh->insert = 0; // Initialize insert index to 0
    sensorCh->avgReady = FALSE; // Initialize average ready flag to FALSE
    sensorCh->Llimit = 0; // Initialize lower limit to 0
    sensorCh->Hlimit = 0; // Initialize upper limit to 0
    sensorCh->average = 0; // Initialize average to 0

} // eo InitializeSensor::

/*>>> configSP1: ===========================================================
Author:		Vraj Patel
Date:	05/13/2025
Modified:	None
Desc:		This function will configure the serial port for 8-bit transmission
            and asynchronous mode. It also sets the baud rate to 25.
Input: 		None
Returns:	None 
 ============================================================================*/
void configSP1()
{
	TXSTA1 = 0x26; // 8 bit transmission, Asynchronous mode
	RCSTA1 = 0x80; // Serial Port enabled, 8 bit reception
	BAUDCON1 = 0x40; 
	SPBRGH1 = 0x00;
	SPBRG1 = 0x19; // 25 Decimal
}// eo configSP1::


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
    OSConfig(); // Configure oscillator
    IOConfig(); // Configure I/O pins
    ConfigADC(); // Configure ADC
    configSP1(); // Configure Serial Port 1
    configTMR0(PRESENTCOUNT); // Configure Timer0
} // eo SystemInitialization::




/*=== MAIN: FUNCTION ==========================================================
 ============================================================================*/
void main( void )
{
    SystemInitialization(); // Initialize system
    int startup = 0;
    for(startup = 0; startup < SENSORCOUNT; startup++)
    {
        InitializeSensor(&sensorCh[startup]); // Initialize each sensor
    }
    
    while(1)
    {
        char sensorID = 0;

        for(sensorID = 0; sensorID < SENSORCOUNT; sensorID++)
        {
            sensorCh[sensorID].sample[sensorCh[sensorID].insert] = getADCSample(sensorID); // Get ADC sample
            sensorCh[sensorID].insert++; // Increment insert index
            if(sensorCh[sensorID].insert >= SAMPLE_SIZE)
            {
                sensorCh[sensorID].insert = 0; // Reset insert index if it exceeds sample size
                sensorCh[sensorID].avgReady = TRUE; // Set average ready flag
            }
        }

        if(sensorCh[0].avgReady && sensorCh[1].avgReady && sensorCh[2].avgReady)
        {
            for(sensorID = 0; sensorID < SENSORCOUNT; sensorID++)
            {
                int index;
                int sum = 0;
                for(index = 0; index < SAMPLE_SIZE; index++)
                {
                    sum += sensorCh[sensorID].sample[index]; // Calculate sum of samples
                }
                sensorCh[sensorID].average = sum / SAMPLE_SIZE; // Calculate average
                sensorCh[sensorID].avgReady = FALSE; // Reset average ready flag
            }
        }

        printf("Sensor 0: %d, Sensor 1: %d, Sensor 2: %d\n", sensorCh[0].average, sensorCh[1].average, sensorCh[2].average); // Print average values
    }

	
} // eo main::