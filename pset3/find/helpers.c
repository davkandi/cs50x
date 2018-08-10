/**
 * helpers.c
 *
 * Computer Science 50
 * Problem Set 3
 *
 * Helper functions for Problem Set 3.
 */
       
#include <cs50.h>

#include "helpers.h"

/**
 * Returns true if value is in array of n values, else false.
 */
bool search(int value, int values[], int n)
{
   // TODO: implement a searching algorithm
     if (n > 1)
     {
        int min = 0, max = n - 1;
        int middle;
        while (min <= max)
        {
            middle = (min + max) / 2;
            if (values[middle] == value)
            {
                return true;
            }
            else if (values[middle] < value)
            {
                min = middle + 1;
            }
            else if (values[middle] > value)
            {
                max = middle - 1;
            }
        }
    
      }
      return false;   
    
}

/**
 * Sorts array of n values.
 */
void sort(int values[], int n)
{
    // TODO: implementing an O(n^2) sorting algorithm
    for (int i = 0; i < n; i++)
    {
        int min = i;
        for (int j = i + 1; j < n; j++)
        {
            if (values[j] < values[min])
            {
                min = j;
            }
        }
        int tmp = values[i];
        values[i] = values[min];
        values[min] = tmp;
    }
}

