//DONE

#include <stdbool.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 26;

// Hash table
node *table[N];

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    int h = hash(word);

    node *c = table[h];
    while (c != NULL)
    {
        if (strcasecmp(c->word, word) == 0)
        {
            return true;
        }
        c = c->next;
    }

    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    return tolower(word[0]) - 'a';
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    for (int i = 0; i < N; i++)
    {
        table[i] = NULL;
    }

    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        printf("Missing file? Not enough memory could be reserved");
        return false;
    }

    // Buffer for a word
    char word[LENGTH + 1];

    while (fscanf(file, "%s", word) != EOF)
    {
        // Hash the word
        int h = hash(word);

        // Create new node
        node *n = malloc(sizeof(node));
        if (n == NULL)
        {
            printf("Out of memory");
            return false;
        }

        strcpy(n->word, word);
        n->next = NULL;

        if (table[h] == NULL)
        {
            table[h] = n;
        }
        else
        {
            node *tmp = table[h];
            while (tmp->next != NULL)
            {
                tmp = tmp->next;
            }
            tmp->next = n;
        }
    }

    fclose(file);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    int count = 0;
    for (int i = 0; i < N; i++)
    {
        node *c = table[i];
        while (c != NULL)
        {
            count++;
            c = c->next;
        }
    }

    return count;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    for (int i = 0; i < N; i++)
    {
        node *c = table[i];
        while (c != NULL)
        {
            node *tmp = c->next;
            free(c);
            c = tmp;
        }
    }

    return true;
}
