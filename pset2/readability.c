#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <ctype.h>


//Prototypes
int count_letters(string text);
int count_words(string text);
int count_sen(string text);


int main(void)
{
    // Prompt user for input
    string text = get_string("Text: ");

    // Count the number of letters, words and sentences
    int letters = count_letters(text);
    float words = (float) count_words(text);
    int sentences = count_sen(text);

    /*
    The Coleman-Liau index, where:
    L is the average number of letters per 100 words in the text,
    S is the average number of sentences per 100 words in the text.
    */
    float L = ((letters * 100) / words);
    float S = ((sentences * 100) / words);

    float index = 0.0588 * L - 0.296 * S - 15.8;

    // Print "Grade X" where X is the grade level computed by the Coleman-Liau formula, rounded to the nearest integer
    int grade = (int) round(index);

    if (grade < 1)
    {
        printf("Before Grade 1\n");
        return 0;
    }

    if (grade < 16)
    {
        printf("Grade %i\n", grade);
        return 0;
    }

    printf("Grade 16+\n");
}


//Determine number of letters
int count_letters (string text)
{
    int letters = 0;
    for (int i = 0, len = strlen(text); i < len; i++)
    {
        //check for alphabeticals
        if (isalpha(text[i]))
        {
            letters++;
        }
    }

    return letters;
}

//Determine number of words
int count_words(string text)
{
    int words = 1;
    for (int j = 0, len = strlen(text); j < len; j++)
    {
        //check for whitespace
        if (isspace(text[j]))
        {
            words++;
        }
    }

    return words;
}

//Determine number of sentences
int count_sen(string text)
{
    int sentences = 0;
    for (int k = 0, len = strlen(text); k < len; k++)
    {
        //check for punctuation
        if (text[k] == '.' || text[k] == '!' || text[k] == '?')
        {
            sentences++;
        }
    }

    return sentences;
}