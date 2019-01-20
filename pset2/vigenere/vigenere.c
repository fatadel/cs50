// Pseudocode

// check the keyword
// prompt for a plain text
// iterate the plain text enciphering char by char
// print out the cipher text

// The task is pretty similar to Caesar's cipher regarding that we need to find out the key on each step

#include <ctype.h>
#include <cs50.h>
#include <stdio.h>
#include <string.h>

#define ALPHABET_SIZE 26

bool isAlphaStr(string s); // we need to check the provided keyword whether it contains only alphabetical characters

// getting the key from keyword, where k is a keyword, i - counter for alphabetical characters in the plain text
int getKey(string k, int i);


int main(int argc, string argv[])
{
    if (argc != 2 || !isAlphaStr(argv[1])) // finish program with error if it was started in an unappropriate way
    {
        printf("Usage: ./vigenere KEYWORD, where KEYWORD is a non-zero sequence of alphabetical characters\n");
        return 1;
    }

    string k = argv[1]; // the keyword
    string p = get_string("plaintext:  "); // get the plain text from user
    int count = 0; // counter for alphabetical characters in the plain text

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
                p[i] = (p[i] - 'a' + getKey(k, count)) % ALPHABET_SIZE;
                p[i] += 'a';
            }
            else if (isupper(p[i])) // formula for upper case letters
            {
                p[i] = (p[i] - 'A' + getKey(k, count)) % ALPHABET_SIZE;
                p[i] += 'A';
            }
            count++;
        }

    }

    printf("ciphertext: %s\n", p); // print the cipher text
    return 0;
}

bool isAlphaStr(string s)
{
    for (int i = 0, n = strlen(s); i < n; i++)
    {
        if (!isalpha(s[i])) // if any char in the string is non-alphabetical, the whole string is non-alphabetical
        {
            return false;
        }
    }

    return true; // every char passed the test, the string is alphabetical :)
}

int getKey(string k, int i)
{
    i %= strlen(k); // wrap around the keyword, so that position of a letter in the keyword can not go above its length

    if (islower(k[i]))
    {
        return k[i] - 'a'; // convert the letter from "ASCII order" to its alphabetical order(from 0 to 25), that is the key
    }

    return k[i] - 'A';
}
