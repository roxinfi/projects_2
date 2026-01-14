/*Use of AI / Cognitive Assistance Software is not allowed in any evaluation, assessment or exercise.*/
/*=============================================================================
	File Name:	ELNC6011LAB5S.c  
	Author:		Vraj Patel
	Date:		29/07/2025
	Modified:	None
	© Fanshawe College, 2025

	Description: Purpose of the code is it will allow the operator to collect sample from multiple sensors on
				on a timed interval. Furthermore, the code will also collect and filter and average the collected data
				for appropriate system action. various buttons are used to change the mode of sensor(High/Low limit),
                change the channel of the sensor, increase or decrease the high/low limit of the selected channel.
                futhermore, the code will also control the stepper motor to open or close the vent based on the sensor readings.
                then the code will send the data to the USART2 to be transmitted to the Controller Module.

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
Desc:		This function will calculate the average of the samples taken from the sensor.
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
Desc:		a variable that will be TRUE or FALSE, indicating that the High or Low limit of the selected channel.
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
Desc:		This function will change the senesor channel based on the push button state of MODEPRESS.
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

/*>>> checkLimitChange: ===========================================================
Author:		Vraj Patel
Date:		08/04/2025
Modified:	None
Desc:		This function will check if the limit has changed and update the changeflags accordingly.
Input: 		None
Returns:	None
 ============================================================================*/
void checkLimitChange(void)
{
    if(changeflags.limitChanged) 
    {
        if(pbs.mode == 0) // If mode is low limit
        {
            sprintf(buff, "$CONLIM,%i,%i,%i,%i,%i", CONTROLLER, MYADDY, pbs.channelselect, pbs.mode, sensorCh[pbs.channelselect].Llimit);
        }
        else
        {
            sprintf(buff, "$CONLIM,%i,%i,%i,%i,%i", CONTROLLER, MYADDY, pbs.channelselect, pbs.mode, sensorCh[pbs.channelselect].Hlimit);
        }
        sprintf(buff,"%s,%i\r", buff, checkSum(buff)); // Append checksum to the string
        changeflags.limitChanged = FALSE; // clear the event flag
        changeflags.rxRdy = TRUE; // Set the receive ready flag
    }
}// eo checkLimitChange::


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
	checkLimitChange();
    if(changeflags.rxRdy)
    {
        changeflags.rxRdy = FALSE; // Clear the receive ready flag
        puts2USART(buff);
    }

} // eo displayData::

/*>>> checkSum: ===========================================================
Author:		Vraj Patel
Date:		22/07/2025
Modified:	None
Desc:		This function will calculate the checksum of the string, by passing the string pointer to it.
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
} // eo checkSum::



/*>>> configSP2: ===========================================================
Author:		Vraj Patel
Date:	07/29/2025
Modified:	None
Desc:		This function will configure the serial port USART2 for 8-bit transmission
            and asynchronous mode, 19200 baudrate. It also sets the baud rate to 12.
Input: 		None
Returns:	None 
 ============================================================================*/
void configSP2()
{
    TXSTA2 = 0x26; // 8 bit transmission, Asynchronous mode
    RCSTA2 = 0x90; // Serial Port enabled, 8 bit reception
    BAUDCON2 = 0x40;
    SPBRGH2 = 0x00;
    SPBRG2 = 0x0C; // 19200 Baudrate
} // eo configSP2::


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
    configSP2(); // Configure Serial Port 2
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
    changeflags.rxRdy = FALSE; // Clear receive ready flag
    changeflags.limitChanged = FALSE; // Clear limit changed flag
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
        checkLimitChange();
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
                        int index;
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
                STEPPERPORT = vent.currentpattern & STEPPERMASK; // Set stepper motor port
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
                STEPPERPORT = vent.currentpattern & STEPPERMASK; // Set stepper motor port
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
        }
        
        if(pbs.pbstate == NOPRESS) // If no button is pressed
        {
            pbs.laststate = NOPRESS; // Update last state and reset it to NOPRESS
        }



    }// End of while(1) loop
} // eo main::


/*Use of AI / Cognitive Assistance Software is not allowed in any evaluation, assessment or exercise.*/
/*=============================================================================
	File Name:	ELNC6011LAB5C.c  
	Author:		Vraj Patel
	Date:		29/07/2025
	Modified:	12/08/2025
	© Fanshawe College, 2025

	Description: Purpose of the code is it recieve the 'CONLIM' message from the controller through USART2,
	and update the sensor limits accordingly, and send the updated limit to display via USART1.

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
#include <string.h>


//Prototypes ==================================================================
void ISR(void);// The interrupt service routine.

#pragma code interrupt_vector = 0x00008 // save into int block

void interrupt_vector(void)
{
	_asm
		GOTO ISR
	_endasm
}// eo interrupt_vector::
#pragma code	//save the following in normal space.


// Constants  =================================================================
#define TRUE    1	// True value for boolean
#define FALSE   0 // False value for boolean
#define TMR0FLAG    INTCONbits.TMR0IF // Timer 0 rollover flag
#define ON  0xFF // ON state for devices
#define OFF 0x00 // OFF state for devices
#define MYID 437 // My ID
#define RC2FLAG PIR3bits.RC2IF // USART2 receive interrupt flag
#define TOKENSIZE 10 // Token size for received data

// Timer and time constants ============================================
#define PRESENTCOUNT    15536 // Preset count for Timer 0

// Printf Constants ============================================================
#define DEGREE  248 // Character for degree symbol (°)


// Sensor Constants ============================================================
#define SENSORCOUNT 3 // Number of sensors

// CONLIM Constants ============================================================
#define GREENHOUSEID 437 // Greenhouse system ID
#define BUFSIZE 35 // Buffer size for string array

// Token Constants ============================================================
#define CONLIM 0 // token index for CONLIM command
#define ADDTO 1 // token index for address
#define MYADDRES 2 // token index for my address
#define CHID 3 // token index for channel ID
#define MODE 4 // token index for mode
#define NEWLIMIT 5 // token index for new limit

// Global Variables  ==========================================================
typedef char flag_t; // Define flag type for boolean values


typedef struct
{
    int Llimit; // Lower limit for sensor
    int Hlimit; // Upper limit for sensor
    int average; // Average of samples
} sensor_t;

typedef struct
{
	int insert; // Index for inserting new sample
	char buffer[BUFSIZE]; // Buffer to store received data
	char sentRdy; // Flag to indicate if data is ready to be sent
} commbuf_t;

commbuf_t rcvr; // Global receiver buffer

char buff[BUFSIZE]; // Buffer for string operations
char *buffptr = buff; // Pointer to the buffer for string operations

sensor_t sensorCh[SENSORCOUNT]; // Array of sensor structures

char conlim[] = {"CONLIM"}; // Command for low limit
char *tokens[TOKENSIZE]; // Array to store tokens from received string


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
Modified:	02/08/2025
Desc:		This function will configure the I/O pins as input and output state according to conditions.
			it also sets un-used pins in safe configuration
Input: 		None.
Returns:	None.
 ============================================================================*/
void configIO(void)
{
	ANSELA = 0x00; 
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
} // eo configIO::


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


/*>>> configInterrupts: ===========================================================
Author:		Vraj Patel
Date:		26/11/2024
Modified:	None
Desc:		This function will configure the interrupts for the system.
			It will set the priority and enable Global Interrupts.
Input: 		None
Returns:	None 
 ============================================================================*/
void configInterrupts()
{
	RCONbits.IPEN = FALSE; //It will set the priority
	INTCON = 0xE0;
	PIR3bits.RC2IF = FALSE; // Reset USART2 receive interrupt flag
	PIE3bits.RC2IE = TRUE; // Enable USART2 receive interrupt
}// eo configInterrupts::



/*>>> initializeSensor: ========================================
Author:		Vraj Patel
Date:		05/13/2025
Modified:	None
Desc:		This function will calculate the average of the samples taken from the sensor.
Input: 		sensorCh_t *sensorCh, pointer to the sensor channel structure.
Returns:	None
 ============================================================================*/
void initializeSensor(sensor_t *sensorCh)
{
    sensorCh->Llimit = 0; // Initialize lower limit to 0
    sensorCh->Hlimit = 0; // Initialize upper limit to 0
    sensorCh->average = 0; // Initialize average to 0
} // eo initializeSensor::

/*>>> intializeCommbuf: ========================================
Author:		Vraj Patel
Date:		05/13/2025
Modified:	None
Desc:		This function will initialize the communication buffer structure with default values.
Input: 		combuf_t *rcvr, pointer to the communication buffer structure.
Returns:	None
 ============================================================================*/
void intializeCommbuf(commbuf_t *rcvrptr)
{
	int loop = 0; // Loop counter
	for(loop = 0; loop < BUFSIZE; loop++)
	{
		rcvrptr->buffer[loop] = 0; // Initialize each character in the buffer to 0
	}
	rcvrptr->insert = 0; // Initialize insert index to 0
	rcvrptr->sentRdy = FALSE; // Initialize sent ready flag to FALSE

} // eo intializeCommbuf::


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
	printf("Contoller %i", MYID); // Print controller ID
    printf("\n\r");
    printf("Temperature:%3d°C,\tHumidity:%3d%%,\t\tCO2:%3dppm\n\r", sensorCh[0].average, sensorCh[1].average, sensorCh[2].average); // Print sensor averages           
    printf("HL: %3d%cC,\t\tHL: %3d%%,\t\tHL: %3dppm\n\r", sensorCh[0].Hlimit, DEGREE, sensorCh[1].Hlimit, sensorCh[2].Hlimit); // Print high limit values
    printf("LL: %3d%cC,\t\tLL: %3d%%,\t\tLL: %3dppm\n\r", sensorCh[0].Llimit, DEGREE, sensorCh[1].Llimit, sensorCh[2].Llimit); // Print low limit values
} // eo displayData::

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
} // eo checkSum::

/*>>> validateSentence: ===========================================================
Author:		Vraj Patel
Date:		02/08/2025
Modified:	None
Desc:		This function will validate the sentence received from USART2.
			It will check if the sentence starts with '$' and ends with '\r',
			and if the checksum is valid.
Input: 		char *str, pointer to the string to be validated.
Returns:	char, TRUE if the sentence is valid, FALSE otherwise.
 ============================================================================*/
char validateSentence(char *str)
{
	char newcs = 0; // Initialize new checksum variable
	char rcvcs = 0; // Initialize received checksum variable
	int len = strlen(str); // Get the length of the string
	char csrdy = FALSE; // Flag to indicate if checksum is ready
	while(csrdy == FALSE)
	{
		if(*(str+len) == ',')
		{
			*(str+len) = 0; // Null terminate the string at the comma
			rcvcs = atoi(str+len+1); // Convert the checksum part to integer
			csrdy = TRUE; // Set checksum ready flag
		}
		len--;
	}
	newcs = checkSum(str); // Calculate the new checksum
	if(newcs == rcvcs) // If the new checksum matches the received checksum
	{
		return TRUE; // Return TRUE for valid sentence
	}
	else
	{
		return FALSE; // Return FALSE for invalid sentence
	}
} // eo validateSentence::

/*>>> parsecSentence:===========================================================
Author:		Vraj Patel
Date:		02/08/2025
Modified:	None
Desc:		This function will parse the sentence received from USART2.
			It will extract the data and store it in the tokens array.
Input: 		char *parstr, pointer to the string to be parsed.
Returns:	None
============================================================================*/
void parseSentence(char *parstr)
{
	int tokenCounter = 0; // Initialize token counter
	while(*parstr) // Loop through each character in the string
	{
		if(*parstr == '$' || *parstr == ',') // If the character is a comma
		{
			*parstr = 0; // Null terminate the string at the comma
			tokens[tokenCounter] = parstr+1; // Store the token in the tokens array
			tokenCounter++; // Increment token counter
		}
		parstr++; // Move to next character
	}
} // eo parseSentence::

/*>>> executeSentence: ===========================================================
Author:		Vraj Patel
Date:		02/08/2025
Modified:	None
Desc:		This function will execute the the string data received from USART2.
Input:		None
Returns:	None
============================================================================*/
void executeSentence(void)
{
	if(strcmp(conlim, tokens[CONLIM]) == 0) // If the command is CONLIM
	{
		if(atoi(tokens[MODE])) // If the limit value is Highlimit
		{
			sensorCh[atoi(tokens[CHID])].Hlimit = atoi(tokens[NEWLIMIT]); // Set the high limit for the selected channel
		}
		else // If the limit value is Lowlimit
		{
			sensorCh[atoi(tokens[CHID])].Llimit = atoi(tokens[NEWLIMIT]); // Set the low limit for the selected channel
		}
	}
} // eo executeSentence::

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


/*>>> configSP2: ===========================================================
Author:		Vraj Patel
Date:	07/29/2025
Modified:	None
Desc:		This function will configure the serial port USART2 for 8-bit transmission
            and asynchronous mode, 19200 baudrate. It also sets the baud rate to 12.
Input: 		None
Returns:	None 
 ============================================================================*/
void configSP2()
{
    TXSTA2 = 0x26; // 8 bit transmission, Asynchronous mode
    RCSTA2 = 0x90; // Serial Port enabled, 8 bit reception
    BAUDCON2 = 0x40;
    SPBRGH2 = 0x00;
    SPBRG2 = 0x0C; // 19200 Baudrate
} // eo configSP2::


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
    configSP1(); // Configure Serial Port 1
    configSP2(); // Configure Serial Port 2
    configTMR0(PRESENTCOUNT); // Configure Timer0
	configInterrupts(); // Configure interrupts
} // eo SystemInitialization::




/*=== MAIN: FUNCTION ==========================================================
 ============================================================================*/
void main( void )
{
	char startup = 0;
	intializeCommbuf(&rcvr); // Initialize communication buffer
    for(startup = 0; startup < SENSORCOUNT; startup++)
    {
        switch (startup)
        {
            case 0: // Temperature Sensor
				sensorCh[startup].average = 25; // Set initial average for temperature
                sensorCh[startup].Llimit = 15; // Set lower limit for temperature
                sensorCh[startup].Hlimit = 35; // Set upper limit for temperature
                break;

            case 1: // Humidity Sensor
				sensorCh[startup].average = 40; // Set initial average for humidity
                sensorCh[startup].Llimit = 35; // Set lower limit for humidity
                sensorCh[startup].Hlimit = 65; // Set upper limit for humidity
                break;
            
            case 2: // CO2 Sensor
				sensorCh[startup].average = 780; // Set initial average for CO2
                sensorCh[startup].Llimit = 650; // Set lower limit for CO2 ppm
                sensorCh[startup].Hlimit = 1000; // Set upper limit for CO2 ppm
                break;
            
            default:
                break;
        }
    }
	systemInitialization(); // Initialize system
    while(1)
    {
	}// End of while(1) loop
} // eo main::

void ISR(void)
{
	if(RC2FLAG) //go to get the byte.
	{
		char hold = RCREG2; // Read the received byte
		if(hold == '$') // If the byte is '$'
		{
			rcvr.insert = 0; // Reset insert index
		}
		if(hold == '\r') // If the byte is '\r'
		{
			rcvr.sentRdy = TRUE; // Set sent ready flag
			hold = 0; // Reset hold variable
		}
		rcvr.buffer[rcvr.insert] = hold; // Store the received byte in the buffer
		rcvr.insert++; // Increment insert index
	}
	
	if(TMR0FLAG)
	{
		resetTMR0(PRESENTCOUNT); // Reset Timer0
		if(rcvr.sentRdy) // If data is ready to be sent
		{
			rcvr.sentRdy = FALSE; // Reset sent ready flag
			if(validateSentence(rcvr.buffer)) // Validate the received sentence
			{
				parseSentence(rcvr.buffer); // Parse the received sentence
				executeSentence(); // Execute the parsed sentence
				displayData(); // Display updated sensor data
			}
		}	
	}

	INTCON |= 0xC0; //Reenable global interrupts.
}//eo ISR::
