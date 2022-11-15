#include <cs50.h>
#include <stdio.h>


int main ()
{
    //prompt for start size
    int start;
    do
    {
        start = get_int("Start size: ");
    }
    while (start < 9);

    //prompt for end size
    int end;
    do
    {
        end = get_int("End size: ");
    }
    
    while (end < start);

    // Keep track of number of years
    int years = 0;
    
    // Updating untill treshold
    while (start < end)
    {
        start = start + (start / 3) - (start / 4);
        years++;
    }
    
    // Print number of years
    printf("Years: %i\n", years);
}