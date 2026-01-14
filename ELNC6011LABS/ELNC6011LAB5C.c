/*Use of AI / Cognitive Assistance Software is not allowed in any evaluation, assessment or exercise.*/
/*=============================================================================
	File Name:	ELNC6011LAB5S.c  
	Author:		Vraj Patel
	Date:		29/07/2025
	Modified:	None
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
}

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
}

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
}

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
