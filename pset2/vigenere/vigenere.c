#include <stdio.h>
#include <cs50.h>
#include <ctype.h>
#include <string.h>

int main(int argc, string argv[])
{
    string k = argv[1];
    if (argc != 2)
    {
        printf("you can use only two command line arguments\n");
        return 1;
    }
    else
    {
        for (int i = 0, m = strlen(argv[1]); i < m; i++)
        {
            if (isalpha(k[i]) == 0)
            {
                printf("You can just use A-Z or a-z characters\n");
                return 1;
            }
        }
    }
    
    printf("Plaintext: ");
    string p = get_string(); //plaintext
    
    int j; //key index
    int length = strlen(k);
    int pos = 0; //position
   
    printf("Ciphertext: ");
    
    for (int i = 0, n = strlen(p); i < n; i++) 
    {                  
          
            if (isupper(p[i]))
            {   
                if (isupper(k[pos % length]))
                    j = (k[pos % length] - 65);
                else 
                    j = (k[pos % length] - 97);
                    
                p[i] = (p[i] - 'A' + j) % 26 + 'A';
                printf("%c", p[i]);
                pos++;
            }
            else if (islower(p[i]))
            {
                if (isupper(k[pos % length]))
                    j = (k[pos % length] - 65);
                else 
                    j = (k[pos % length] - 97);
                    
                p[i] = (p[i] - 'a' + j) % 26 + 'a';
                printf("%c", p[i]);
                pos++; 
            }
            else
            { 
                printf("%c", p[i]);
            }
    }
    printf("\n");
    return 0;
}