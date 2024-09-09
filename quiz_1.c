#include<stdio.h>


char gibberish[50] = "abc123xyzzzz56pqrtttuuvv77aaaopmmnnmqqx";

int repart()
{
    int count = 0;
    char input;
    printf("Enter a character: ");
    scanf("%c", &input);
    for (int i = 0; i < 50; i++)
    {
        if (gibberish[i] == input)
        {
            count++;
        }
    }
    printf("The character %c appears %d times in the string.\n", input, count);
}

int main()
{
    repart();
    return 0;
}