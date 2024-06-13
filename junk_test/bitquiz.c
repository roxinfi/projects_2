#include<stdio.h>
int main(int argc, const char * argv[]) 

{

     unsigned char switches;

     switches = 0x58;

    

    unsigned char switch1 = switches & 0xF0;

    unsigned char switch2 = switches & 0x0F;

    switch1 = switch1 >> 4;

    printf("sw2:%02X  sw1:%02X\n", switch2, switch1);

    

    unsigned char value = 148;

    value = value | 0x02;

    printf("Value contains: %i\n", value);

    return 0;

}