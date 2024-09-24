#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./recover FILE\n");
        return 1;
    }

    FILE *card = fopen(argv[1], "r");

    if (card == NULL)
    {
        printf("Error opening file\n");
        return 1;
    }

    int block_size = 512;

    uint8_t buffer[block_size];

    int file_number = 0;

    char filename[8];

    FILE *img = NULL;

    while (fread(buffer, 1, 512, card) == 512)
    {

        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff &&
            (buffer[3] & 0xf0) == 0xe0)
        {
            if (file_number == 0)
            {
                sprintf(filename, "%03i.jpg", 0);

                img = fopen(filename, "w");

                if (img == NULL)
                {
                    printf("Error opening file\n");
                    return 1;
                }

                fwrite(buffer, 1, 512, img);
                file_number++;
            }

            else
            {
                fclose(img);
                sprintf(filename, "%03i.jpg", file_number++);

                img = fopen(filename, "w");
                if (img == NULL)
                {
                    printf("Error opening file\n");
                    return 1;
                }
                fwrite(buffer, 1, 512, img);
            }
        }

        else if (img != NULL)
        {
            fwrite(buffer, 1, 512, img);
        }
    }

    if (img != NULL)
    {
        fclose(img);
    }
    fclose(card);
    return 0;
}
Ï
