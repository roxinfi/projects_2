	// Functions  =================================================================

/*>>> setOSC4mhz: ===========================================================
Author:Rushi K Trivedi
Date:22/01/2025
Modified:None
Desc:This function configures the internal oscillator of the PIC18F45K22 microcontroller to run at a frequency of 4 MHz.
Input:None 
Returns:None 
 ============================================================================*/
void setOSC4mhz(void)
{
	OSCCON=0x52;
	while(!OSCCONbits.HFIOFS);
	
} // eo setOSC4mhz::

/*>>> configIO:: ===========================================================
Author:	Rushi K Trivedi
Date:29/01/2025
Modified:None
Desc:The configIO function sets the analog pins to digital mode for each port. It does this by
     writing 0x00 to the ANSELB, ANSELC, ANSELD, and ANSELE registers, which disables the analog functionality and 
     for PORTA are set to analog mode by writing 0x07 to ANSELA.
Input:None 
Returns:None 
 ============================================================================*/
void configIO (void)
{
	ANSELA=0x07;	 // Set RA0, RA1, and RA2 as analog inputs and rest as digital 
	LATA=0x00;	 // clear the PortA output and initialize to LOW
	TRISA=0xFF; 	 // Set all PORTA pins as inputs

	ANSELB=0x00; 	// Set all PORTB pins as digital
	LATB=0x00;	// clear the PortB output and initialize to LOW
	TRISB=0xF0; 	// Set upper nibble of PORTB as input

	ANSELC=0x00; 	// Set all PORTC pins as digital
	LATC=0x00;	// clear the PortC output and initialize to LOW
	TRISC=0xF0; 	// Set upper nibble of PORTC as input

	ANSELD=0x00; 	// Set all PORTD pins as digital
	LATD=0x00;   	// clear the PortD output and initialize to LOW
	TRISD=0xFF;  	// Set all PORTD pins as inputs

	ANSELE=0x00; 	// Set all PORTE pins as digital
	LATE=0x00;	// clear the PortE output and initialize to LOW
	TRISE=0xFF; 	// Set all PORTE pins as inputs
	
} // eo configIO::

/*>>> configADC : ===========================================================
Author:	RUSHI K TRIVEDI
Date:	08/10/2024
Modified: None
Desc: This Function configure the settings of the Analog-Digital converter.In ADCON0 we have to choose the channelid .
      In ADCON1 we have taken standard voltage.In ADCON2 we have taken 12 Tad,Fosc/8 and Right justified. 
Input: 	None	
Returns: None	
 ============================================================================*/
void configADC(void)
{ 
	ADCON0= 0x01;
	ADCON1= 0x00;
	ADCON2=0xAC;

}// eo configADC ::

/*>> getADCsample : ===========================================================
Author:	RUSHI K TRIVEDI
Date:	08/10/2024
Modified: None
Desc: Function performs analog-digital conversion on specific channel 	
Input: 	Char chID,The ADC channel number to be selected for conversion.	
Returns: int returns an integer value representing the digital 
 ============================================================================*/
int getADCsample(char chID)
{
	ADCON0bits.CHS=chID;
	ADCON0bits.GO=TRUE;
	while(ADCON0bits.GO);
	return ADRES;

}// getADCSAMPLE ::

/*>>>configSP1  : ===========================================================
Author:	RUSHI K TRIVEDI
Date: 05/11/2024
Modified: None
Desc: This function configures the Serial Peripheral Interface (SPI) for communication using 
      the microcontroller's USART1 module. It sets up the TXSTA1: Configures the transmitter for 
      asynchronous mode, enabling transmission,RCSTA1: Configures the receiver, enabling continuous 
      reception,BAUDCON1, SPBRGH1, SPBRG1: Set the baud rate for communication (9600 baud).
Input: 	None	
Returns: None	
 ============================================================================*/
void configSP1 (void)
{
	TXSTA1 = 0x26;
	RCSTA1 = 0X90;
	BAUDCON1=0x40;
	SPBRGH1=0x00;
	SPBRG1=0x19;
		
} //configSP1 eo ::

/*>>> resentTMR0  : ===========================================================
Author:	RUSHI K TRIVEDI
Date:	29/01/2025
Modified: None
Desc: The resetTMR0 function resets Timer 0 by clearing the interrupt flag and setting the timer 
      value to TMR0H to 0x3C and the low byte TMR0L to 0xB0
Input:None	
Returns: None	
 ============================================================================*/
void resetTMR0(int psc)
{
	TMR0FLAG=FALSE; //RESET TIMER0
	TMR0H=psc>>8;
	TMR0L=psc;

}// eo resentTMR0::

/*>>> configTMR0  : ===========================================================
Author:	RUSHI K TRIVEDI
Date:	29/01/2025
Modified: None
Desc: This function configure Timer0 for use in the application also intialiaze timer with specific prescale value .
Input: 	None	
Returns: None	
 ============================================================================*/
void configTMR0(void)
{
	T0CON=0x90;

}// eo configTMR0 ::

/*>>> SystemInitialization  : ===========================================================
Author:	RUSHI K TRIVEDI
Date:	29/01/2025
Modified: None
Desc: The systemInitialization function initializes the system by setting the oscillator, configuring I/O ports, ADC, SPI, and Timer 0
Input: 	None	
Returns: None	
 ============================================================================*/
void systemInitialization (void)
{
	setOSC4mhz();
	configIO();
	configADC();
	configSP1();
	configTMR0();

}// eo systemInitialization  ::	

void main(void)
{
char count=0; // Initialize a counter
	char chID=0;
	systemInitialization();
	for(chID=0; chID<SENSORCOUNT; chID++)  // Initialize each sensor channel by resetting its data
	{
		intSensorCh(&sensors[chID]);
	} //eo for
	while(TRUE)
	{
		if(TMR0FLAG)
		{
			resetTMR0(PRESETCOUNT); // Reset Timer0 and clear the interrupt flag
			count++;
			if(count == ONESEC)
			{
				count=0;
				for(chID=0; chID<SENSORCOUNT; chID++)
				{
					sensors[chID].samples[sensors[chID].insert]=getADCsample(chID); //// Get ADC sample for the current sensor and store it
					sensors[chID].insert++; // Reset the insert index
					if(sensors[chID].insert>=SAMPSIZE)
					{
						sensors[chID].insert=0;
						sensors[chID].avgRdy=TRUE;
					} //eo if
					if(sensors[chID].avgRdy)
					{
						long sum=0; // Variable to hold the sum of samples
						char index=0;
						for(index=0; index<SAMPSIZE; index++)
						{
							sum+=sensors[chID].samples[index];
						}
						sensors[chID].avg=sum/SAMPSIZE; // Calculate the average and store it
						if(chID == 0)
						{
							//temperature=((sensors[0].avg*ADCRES)-TEMPB)/TEMPM			
							temperature	= sensors [0]. avg*ADCRES;
							temperature=temperature-TEMPB;
							temperature=temperature / TEMPM;
							if (chID== 1)
							{
								sensors [1]. avg = sensors [1].avg * CONVER;
							}
							if (chID == 2)
							{
								sensors[2].avg *=CONVPPM;
								temperature	= sensors [0]. avg*ADCRES;
							temperature=temperature-CO2;
							temperature=temperature / CO1;
							} //
							
						} //eo if
					} //eo for
						printf("%d%c,%d,d",temperature 248,sensor[1].avg,sensor[2].avg) // Update the display with the latest sensor data
				} //eo if
			} //eo if
		} //eo while
	}
}
