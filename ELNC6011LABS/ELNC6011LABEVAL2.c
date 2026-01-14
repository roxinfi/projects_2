#define BUFFER_SIZE 64
#define TOKENSIZE 32
#define CONLIM 0
#define ADDTO 1
#define MYADDRES 2
#define CHID 3
#define MODE 4
#define NEWLIMIT 5


typedef char flag_t; // Define flag type for boolean values

typedef struct{
    int Buff[BUFFER_SIZE];
    char insert; // Index for inserting new sample
    flag_t sentrdyflag;
} comBuf_t;

comBuf_t rcvr; // Global receiver buffer

char tokens[TOKENSIZE];

// parsingSentence()
// ValidationSentence()

void InitializeComBuff(comBuf_t *ptr)
{
    int index = 0;
    ptr->insert = 0; // Initialize insert index
    ptr->sentrdyflag = 0; // Reset send ready flag
    for(index = 0; index < BUFFER_SIZE; index++)
    {
        ptr->Buff[index] = 0; // Initialize buffer to zero
    }
} // eo InitializeComBuff::


void ExecutionSentence()
{
    printf("%s", tokens[CONLIM]);
    printf("\n\r%s", tokens[ADDTO]);
    printf("\n\r%s", tokens[MYADDRES]);
    printf("\n\r%s", tokens[CHID]);
    printf("\n\r%s", tokens[MODE]);
    printf("\n\r%s", tokens[NEWLIMIT]);
}


void main()
{
    // SystemInit();
    // InitializeCOMBuff();
    while(1)
    {
        if(rcvr.sentrdyflag)
            rcvr.sentrdyflag = 0; // Reset send ready flag
            if(validationSentence(rcvr.Buff))
            {
                parsingSentence(rcvr.Buff);
                ExecutionSentence();
            }
    }
}

void ISR()
{

    if(RC1FLAG)
    {
        char hold = RCREG1;
        if(hold == '$')
        {
            rcvr.insert = 0; // Reset insert index
        }
        if(hold == '\r')
        {
            hold = 0;
            rcvr.sentrdyflag = 1; // Set send ready flag
        }
        rcvr.Buff[rcvr.insert] = hold;
        rcvr.insert++; // Increment insert index
    }
}

ANSELA = 0x00;
LATA = 0x00;
TRISA = 0xFF;


ANSELB = 0x00;
LATB = 0x00;
TRISB = 0xFF;


// TXSTA1 = 0x26;
// RCSTA1 = 0x90;
// BAUDCON1 = 0x40;
// SPBRGH1 = 0x00;
SPBRG1 = 0x10;

INTICON = 0xE0;
RCONbits.IPEN = 0; // Disable priority levels
PIR1bits.RC1IF = 0; // Clear USART1 receive interrupt flag
PIE1bits.RC1IE = 1; // Enable USART1 receive interrupt


// $CONLIM,1,555,2,1,1370,0\r