#include <cs50.h>
#include <stdio.h>

int main(void)
{
  
    int height;
    do
    {
        printf("Height: ");
        height = get_int();
    }
    while (height < 0 || height >== 23);
    
    int hashes = 2;
    int spaces = height - 1;
    
    for (int i = 0; i < height; i++)
    {
        for(int j = 0; j < spaces; j++)
        {
            printf(" ");
        }
        spaces--;
        
        for(int k = 0; k < hashes; k++)
        {
            printf("#");
        }
        hashes++;
        
        printf("\n");
        
    }
}