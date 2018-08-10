#include <stdio.h>
#include <stdint.h>

typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: ./recover filename\n");
        return 1;
    }

    char *filename = argv[1];

    // open the file
    FILE *file = fopen(filename, "r");

    if (file == NULL)
    {
        fclose(file);
        fprintf(stderr, "couldn't open %s.\n", filename);
        return 2;
    }

    // counter
    int counter = 0;

    BYTE buffer[512];

    char title[10];

    // initialize output file
    FILE *outptr = NULL;


    // until the end of file
    while (!feof(file))
    {

        // check first bytes of the jpg
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] == 0xe0 || buffer[3] == 0xe1))
        {
            // if already exist, then close file
            if (outptr != NULL)
            {
                fclose(outptr);

            }

            // labeling files
            sprintf(title, "%03d.jpg", counter);

            outptr = fopen(title, "w");

            // update the counter
            counter++;

            // new file with buffer
            fwrite(buffer, sizeof(buffer), 1, outptr);
        }
        else if (outptr != NULL)
        {
            // continue to write the file
            fwrite(buffer, sizeof(buffer), 1, outptr);

        }

        fread(buffer, sizeof(buffer), 1, file);

    }

    // close the file
    fclose(file);


    return 0;
}