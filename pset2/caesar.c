#include <cs50.h>
#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

bool is_number(char* input);

int main (int argc, string argv[])
{
    //Prompt user for key (commandline argument)
    if (argc != 2 || !is_number(argv[1]))
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }

    //Convert string into integer?
    int k = atoi(argv[1]) % 26;

    //Prompt user for plaintext
    string plaintext = get_string("Plaintext: ");

    int length = strlen(plaintext);
    printf("ciphertext: ");
    for (int i = 0; i < length; i++)
    {
        if (isupper(plaintext[i]))
        {
            printf("%c", (plaintext[i] - (int) 'A' + k) % 26 + (int) 'A');
        }
        
        else if (islower(plaintext[i]))
        {
            printf("%c", (plaintext[i] - (int) 'a' + k) % 26 + (int) 'a');
        }
        
        else {
            printf("%c", plaintext[i]);
        }
    }

    printf("\n");
    return 0;
}

bool is_number(char* input)
{
    for (int i = 0, length = strlen(input); i < length; i++)
    {
         if (!isdigit(input[i]))
         {
             return false;
         }
    }
    return true;
}