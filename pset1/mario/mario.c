#include <cs50.h>
#include <stdio.h>

int main()
{
    // Prompt for positive integer
    int height;
    do 
    {
        height = get_int("Height: ");
    }
    while (height > 8 || height < 1);
    
    for (int i = 1; i <= height; i++)
    {
        for (int w = 0; w < height - i; w++)
        {
            printf(" ");
        }
        
        for (int j = 0; j < i; j++)
        {
            printf("#");
        }
        
        printf("  ");
        
        for (int k = 0; k < i; k++)
        {
            printf("#");
        }
        
        printf("\n");
    }
}