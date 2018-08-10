#include <cs50.h>
#include <stdio.h>
#include <math.h>

int main(void)
{
    int q = 25, d = 10, n = 5, p = 1;
    float owed;
    
    do
    {
        printf("How much change is owed?: \n");
        owed = get_float();
    }
    while(owed < 0);
    
    float change = owed * 100;
    
    int cents = (int) round(change);
    
    int counter = 0;
    
    while (cents >= q)
    {
        cents -= q;
        counter++;
    }
    while (cents >= d)
    {
        cents -= d;
        counter++;
    }
    while (cents >= n)
    {
        cents -= n;
        counter++;
    }
    while (cents >= p)
    {
        cents -= p;
        counter++;
    }
    printf("%i\n", counter);
   
}