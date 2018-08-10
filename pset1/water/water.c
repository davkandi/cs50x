#include <stdio.h>
#include <cs50.h>

int main(void)
{
    printf("minutes: ");
    int m = get_int();
    printf("bottles: %i\n", m * 12);

}