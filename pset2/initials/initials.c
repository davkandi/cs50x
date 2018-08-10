#include <cs50.h>
#include <stdio.h>
#include <ctype.h>

int main(void)
{
    string s = get_string();
    
    printf("%c", toupper(s[0]));
    int i = 0;
    
    while(s[i] != '\0')
    {
        if(s[i] == ' ')
        {
            printf("%c", toupper(s[i + 1]));
        }
        i++;
    }
    printf("\n");
}