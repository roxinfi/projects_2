/*Use of AI / Cognitive Assistance Software is not allowed in any evaluation, assessment or exercise.*/
/*=============================================================================
	File Name:	ELNC6011LAB1.c  
	Author:		Vraj Patel
	Date:		05/27/2025
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
#include <delays.h>

// Constants  =================================================================
#define TRUE	1	
#define FALSE	0
#define TMR0FLAG    INTCONbits.TMR0IF // Timer 0 flag
#define PRESENTCOUNT 3036 // Preset count for Timer 0
#define SAMPLE_SIZE 10 // Number of samples to be taken
#define SENSORCOUNT 3 // Number of sensors
#define ON 0xFF
#define OFF 0x00
#define PBMASK 0xF0 // Mask for push button state
#define NOPRESS    0xF0 // No press state for push button
#define MODEPRESS	0xE0
#define	CHANNELPRESS	0xD0
#define	INCREASE	0x70
#define	DECREASE	0XB0
#define PBSTATE (PORTA & PBMASK) // Push button state
#define ADCRESOLUTION  (5.0f/1023.0f)   // ? now a true float (˜0.0048876)
#define TEMPB           0.5f           // Temperature offset in volts
#define TEMPM           0.01f          // Temperature multiplier (°C per volt)
#define HUMIDM          0.05f          // Humidity multiplier (fraction per volt)
#define CO2M            0.00035625f    // CO2 multiplier (volts per ppm)
#define DEBOUNCE_DELAY  10             // milliseconds of debounce
#define ONSEC           1              // “1 second” worth of Timer0 overflows
#define DEGREE 	248 // Character for degree symbol
#define PATTERNCOUNT 4 // Number of patterns for stepper motor
#define LIGHTING LATCbits.LATC0 // Fan control pin
#define COOLER LATCbits.LATC1 // Cooler control pin
#define HEATER LATCbits.LATC2 // Heater control pin
#define FAN LATCbits.LATC3 // Fan control pin
#define SPKLR LATCbits.LATC4 // Speaker control pin
#define STEP 3 // Stepper motor step count


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

typedef struct
{
    int currentpattern; // Current pattern of the stepper motor
    int patterncount; // Number of patterns in the stepper motor
    int currentposition; // Current position of the stepper motor
    int setposition; // Desired position of the stepper motor
    char movingflag; // Flag to indicate if the motor is moving
}stepper_t;

typedef struct
{
    int channelselect; // Channel selection for the sensor
    char mode; // Mode of the sensor (High/Low limit)
    int pbstate; // Push button state
    int laststate; // Last state of the push button
}pbs_t;


// create an object of the stepper motor typr above, named "vent"
stepper_t vent;

// craete an object of the push button sensor type above, named "pbs"
pbs_t pbs;

char stpmotorarr[PATTERNCOUNT] = {0x01, 0x02, 0x04, 0x08}; // Array for stepper motor states

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


/*>>> ConfigIO: ===========================================================
Author:		Patel Vraj
Date:		05/13/2025
Modified:	None
Desc:		This function will configure the I/O pins as input and output state according to conditions.
			it also sets un-used pins in safe configuration
Input: 		None.
Returns:	None.
 ============================================================================*/
void ConfigIO(void)
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
} // eo ConfigIO::

/*>>> ConfigADC: ===========================================================
Author:		Vraj Patel
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
Desc:		This function will reset the Timer0 counter to pre-set count.
Input: 		int setcount, to set the count value for the timer.
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
Date:		05/13/2025
Modified:	None
Desc:		This function will configure the Timer0 for 1:4 prescaler and 16-bit mode.
Input: 		int setcount, to set the count value for the timer.
Returns:	None.
 ============================================================================*/
void configTMR0(int setcount)
{
	resetTMR0(setcount);
	T0CON = 0x93;
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



/*>>> IntializeStepper: ========================================
Author:		Vraj Patel
Date:		05/27/2025
Modified:	None
Desc:		This function will initialize the stepper motor structure with default values.
Input: 		stepper_t *stepper, pointer to the stepper motor structure.
Returns:	None
============================================================================*/
void IntializeStepper(stepper_t *stepper)
{
    stepper->currentpattern = 0x01; // Start with the first pattern
    stepper->patterncount = 0; // Total number of patterns
    stepper->currentposition = 0; // Start at position 0
    stepper->setposition = 0; // Set desired position to 0
    stepper->movingflag = FALSE; // Motor is not moving initially
}// eo IntializeStepper::

/*>>> IntializePBS: ========================================
Author:		Vraj Patel
Date:		05/27/2025
Modified:	None
Desc:		This function will initialize the push button sensor structure with default values.
Input: 		pbs_t *pbs, pointer to the push button sensor structure.
Returns:	None
============================================================================*/
void IntializePBS(pbs_t *pbs)
{
    pbs->channelselect = 0; // Default channel selection
    pbs->mode = 0; // Default mode
    pbs->pbstate = OFF; // Push button state is off initially
    pbs->laststate = PBMASK; // Last state of the push button is masked
}// eo IntializePBS::

/*>>> ChangeMode: ===========================================================
Author:		Vraj Patel
Date:		05/27/2025
Modified:	None
Desc:		a variable that will be TRUE or FALSE, indicating that the High or Low limit of the selected channel.
Input: 		None
Returns:	None
 ============================================================================*/
void ChangeMode(void)
{
    pbs.mode = !pbs.mode; // Toggle mode between high and low limit
}// eo ChangeMode::

/*>>> ChangeChannel: ===========================================================
Author:		Vraj Patel
Date:		05/27/2025
Modified:	None
Desc:		This function will change the senesor channel based on the push button state of MODEPRESS.
Input: 		None
Returns:	None
 ============================================================================*/
void ChangeChannel(void)
{
    pbs.channelselect++; // Increment channel selection
    if(pbs.channelselect >= SENSORCOUNT) // If it exceeds the number of sensors
    {
        pbs.channelselect = 0; // Reset to first sensor
    }
}// eo ChangeChannel::

/*>>> IncreaseLimit: ===========================================================
Author:		Vraj Patel
Date:		05/27/2025
Modified:	None
Desc:		This function will increase the high or low limit of the selected sensor channel on the push button state of INCREASE.
Input: 		None
Returns:	None
 ============================================================================*/
void IncreaseLimit(void)
{
    char ch = pbs.channelselect; // Get the selected channel
    
    if (pbs.mode == 0) 
    {
        sensorCh[ch].Llimit++;
    }
    else 
    {
        sensorCh[ch].Hlimit++;
    }
}// eo IncreaseLimit::

/*>>> DecreaseLimit: ===========================================================
Author:		Vraj Patel
Date:		05/27/2025
Modified:	None
Desc:		This function will decrease the high or low limit of the selected sensor channel on the push button state of DECREASE.
Input: 		None
Returns:	None
 ============================================================================*/
void DecreaseLimit(void)
{
    int ch = pbs.channelselect;

    if (pbs.mode == 0) 
    {
        sensorCh[ch].Llimit--;
    } 
    
    else 
    {
        sensorCh[ch].Hlimit--;
    }
}// eo DecreaseLimit::

/* >>>DisplayData: ===========================================================
Author:		Vraj Patel
Date:		05/27/2025
Modified:	None
Desc:		This function will display the data from the sensors on the serial port.
Input: 		None
Returns:	None
    ============================================================================*/
void DisplayData(void)
{
	printf("\033[2J \033[H"); // Clear screen
	printf("Sensor System (437)\n\n\r");
    printf("Channel: %d", pbs.channelselect); // Print selected channel
    if(pbs.mode == 0) // If mode is low limit
    {
        printf("\tMode: Low Limit\n\r");
    }
    else // If mode is high limit
    {
        printf("\tMode: High Limit\n\r");
    }
    printf("\n\r");
    printf("Sen0: %3d%cC,\tSen1: %3d%%,\tSensor 2: %3dppm\n\r", sensorCh[0].average, DEGREE, sensorCh[1].average, sensorCh[2].average); // Print sensor averages
    printf("HL: %3d%cC,\tHL: %3d%%,\tHL: %3dppm\n\r", sensorCh[0].Hlimit, DEGREE, sensorCh[1].Hlimit, sensorCh[2].Hlimit); // Print high limit values
    printf("LL: %3d%cC,\tLL: %3d%%,\tLL: %3dppm\n", sensorCh[0].Llimit, DEGREE, sensorCh[1].Llimit, sensorCh[2].Llimit); // Print low limit values

}// eo DisplayData::

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
	RCSTA1 = 0x90; // Serial Port enabled, 8 bit reception
	BAUDCON1 = 0x40; 
	SPBRGH1 = 0x00;
	SPBRG1 = 0x19; // 25 Decimal
}// eo configSP1::


/*>>> SystemInitialization: ========================================
Author:		Vraj Patel
Date:		05/13/2025
Modified:	None
Desc:		This function will be called in the main for intialization
			of clock, I/Os, and ADC  configuration, USART1, and Timer0.
Input: 		None.
Returns:	None.
 ============================================================================*/
void SystemInitialization(void)
{
    OSConfig(); // Configure oscillator
    ConfigIO(); // Configure I/O pins
    ConfigADC(); // Configure ADC
    configSP1(); // Configure Serial Port 1
    configTMR0(PRESENTCOUNT); // Configure Timer0
} // eo SystemInitialization::



void main()
{
    char second = 0;
    char sensorindex;
    float rawADC, volts;
    char startup;
    char index;
    for(startup = 0; startup < SENSORCOUNT; startup++)
    {
        InitializeSensor(&sensorCh[startup]); // Initialize each sensor
        switch (startup)
        {
            case 0: // Temperature Sensor
                sensorCh[startup].Llimit = -40; // Set lower limit for temperature
                sensorCh[startup].Hlimit = 125; // Set upper limit for temperature
                break;

            case 1: // Humidity Sensor
                sensorCh[startup].Llimit = 0; // Set lower limit for humidity
                sensorCh[startup].Hlimit = 100; // Set upper limit for humidity
                break;
            
            case 2: // CO2 Sensor
                sensorCh[startup].Llimit = 400; // Set lower limit for CO2 ppm
                sensorCh[startup].Hlimit = 5200; // Set upper limit for CO2 ppm
                break;
            
            default:
                break;
        }
        systemInitialization(); // Initialize system
        IntializeStepper(&vent); // Initialize stepper motor
        IntializePBS(&pbs); // Initialize push button sensor
        if(TMR0FLAG)
        {
            second++; // Increment second counter
            if(second == ONSEC)
            {
                resetTMR0(PRESENTCOUNT); // Reset Timer0
                second = 0; // Reset second counter
                for(sensorindex = 0; sensorindex < SENSORCOUNT; sensorindex++)
                {
                    sensorCh[sensorindex].sample[sensorCh[sensorindex].insert] = getADCSample(sensorindex); // Get ADC sample
                    sensorCh[sensorindex].insert++; // Increment insert index
                    if(sensorCh[sensorindex].insert >= SAMPLE_SIZE)
                    {
                        sensorCh[sensorindex].insert = 0; // Reset insert index if it exceeds sample size
                        sensorCh[sensorindex].avgReady = TRUE; // Set average ready flag
                    }
                    if(sensorCh[sensorindex].avgReady)
                    {
                        long sum = 0;
                        for(index = 0; index < SAMPLE_SIZE; index++)
                        {
                            sum = sum + sensorCh[sensorindex].sample[index]; // Calculate sum of samples
                        }
                    

                    
                }

                    
            }
        }
    }
    
}