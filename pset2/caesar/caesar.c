#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

int main(int argc, string argv[])
{
    int k;
    if(argc != 2)
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }
    else if (atoi(argv[1]) < 1)
    {
        printf("Key must be a non-negative integer\n");
        return 2;
    }
    else
    {
        k = atoi(argv[1]);
    }
    
    printf("Plaintext: ");
    string p = get_string(); //plaintext
    
    printf("Ciphertext: ");
    
    for (int i = 0, n = strlen(p); i < n; i++)
    {
        if(islower(p[i]))
        {
            printf("%c", (p[i] + k - 'a') % 26 + 'a');
        }
        else if(isupper(p[i]))
        {
            printf("%c", (p[i] + k - 'A') % 26 + 'A');
        }
        else
        {
            printf("%c", p[i]);
        }
    }
    
    printf("\n");
    
    return 0;
}