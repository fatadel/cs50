#include <ctype.h>
#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ALPHABET_SIZE 26

int main(int argc, string argv[])
{
    if (argc != 2) // finish program with error if it was started in an unappropriate way
    {
        printf("Usage: ./caesar k, where k is a non-negative number for \"key\"\n");
        return 1;
    }

    int k = atoi(argv[1]); // convert the key from string to int using atoi function from stdlib.h
    string p = get_string("plaintext:  "); // get the plain text from user

    for (int i = 0, n = strlen(p); i < n; i++) // iterate the plain text; encipher char by char
    {
        if (isalpha(p[i])) // we need to encipher only the letters
        {
            if (islower(p[i])) // formula for lower case letters
            {
                // p[i] - 'a' converts the "ASCII order" of a character to the alphabetical order, appliable in the formula
                // p[i] += 'a' converts it back to ASCII order
                // the same applies down for upper case letters
                // we save the cipher text over the same variable p since there is no need for the plain text in the future
                p[i] = (p[i] - 'a' + k) % ALPHABET_SIZE;
                p[i] += 'a';
            }
            else if (isupper(p[i])) // formula for upper case letters
            {
                p[i] = (p[i] - 'A' + k) % ALPHABET_SIZE;
                p[i] += 'A';
            }
        }

    }

    printf("ciphertext: %s\n", p); // print the cipher text
    return 0;
}