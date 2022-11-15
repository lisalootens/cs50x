#include <cs50.h>
#include <stdio.h>

int get_positive_int_start();
int get_positive_int_end(int end);

int main(void)
{
    int start_size = get_positive_int_start();
    int end_size = get_positive_int_end(start_size);
    int years = end_size - start_size;
   
     printf("Years: %i", years);
}

// prompt user for positive start integer
int get_positive_int_start()
{
    int n;
    do
    {
        n = get_int("Positive Integer start: ");

    }
    while (n < 9);
    return n;
}

// prompt user for positive end integer
int get_positive_int_end(int end)
{
    int n;
    do
    {
        n = get_int("Positive Integer end: ");

    }
    while (n <= end);
    return n;
}