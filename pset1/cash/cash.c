#include <cs50.h>
#include <stdio.h>
#include <math.h>

int main()
{
    int coins = 0;
    
    // Ask for owed change
    float change;
    do 
    {
        change = get_float("Change owed: ");
    }
    while (change <= 0);


    // Convert to cents
    int cents = round(change * 100);
    
    
    // Loop conditions for quarters 25¢, dimes 10¢, nickels 5¢ and pennies 1¢
    while (cents >= 25)
    { 
        cents = cents - 25;
        coins++;
    }
    
    while (cents >= 10)
    { 
        cents = cents - 10;
        coins++;
    }
    
    while (cents >= 5)
    { 
        cents = cents - 5;
        coins++;
    }
    
    while (cents >= 1)
    { 
        cents = cents - 1;
        coins++;
    }

    
    // Print number of coins used
    printf("%i coins\n", coins);
    
}