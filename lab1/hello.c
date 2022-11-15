#include <cs50.h>
#include <stdio.h>

int main(void)
{
    string name = get_string("What is your name? "); // Prompt user for input 
    printf("Hello, %s", name);
}