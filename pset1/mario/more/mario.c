#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int n; // the height

    // Prompt user to enter the half-pyramid’s height, must be >= 0 and <=23
    // Prompt once, if n is not desired number - prompt again
    do
    {
        n = get_int("Height: ");
    }
    while (n < 0 || n > 23);

    for (int i = 0; i < n; i++) // number of lines
    {
        for (int s = 0; s < n - 1 - i; s++) // s - for spaces, number of "left" spaces on each line
        {
            printf(" ");
        }
        for (int h = 0; h < i + 1; h++) // h - for hashes, number of "left" hashes on each line
        {
            printf("#");
        }
        printf("  "); // always two spaces between the half-pyramids
        for (int h = 0; h < i + 1; h++) // number of "right" hashes on each line, equal to number of "left" hashes on each line
        {
            printf("#");
        }
        printf("\n");
    }
}