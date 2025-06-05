 /*Use of AI / Cognitive Assistance Software is not allowed in any evaluation, assessment or exercise.*/
/*=============================================================================
	File Name:	ELNC6011MJLab2.c  
	Author:		Mihir Jariwala
	Date:		27/05/2025
	Modified:	 None
	© Fanshawe College, 2025

	Description: Describe the purpose of the whole program.  What the 
			program does and why. What platform (hardware part number) and / or 
			supplemental documents required for the program’s use. 
			Describe any supporting information (schematic, programming 
			environment, ect..). 
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
#define TMR0FLAG  INTCONbits.TMR0IF
#define SAMPLESIZE 10
#define SENSORCOUNT 3
#define ONESEC 1
#define	PRESENTCOUNT 3036
#define PBMASK 0XF0
#define NOPRESS 0XF0
#define STEPPERMASK 0X0F
#define PATTERNCOUNT 4
#define FAN LATCbits.LATC3
#define HEATER LATCbits.LATC2
#define COOLER LATCbits.LATC1
#define LIGHT  LATCbits.LATC0
#define MODE 0xE0
#define CHANSEL 0XD0
#define INCREMENT 0XB0
#define DECREMENT 0X70
#define TEMPM  0.01f
#define TEMPB 0.5f
#define CO2M  0.000362f
#define ADCRES 0.00488f
#define CONVPER 0.05f
#define DEBOUNCE 10 // milliseconds of debounce

// Global Variables  ==========================================================
char pattern[PATTERNCOUNT]= {0X01,0X02,0X04,0X08};

typedef struct
{
	char currentPattern;
	char patternCounter;
	int currentPos;
	int setPos;
	char motorMov;

}stepper_t;
stepper_t vent;

typedef struct
{
	char chanSel;
	char mode;
	char pbState;
	char pbLaststate;

}pbs_t;

pbs_t pbs;

typedef int sensor_t;
typedef char flag_t;

typedef struct
{
	sensor_t samples[SAMPLESIZE];
	sensor_t avg;
	sensor_t upperLimit;
	sensor_t lowerLimit;
	sensor_t insert;
	flag_t   avgRdy;

}sensorCh_t;

sensorCh_t sensors[SENSORCOUNT];

// Functions  =================================================================
/*>>> setOSC4MHZ: ===========================================================
Author:		Name
Date:		DD/MM/YYYY
Modified:	Name & date or None
Desc:		Specifically, what does this function do?
Input: 		None or dataType variableName, why this input is needed / what is 
			its use.  LIST EACH INPUT ARGUMENT ON ITS OWN LINE.
Returns:	None or datatype, what is the data / what is its meaning
 ============================================================================*/
 void setOSC4MHZ(void)
 {
	 OSCCON = 0X52;
	 while(!OSCCONbits.HFIOFS);
	 
 }//eo setOSC4MHZ::
 
 /*>>>configIO : ===========================================================
Author:		Name
Date:		DD/MM/YYYY
Modified:	Name & date or None
Desc:		Specifically, what does this function do?
Input: 		None or dataType variableName, why this input is needed / what is 
			its use.  LIST EACH INPUT ARGUMENT ON ITS OWN LINE.
Returns:	None or datatype, what is the data / what is its meaning
 ============================================================================*/
 void configIO(void)
 {
 	ANSELA=0X07;
 	LATA=0X00;
 	TRISA=0XFF;
 	
 	ANSELB=0X00;
 	LATB=0X00;
 	TRISB=0XF0;
 	
 	ANSELC=0X00;
 	LATC=0X00;
 	TRISC=0XF0;
 	
 	ANSELD=0X00;
 	LATD=0X00;
 	TRISD=0XFF;
 	
 	ANSELE=0X00;
 	LATE=0X00;
 	TRISE=0XFF;
 
 }//eo configIO::
 
 /*>>>configADC : ===========================================================
Author:		Name
Date:		DD/MM/YYYY
Modified:	Name & date or None
Desc:		Specifically, what does this function do?
Input: 		None or dataType variableName, why this input is needed / what is 
			its use.  LIST EACH INPUT ARGUMENT ON ITS OWN LINE.
Returns:	None or datatype, what is the data / what is its meaning
 ============================================================================*/
 
 void configADC(void)
 {
 	ADCON0 =0X01;
 	ADCON1=0X00;
 	ADCON2=0XA9;
 }//eo configADC::
 
 /*>>>getADCSample : ===========================================================
Author:		Name
Date:		DD/MM/YYYY
Modified:	Name & date or None
Desc:		Specifically, what does this function do?
Input: 		None or dataType variableName, why this input is needed / what is 
			its use.  LIST EACH INPUT ARGUMENT ON ITS OWN LINE.
Returns:	None or datatype, what is the data / what is its meaning
 ============================================================================*/
 
 int getADCSample(char chID)
{
	ADCON0bits.CHS = chID;
	ADCON0bits.GO = TRUE;
	while(ADCON0bits.GO);
	return ADRES;	
} // eo getADCSample::

/*>>>resetTMR0 : ===========================================================
Author:		Name
Date:		DD/MM/YYYY
Modified:	Name & date or None
Desc:		Specifically, what does this function do?
Input: 		None or dataType variableName, why this input is needed / what is 
			its use.  LIST EACH INPUT ARGUMENT ON ITS OWN LINE.
Returns:	None or datatype, what is the data / what is its meaning
 ============================================================================*/ 

 void resetTMR0(int psc)
{
	TMR0FLAG = FALSE; //reset timer0
	TMR0H = psc >> 8;
	TMR0L = psc;
}// eo resetTMR0::

/*>>>configTMR0: ===========================================================
Author:		Name
Date:		DD/MM/YYYY
Modified:	Name & date or None
Desc:		Specifically, what does this function do?
Input: 		None or dataType variableName, why this input is needed / what is 
			its use.  LIST EACH INPUT ARGUMENT ON ITS OWN LINE.
Returns:	None or datatype, what is the data / what is its meaning
 ============================================================================*/ 

void configTMR0(int psc)
{
	resetTMR0(psc);
	T0CON = 0x93;
}// eo configTMR0::

/*>>>configSP1: ===========================================================
Author:		Name
Date:		DD/MM/YYYY
Modified:	Name & date or None
Desc:		Specifically, what does this function do?
Input: 		None or dataType variableName, why this input is needed / what is 
			its use.  LIST EACH INPUT ARGUMENT ON ITS OWN LINE.
Returns:	None or datatype, what is the data / what is its meaning
 ============================================================================*/

void configSP1()
{
	TXSTA1 = 0x26; // 8 bit transmission, Asynchronous mode
	RCSTA1 = 0x90; // Serial Port disabled, 8 bit reception
	BAUDCON1 = 0x40; 
	SPBRGH1 = 0x00;
	SPBRG1 = 0x19; // 25 Decimal
}// eo configSP1::

/*>>>intSensorCh: ===========================================================
Author:		Name
Date:		DD/MM/YYYY
Modified:	Name & date or None
Desc:		Specifically, what does this function do?
Input: 		None or dataType variableName, why this input is needed / what is 
			its use.  LIST EACH INPUT ARGUMENT ON ITS OWN LINE.
Returns:	None or datatype, what is the data / what is its meaning
 ============================================================================*/

void intSensorCh(sensorCh_t *senPtr)
{
	int index;
	for(index=0;index<SAMPLESIZE;index++)
	{	
		senPtr->samples[index]=FALSE;
	}
	if(senPtr==&sensors[0])
	{
		senPtr->upperLimit=35;
		senPtr->lowerLimit=15;

	}
	if(senPtr==&sensors[1])
	{
		senPtr->upperLimit=65;
		senPtr->lowerLimit=35;
	}
	if (senPtr==&sensors[2])
	{
		senPtr->upperLimit=1400;
		senPtr->lowerLimit=650;
		
	}
	senPtr->avg=0;
	senPtr->insert=0;
	senPtr->avgRdy=0;
	
}//eo intSensorCh::



/*>>>systemInitialization: ===========================================================
Author:		Name
Date:		DD/MM/YYYY
Modified:	Name & date or None
Desc:		Specifically, what does this function do?
Input: 		None or dataType variableName, why this input is needed / what is 
			its use.  LIST EACH INPUT ARGUMENT ON ITS OWN LINE.
Returns:	None or datatype, what is the data / what is its meaning
 ============================================================================*/

void systemInitialization(void)
{
	setOSC4MHZ();
	configIO();
	configADC();
	configTMR0(PRESENTCOUNT);
	configSP1();	
}// eo systemInitialization::
 

/*>>> intstepperMotor: ===========================================================
Author:		Name
Date:		DD/MM/YYYY
Modified:	Name & date or None
Desc:		Specifically, what does this function do?
Input: 		None or dataType variableName, why this input is needed / what is 
			its use.  LIST EACH INPUT ARGUMENT ON ITS OWN LINE.
Returns:	None or datatype, what is the data / what is its meaning
 ============================================================================*/
void intstepperMotor(stepper_t *mtr)
{
	mtr->currentPattern =0x01;
	mtr->patternCounter=0;
	mtr->currentPos=0;
	mtr->setPos=0;
	mtr->motorMov= FALSE;
	
} // eo intstepperMotor ::
/*>>> intPbs: ===========================================================
Author:		Name
Date:		DD/MM/YYYY
Modified:	Name & date or None
Desc:		Specifically, what does this function do?
Input: 		None or dataType variableName, why this input is needed / what is 
			its use.  LIST EACH INPUT ARGUMENT ON ITS OWN LINE.
Returns:	None or datatype, what is the data / what is its meaning
 ============================================================================*/

void intPbs(pbs_t *pb)
{
	pb->chanSel =0;
	pb->mode=FALSE;
	pb->pbState =0;
	pb->pbLaststate= PBMASK;

}//eo intPbs::

/*>>> UpdateDisplay: ===========================================================
Author:		Mihir Jariwala
Date:		27/05/2025
Modified:	None
Desc:		This function updates the display with the current sensor readings and limits.
Input: 		None
Returns:	None
 ============================================================================*/
void UpdateDisplay(void)
{
    printf("\033[2J \033[H");// clear screen
	printf("sensor system (388)");
	printf("Channel: %d", pbs.chanSel);
	printf(pbs.mode ? "\t\tMode: High " : "\t\tMode: Low ");
	printf("\n\n\rTemp: %d °C\t\tHumd: %d%%\t\tCO2: %4d ppm", sensors[0].avg, sensors[1].avg, sensors[2].avg);
	printf("\n\r");
	printf("HL:  %3d °C\t\tHL:  %3d%%\t\tHL:  %4d ppm", sensors[0].upperLimit, sensors[1].upperLimit, sensors[2].upperLimit);
	printf("\n\r");
	printf("LL:  %3d °C\t\tLL:  %3d%%\t\tLL:  %4d ppm", sensors[0].lowerLimit, sensors[1].lowerLimit, sensors[2].lowerLimit);
}

void changeMode()
{
	pbs.mode = !pbs.mode;
	
}//eo changeMode:: 

void changeChannel()
{
	pbs.chanSel++;
	if(pbs.chanSel>=SENSORCOUNT)
	{
		pbs.chanSel=0;
	}

}//eo changeChannel::

void incrementLimit() 
{
	if(pbs.mode)
	{
		sensors[pbs.chanSel].upperLimit++;
	}	
	else
	{
		sensors[pbs.chanSel].lowerLimit++;
	}
}//eo incrementLimit::


void decrementLimit()
{
	if(pbs.mode)
	{
		sensors[pbs.chanSel].upperLimit--;
	}	
	else
	{
		sensors[pbs.chanSel].lowerLimit--;
	}


}//eo decrementLimit::






/*=== MAIN: FUNCTION ==========================================================
 ============================================================================*/
void main( void )
{
	char count = 0, chID = 0;
	char i;
	long sum;
	float tempC = 0;
	float rh = 0;
	float ppm = 0;
	int rawADC = 0;
	float volts = 0;
	systemInitialization();
	intstepperMotor(&vent);
	intPbs(&pbs);
	for (chID = 0; chID < SENSORCOUNT; chID++) 
	{
        intSensorCh(&sensors[chID]);
    }
	while(1)
	{
		
		pbs.pbState = PORTA & PBMASK;
        if (pbs.pbState != NOPRESS && pbs.pbState != pbs.pbLaststate) 
	    {
            pbs.pbLaststate = pbs.pbState;
            Delay10KTCYx(DEBOUNCE); // Debounce 
            switch (pbs.pbState) 
	        {
                case MODE: 
                changeMode();
                break;
                
                case CHANSEL: 
                changeChannel();
                break;
                
                case INCREMENT: 
                incrementLimit(); 
                break;
                
                case DECREMENT: 
                decrementLimit(); 
                break;
                
                default: 
                break;
            }
        } else if (pbs.pbState == NOPRESS) 
        {
            pbs.pbLaststate = PBMASK;
        }

        if (TMR0FLAG) 
	    {
            resetTMR0(PRESENTCOUNT);
            count++;
            if (count == ONESEC) 
	        {
                count = 0;
                for (chID = 0; chID < SENSORCOUNT; chID++) 
	            {
                    sensors[chID].samples[sensors[chID].insert] = getADCSample(chID);
                    sensors[chID].insert++;
                    if (sensors[chID].insert >= SAMPLESIZE) 
	                {
                        sensors[chID].insert = 0;
                        sensors[chID].avgRdy = TRUE;
                    }
                    if (sensors[chID].avgRdy) 
	                {
                         sum = 0;
                        for ( i = 0; i < SAMPLESIZE; i++)
                        {
                         	sum += sensors[chID].samples[i];
                        }
                        rawADC = (int)sum / SAMPLESIZE; // Calculate average
                        volts = rawADC * ADCRES; // Convert to volts
                        if(chID == 0) // Temperature Sensor
                        {
                            tempC = (volts - TEMPB) / TEMPM; // Calculate temperature in Celsius
                            sensors[chID].avg = (int)tempC; // Store average temperature
                        }
                        if(chID == 1) // Humidity Sensor
                        {
                            rh = volts / CONVPER; // Calculate relative humidity
                            sensors[chID].avg = (int)rh; // Store average humidity
                        }
                        if(chID == 2) // CO2 Sensor
                        {
                            ppm = volts / CO2M; // Calculate CO2 ppm
                            sensors[chID].avg = (int)ppm; // Store average CO2 ppm
                        }
                		sensors[chID].avgRdy = FALSE;
                }
                // Convert ADC averages to real-world units
                         
         	}
            UpdateDisplay(); // Update the display with sensor data 
         	
         }
        }
         
 } //eo while 
 	
}// eo main:
	        