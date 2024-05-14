#include<stdio.h>
#include<conio.h>

int main(void)
{
	printf("\nhello world\n");
	
	float Pressure, Volume, Temperature, mole, Constant;

	Constant = 8 * 10 ^ -34;
	
	mole = 2;
	
	Temperature = 100;
	
	Volume = 5;
	
	Pressure = (mole * Constant * Temperature) / Volume;

	printf(" The Pressure is %f Bar", Pressure);

	return 0;
}