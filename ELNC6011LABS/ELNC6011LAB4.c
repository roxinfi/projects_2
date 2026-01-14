/*Use of AI / Cognitive Assistance Software is not allowed in any evaluation, assessment or exercise.*/
/*=============================================================================
	File Name:	ELNC6011LAB4.c  
	Author:		Vraj Patel
	Date:		22/07/2025
	Modified:	None
	© Fanshawe College, 2025

	Description: Purpose of the code is it will allow the operator to collect sample from multiple sensors on
				on a timed interval. Furthermore, the code will also collect and filter and average the collected data
				for appropriate system action. various buttons are used to change the mode of sensor(High/Low limit),
                change the channel of the sensor, increase or decrease the high/low limit of the selected channel.
                futhermore, the code will also control the stepper motor to open or close the vent based on the sensor readings.
                it also sends a specific 'CONLIM' message to the display when the high/low limit is changed.

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
#include <usart.h>

// Constants  =================================================================
#define TRUE    1	// True value for boolean
#define FALSE   0 // False value for boolean
#define TMR0FLAG    INTCONbits.TMR0IF // Timer 0 rollover flag
#define ON  0xFF // ON state for devices
#define OFF 0x00 // OFF state for devices

// Switch Constants ============================================================
#define PBMASK  0xF0 // Mask for push button state
#define NOPRESS    0xF0 // No press state for push button
#define MODEPRESS	0xE0 // Mode button press state(High/Low limit)
#define	CHANNELPRESS	0xD0 // Channel button press state(Temperature/Humidity/CO2)
#define	INCREASE	0x70 // Increase button press state
#define	DECREASE	0XB0 // Decrease button press state
#define PBSTATE     (PORTA & PBMASK) // Push button state

// ADC & Voltage Constants ============================================
#define ADCRESOLUTION  (5.0f/1024.0f)   //  a true float value of (0.0048876)
#define TEMPB           0.5f           // Temperature offset in volts
#define TEMPM           0.01f          // Temperature multiplier (°C per volt)
#define HUMIDM          0.05f          // Humidity multiplier (fraction per volt)
#define CO2M            0.000345833f    // CO2 multiplier (volts per ppm)

// Timer and time constants ============================================
#define DEBOUNCE_DELAY  5             // milliseconds of debounce
#define ONSEC   10              // “1 second” worth of Timer0 overflows
#define FIVESEC  5             // “5 seconds” worth of Timer0 overflows
#define PRESENTCOUNT    15536 // Preset count for Timer 0

// Printf Constants ============================================================
#define DEGREE  248 // Character for degree symbol (°)
#define PATTERNCOUNT    4 // Number of patterns for stepper motor
#define LIGHTING    LATCbits.LATC0 // Fan control pin
#define COOLER  LATCbits.LATC1 // Cooler control pin
#define HEATER  LATCbits.LATC2 // Heater control pin
#define FAN LATCbits.LATC3 // Fan control pin
#define  SPKLR   LATCbits.LATC4 // Speaker control pin

// Stepper Motor Constants ============================================
#define STEP    3 // Stepper motor step count
#define STEPPERPORT LATB // Port for stepper motor control
#define STEPPERMASK 0x0F // Mask for stepper motor control pins 


// Sensor Constants ============================================================
#define SAMPLE_SIZE 10 // Number of samples to be taken
#define SENSORCOUNT 3 // Number of sensors

// CONLIM Constants ============================================================
#define GREENHOUSEID 437 // Greenhouse system ID
#define BUFSIZE 35 // Buffer size for string array
#define CONTROLLER 1 // Controller ID for checksum
#define MYADDY 437 // My address as a primary controller


// Global Variables  ==========================================================
typedef char flag_t; // Define flag type for boolean values


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

typedef struct 
{
  flag_t rxRdy;    // set when a full sentence arrives over UART
  flag_t limitChanged;  // set whenever increaseLimit() or decreaseLimit() runs
} eventFlags_t;

eventFlags_t changeflags; // Global event flags

stepper_t vent; // created an object of the stepper motor type above, named "vent"

pbs_t pbs; // created an object of the push button sensor type above, named "pbs"

char buff[BUFSIZE]; // Buffer for string operations
char *buffptr = buff; // Pointer to the buffer for string operations

char stpmotorarr[PATTERNCOUNT] = {0x01, 0x02, 0x04, 0x08}; // Array for stepper motor states

sensor_t sensorCh[SENSORCOUNT]; // Array of sensor structures
char state[2][4] = {"ON ", "OFF"}; // State of the devices (ON/OFF)
char displaysecond = 0; // Second counter for to display CONLIM Change




// Functions  =================================================================


/*>>> oscConfig: ===========================================================
Author:		Patel Vraj
Date:		05/11/2024
Modified:	13/05/2025
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


/*>>> configIO: ===========================================================
Author:		Patel Vraj
Date:		05/11/2024
Modified:	13/05/2025
Desc:		This function will configure the I/O pins as input and output state according to conditions.
			it also sets un-used pins in safe configuration
Input: 		None.
Returns:	None.
 ============================================================================*/
void configIO(void)
{
	ANSELA = 0x07; // Configure AN0-AN2 as analog inputs
	LATA   = 0x00;
	TRISA  = 0xFF;

	ANSELB = 0x00;
	LATB   = 0x00;
	TRISB  = 0xF0; // Configure RB0-RB3 as output

	ANSELC = 0x00;
	LATC   = 0x00;
	TRISC  = 0xF0; // Configure RC0-RC4 as output

	ANSELD = 0x00;
	LATD   = 0x00;
	TRISD  = 0xFF; 

	ANSELE = 0x00;
	LATE   = 0x00;
	TRISE  = 0xFF;
} // eo configIO::

/*>>> configADC: ===========================================================
Author:		Vraj Patel
Date:		05/11/2024
Modified:	13/05/2025
Desc:		This functions configures the ADC module to 12TAD, right justified , Fosc/8
			and standard voltage references.
Input: 		None.
Returns:	None.
 ============================================================================*/
void configADC(void)
{
	ADCON0 = 0x01;
	ADCON1 = 0x00;
	ADCON2 = 0xA9;
} // eo configADC::


/*>>> resetTMR0: ===========================================================
Author:		Vraj Patel
Date:		05/13/2025
Modified:	None
Desc:		This function will reset the Timer0 counter to pre-set count, and clear the Timer0 flag.
            This function is used to reset the Timer0 for the next cycle.
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
Desc:		This function will configure the Timer0 for 1:2 prescaler and 16-bit mode, 
            and set the Timer0 to pre-set count. This function is used to configure the Timer0 for the next cycle.
Input: 		int setcount, to set the count value for the timer.
Returns:	None.
 ============================================================================*/
void configTMR0(int setcount)
{
	resetTMR0(setcount);
	T0CON = 0x90;
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

/*>>> initializeSensor: ========================================
Author:		Vraj Patel
Date:		05/13/2025
Modified:	None
Desc:		This function will initialize the sensor channel structure required for ADC sensor sampling.
Input: 		sensorCh_t *sensorCh, pointer to the sensor channel structure.
Returns:	None
 ============================================================================*/
void initializeSensor(sensor_t *sensorCh)
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

} // eo initializeSensor::



/*>>> intializeStepper: ========================================
Author:		Vraj Patel
Date:		05/27/2025
Modified:	None
Desc:		This function will initialize the stepper motor structure with default values.
Input: 		stepper_t *stepper, pointer to the stepper motor structure.
Returns:	None
============================================================================*/
void intializeStepper(stepper_t *stepper)
{
    stepper->currentpattern = 0x01; // Start with the first pattern
    stepper->patterncount = 0; // Total number of patterns
    stepper->currentposition = 0; // Start at position 0
    stepper->setposition = 0; // Set desired position to 0
    stepper->movingflag = FALSE; // Motor is not moving initially
}// eo intializeStepper::

/*>>> intializePbs: ========================================
Author:		Vraj Patel
Date:		05/27/2025
Modified:	None
Desc:		This function will initialize the push button sensor structure with default values.
Input: 		pbs_t *pbs, pointer to the push button sensor structure.
Returns:	None
============================================================================*/
void intializePbs(pbs_t *pbs)
{
    pbs->channelselect = 0; // Default channel selection
    pbs->mode = 0; // Default mode
    pbs->pbstate = OFF; // Push button state is off initially
    pbs->laststate = PBMASK; // Last state of the push button is masked
}// eo intializePbs::

/*>>> ChangeMode: ===========================================================
Author:		Vraj Patel
Date:		05/27/2025
Modified:	None
Desc:		This function will change the mode of the sensor (High/Low limit) on the push button state of MODEPRESS.
Input: 		None
Returns:	None
 ============================================================================*/
void ChangeMode(void)
{
    pbs.mode = !pbs.mode; // Toggle mode between high and low limit
}// eo ChangeMode::

/*>>> changeChannel: ===========================================================
Author:		Vraj Patel
Date:		05/27/2025
Modified:	None
Desc:		This function will change the sensor channel based on the push button state of MODEPRESS.
Input: 		None
Returns:	None
 ============================================================================*/
void changeChannel(void)
{
    pbs.channelselect++; // Increment channel selection
    if(pbs.channelselect >= SENSORCOUNT) // If it exceeds the number of sensors
    {
        pbs.channelselect = 0; // Reset to first sensor
    }
}// eo changeChannel::

/*>>> increaseLimit: ===========================================================
Author:		Vraj Patel
Date:		05/27/2025
Modified:	None
Desc:		This function will increase the high or low limit of the selected sensor channel on the push button state of INCREASE.
Input: 		None
Returns:	None
 ============================================================================*/
void increaseLimit(void)
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
    changeflags.limitChanged = TRUE; // Set the limit changed flag
}// eo increaseLimit::

/*>>> decreaseLimit: ===========================================================
Author:		Vraj Patel
Date:		05/27/2025
Modified:	None
Desc:		This function will decrease the high or low limit of the selected sensor channel on the push button state of DECREASE.
Input: 		None
Returns:	None
 ============================================================================*/
void decreaseLimit(void)
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
    changeflags.limitChanged = TRUE; // Set the limit changed flag
}// eo decreaseLimit::

/* >>>displayData: ===========================================================
Author:		Vraj Patel
Date:		05/27/2025
Modified:	07/08/2025
Desc:		This function will display the data from the sensors on the serial port via USART1.
            It will print the selected channel, mode, sensor averages, high and low limits,
            and the state of the heater, cooler, fan, and speaker.
Input: 		None
Returns:	None
    ============================================================================*/
void displayData(void)
{

    printf("\033[H\033[2J"); // Clear screen
    printf("Greenhouse Control System %i\n\r", GREENHOUSEID); // Print system ID
    printf("\n\r");
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
    printf("Temperature:%3d°C,\tHumidity:%3d%%,\t\tCO2:%3dppm\n\r", sensorCh[0].average, sensorCh[1].average, sensorCh[2].average); // Print sensor averages           
    printf("HL: %3d%cC,\t\tHL: %3d%%,\t\tHL: %3dppm\n\r", sensorCh[0].Hlimit, DEGREE, sensorCh[1].Hlimit, sensorCh[2].Hlimit); // Print high limit values
    printf("LL: %3d%cC,\t\tLL: %3d%%,\t\tLL: %3dppm\n\r", sensorCh[0].Llimit, DEGREE, sensorCh[1].Llimit, sensorCh[2].Llimit); // Print low limit values
    printf("\n\r");
    if(HEATER) // If heater is ON
    {
        printf("Heater: %s\t", state[0]); // Print heater state
    }
    else // If heater is OFF
    {
        printf("Heater: %s\t", state[1]); // Print heater state
    }
    if(COOLER) // If cooler is ON
    {
        printf("Cooler: %s\t", state[0]); // Print cooler state
    }
    else // If cooler is OFF
    {
            printf("Cooler: %s\t", state[1]); // Print cooler state
    }
    if(FAN) // If fan is ON
    {
        printf("Fan: %s\t", state[0]); // Print fan state
    }
    else // If fan is OFF
    {
        printf("Fan: %s\t", state[1]); // Print fan state
    }
    if(SPKLR) // If speaker is ON
    {
        printf("\n\rSpeaker: %s\t", state[0]); // Print speaker state
    }
    else // If speaker is OFF
    {
        printf("\n\rSpeaker: %s\t", state[1]); // Print speaker state
    }
    printf("Lighting: ON\n\r"); // Print lighting state
    printf("\n\r");
    printf("Vent\n\r");
    printf("Set Position: %d,\tCurrent Position: %d\n\r", vent.setposition, vent.currentposition); // Print stepper motor positions
    printf("Data Pattern: %x\n\r", vent.patterncount); // Print current pattern of the stepper motor
    if(changeflags.limitChanged) // If limit has changed
    {
        if(pbs.mode == 0) // If mode is low limit
        {
            sprintf(buff, "$CONLIM,%i,%i,%i,%i,%i", CONTROLLER, MYADDY, pbs.channelselect, pbs.mode, sensorCh[pbs.channelselect].Llimit);
        }
        else 
        {
            sprintf(buff, "$CONHILM,%i,%i,%i,%i,%i", CONTROLLER, MYADDY, pbs.channelselect, pbs.mode, sensorCh[pbs.channelselect].Hlimit);
        }
        sprintf(buff,"%s,%i", buff, checkSum(buff)); // Append checksum to the string
        changeflags.limitChanged = FALSE; // clear the event flag
        changeflags.rxRdy = TRUE; // Set receive ready flag
    }
    if(changeflags.rxRdy)
    {
        displaysecond++; // Increment second counter
        printf("\n\r%s\r", buff); // Print the buffer
        if(displaysecond >= FIVESEC)
        {
            changeflags.limitChanged = FALSE; // Reset limit changed flag
            changeflags.rxRdy = FALSE; // Clear receive ready flag
            displaysecond = 0; // Reset second counter
        }
    }
}

/*>>> checkSum: ===========================================================
Author:		Vraj Patel
Date:		22/07/2025
Modified:	None
Desc:		This function will calculate the checksum of the string, variable passed to it.
            It will return the checksum value as an integer.
Input: 		char *str, pointer to the string for which checksum is to be calculated.
Returns:	int, checksum value of the string.
 ============================================================================*/
int checkSum(char *str)
{
    char ch = 0; // Initialize checksum variable
    while(*str) // Loop through each character in the string
    {
        ch = ch ^ *str; // Add ASCII value of character to checksum
        str++; // Move to next character
    }
    return ch; // Return the calculated checksum
}// eo checkSum::

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
void systemInitialization(void)
{
    oscConfig(); // Configure oscillator
    configIO(); // Configure I/O pins
    configADC(); // Configure ADC
    configSP1(); // Configure Serial Port 1
    configTMR0(PRESENTCOUNT); // Configure Timer0
} // eo SystemInitialization::




/*=== MAIN: FUNCTION ==========================================================
 ============================================================================*/
void main( void )
{
	char second = 0; // Second counter for Timer0
    char sensorindex = 0; // Index for sensor channels
    char startup = 0; // Startup index for initialization of sensors
    float rawADC = 0; // Raw ADC value
    float volts = 0; // Voltage value from ADC

    intializePbs(&pbs); // Initialize push button sensor
    intializeStepper(&vent); // Initialize stepper motor
    pbs.pbstate = PBSTATE; // Initialize push button state
    changeflags.limitChanged = FALSE; // Initialize limit changed flag
    changeflags.rxRdy = FALSE; // Initialize receive ready flag
    for(startup = 0; startup < SENSORCOUNT; startup++)
    {
        initializeSensor(&sensorCh[startup]); // Initialize each sensor
        switch (startup)
        {
            case 0: // Temperature Sensor
                sensorCh[startup].Llimit = 15; // Set lower limit for temperature
                sensorCh[startup].Hlimit = 35; // Set upper limit for temperature
                break;

            case 1: // Humidity Sensor
                sensorCh[startup].Llimit = 35; // Set lower limit for humidity
                sensorCh[startup].Hlimit = 65; // Set upper limit for humidity
                break;
            
            case 2: // CO2 Sensor
                sensorCh[startup].Llimit = 650; // Set lower limit for CO2 ppm
                sensorCh[startup].Hlimit = 1400; // Set upper limit for CO2 ppm
                break;
            
            default:
                break;
        }
    }

    intializeStepper(&vent); // Initialize stepper motor
    intializePbs(&pbs); // Initialize push button sensor

	systemInitialization(); // Initialize system
    
    while(1)
    {
        if(TMR0FLAG)
        {
            resetTMR0(PRESENTCOUNT); // Reset Timer0
            second++; // Increment second counter
            if(second == ONSEC)
            {
				displayData(); // Display sensor data
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
                        char index;
                        long sum = 0;
                        for(index = 0; index < SAMPLE_SIZE; index++)
                        {
                            sum += sensorCh[sensorindex].sample[index]; // Calculate sum of samples
                        }
                        rawADC = (float)sum / SAMPLE_SIZE; // Calculate raw ADC value 
                        volts  = rawADC * ADCRESOLUTION; // Convert raw ADC value to volts
                        switch(sensorindex)
                        {
                            case 0: // Temperature Sensor
                                sensorCh[sensorindex].average = (int)((volts - TEMPB) / TEMPM); // Calculate average temperature in Celsius
                                break;
                            case 1: // Humidity Sensor
                                sensorCh[sensorindex].average = (int)(volts / HUMIDM); // Calculate average humidity
                                break;
                            case 2: // CO2 Sensor
                                sensorCh[sensorindex].average = (int)(volts / CO2M); // Calculate average CO2 ppm
                                break;
                            default:
                                break;
                        }
					}
                }
            }

            // Check temperature and control cooler, heater, and fan
            if(sensorCh[0].average > sensorCh[0].Hlimit) // If temperature exceeds high limit
            {
                COOLER = ON; // Turn off cooler
                HEATER = OFF; // Turn off heater
                FAN = ON; // Turn on fan
                vent.setposition = 90; // Set stepper motor position to 90 degrees
            }
            else if(sensorCh[0].average < sensorCh[0].Llimit) // If temperature is below low limit
            {
                COOLER = OFF; // Turn off cooler
                HEATER = ON; // Turn on heater
                FAN = ON; // Turn on fan
                vent.setposition = 6; // Set stepper motor position to 6 degrees
            }
            else // If temperature is within limits
            {
                COOLER = OFF; // Turn off cooler
                HEATER = OFF; // Turn off heater
                FAN = OFF; // Turn off fan
            }

            // Check humidity and control speaker and vent
            if(sensorCh[1].average > sensorCh[1].Hlimit) // If humidity exceeds high limit
            {
                SPKLR = OFF;
                vent.setposition = 66; // Set stepper motor position to 90 degrees
            }
            else if(sensorCh[1].average < sensorCh[1].Llimit) // If humidity is below low limit
            {
                SPKLR = ON; // Turn off lighting
                vent.setposition = 12; // Set stepper motor position to 6 degrees
            }
            else
            {
                SPKLR = OFF; // Turn off speaker
            }

            // Check CO2 levels and control fan and vent
            if(sensorCh[2].average > sensorCh[2].Hlimit) // If CO2 exceeds high limit
            {
                FAN = ON; // Turn on fan
                vent.setposition = 9; // Set stepper motor position to 90 degrees
            }
            else if(sensorCh[2].average < sensorCh[2].Llimit) // If CO2 is below low limit
            {
                FAN = ON; // Turn on fan
                vent.setposition = 90; // Set stepper motor position to 6 degrees
            }
            else
            {
                // Do nothing if CO2 is within limits
            }

            if(vent.setposition > vent.currentposition)
            {
                vent.patterncount++; // Increment pattern count
                if(vent.patterncount >= PATTERNCOUNT) // If pattern count exceeds number of patterns
                {
                    vent.patterncount = 0; // Reset pattern count
                }
                vent.currentpattern = stpmotorarr[vent.patterncount]; // Get next pattern
                vent.currentposition = vent.currentposition + STEP; // Move stepper motor forward
                STEPPERPORT = vent.currentpattern & STEPPERMASK; // Set stepper motor port and mask other bits
            }
            else if(vent.setposition < vent.currentposition)
            {
                vent.patterncount--; // Decrement pattern count
                if(vent.patterncount < 0) // If pattern count is less than 0
                {
                    vent.patterncount = PATTERNCOUNT - 1; // Reset to last pattern
                }
                vent.currentpattern = stpmotorarr[vent.patterncount]; // Get previous pattern
                vent.currentposition = vent.currentposition - STEP; // Move stepper motor backward
                STEPPERPORT = vent.currentpattern & STEPPERMASK; // Set stepper motor port and mask other bits
            }
        } // End of Timer0 interrupt handling

        pbs.pbstate = PBSTATE; // Update push button state

        if(pbs.pbstate != pbs.laststate) // If push button state has changed
        {
            pbs.laststate = pbs.pbstate; // Update last state
            switch (pbs.pbstate)
            {
                case MODEPRESS: // If MODE button is pressed
                    ChangeMode(); // Change mode
                    break;
                case CHANNELPRESS: // If CHANNEL button is pressed
                    changeChannel(); // Change channel
                    break;
                case INCREASE: // If INCREASE button is pressed
                    increaseLimit(); // Increase limit
                    break;
                case DECREASE: // If DECREASE button is pressed
                    decreaseLimit(); // Decrease limit
                    break;
                default:
                    break; // Do nothing for other states
            }
        }// eo push button state change handling

        if(pbs.pbstate == NOPRESS) // If no button is pressed
        {
            pbs.laststate = NOPRESS; // Update last state
        }// eo push button state change handling



    }// End of while(1) loop
} // eo main::
