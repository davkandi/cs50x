/**
 * Implements a dictionary's functionality.
 */

#include <stdio.h>
#include <cs50.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#include "dictionary.h"

#define SIZE 17576 // 26 * 26 * 26 hashing using the first 3 characters

typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

//initialize the hashtable with NULL values
node *hashtable[SIZE] = {NULL};
//words counter
int nwords = 0;

//hash function
int hash(char *word)
{
    int first, second, third;

    first = (int)tolower(word[0]) - 97;

    if (word[1] == '\0')
    {
        second = 0;
        third = 0;
    }
    else if (word[1] != '\0' && word[2] == '\0')
    {
        second = (int)tolower(word[1]) - 97;
        third = 0;
    }
    else
    {
        second = (int)tolower(word[1]) - 97;
        third = (int)tolower(word[2]) - 97;
    }

    int index = first * 26 * 26 + second * 26 + third;

    return index;
}

/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char *word)
{
    // TODO
    char cp[LENGTH + 1];
    int n = strlen(word);
    for (int i = 0; i < n; i++)
    {
        cp[i] = tolower(word[i]);
    }
    cp[n] = '\0';
    int index = hash(cp);

    if (hashtable[index] == NULL)
    {
        return false;
    }
    else
    {
        node* ptr = hashtable[index];
        while (ptr != NULL)
        {
            if (strcmp(ptr->word, cp) == 0)
            {
                return true;
            }
            else
            {
                ptr = ptr->next;
            }
        }
        return false;
    }
}

/**
 * Loads dictionary into memory. Returns true if successful else false.
 */
bool load(const char *dictionary)
{
    // TODO
    FILE *file = fopen(dictionary, "r");

    if (file == NULL)
    {
        fprintf(stderr, "Couldn't open the dictionary file\n");
        return false;
    }

    char *buffer = malloc((LENGTH + 1) * sizeof(char));

    while(fscanf(file, "%s", buffer) != EOF)
    {
        node *new_node = malloc(sizeof(node));
        strcpy(new_node->word, buffer);
        new_node->next = NULL;

        int index = hash(buffer);

        if (hashtable[index] == NULL)
        {
            hashtable[index] = new_node;
        }
        else
        {
            node *cursor = hashtable[index];
            while (cursor->next != NULL)
            {
                cursor = cursor->next;
            }
            cursor->next = new_node;
        }
        nwords++;
    }
    fclose(file);
    free(buffer);

    return true;
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    // TODO
    return nwords;
}

/**
 * Unloads dictionary from memory. Returns true if successful else false.
 */
bool unload(void)
{
    // TODO
    for (int i = 0; i < SIZE; i++)
    {

        if (hashtable[i] != NULL)
        {
            node *cursor = hashtable[i];
            while (cursor != NULL)
            {
                node *temp = cursor;
                cursor = cursor->next;
                free(temp);
            }
        }
    }
    return true;
}
