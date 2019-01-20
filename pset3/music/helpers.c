// Helper functions for music

#include <cs50.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

#include "helpers.h"

#define A4_DURATION 440 // duration of the A4 note is 440 Hz, that's the basis

// Converts a fraction formatted as X/Y to eighths
int duration(string fraction)
{
    // TODO

    // In order to know how many one eighths a fraction x/y consists of
    // we need to do some simple math from elementary school
    // So, ((x/y) / (1/8)) = (8 * x) / (1 * y) = 8x/y

    // In order to get the actual integers (the numerator and the denominator) from the string fraction
    // we need to use the atoi function from stdlib.h
    // But before that we need to "tokenize" the string by a delimeter ("/" in our case)
    // since we need to pass a string to atoi
    // We use the strtok function from string.h to do that
    return 8 * atoi(strtok(fraction, "/")) / atoi(strtok(NULL, "/"));
}

// Calculates frequency (in Hz) of a note
int frequency(string note)
{
    // TODO

    char letter; // the "letter" part of a note (a character form A through G)
    char accidental = '/'; // the accidental, the default value '/' means there is no accidental
    int oct; // the octave (integer from 0 to 8)
    int dist; // used to count the distance from A to other letters within an octave
    double freq; // frequence in Hz

    // First, we need to parse the note
    // it can be given as two characters, like A4 (without accidentals)
    // and also it can be given as three chararcters, like A#4 (with an accidental)
    if (strlen(note) == 2)
    {
        letter = note[0]; // the "note", a letter A through G
        // We don't have any delimeters here to tokenize the string into note and octave
        // to pass it into atoi to get the actual int value for an octave
        // So here we use a trick for converting a "char int" (e.g. '4') to an actual integer value
        // According to ASCII table subtracting '0' will give us the value we need
        // The same happens below for three character notes
        oct = note[1] - '0'; // the octave, an integer from 0 to 8 (inclusive both)
    }
    else
    {
        letter = note[0]; // the "note", a letter A through G
        accidental = note[1]; // the accidental, either # or b
        oct = note[2] - '0'; // the octave, an integer from 0 to 8 (inclusive both)
    }

    // now let's determine the distance from A within an octave
    switch (letter)
    {
        case 'B':
        case 'G':
            dist = 2;
            break;
        case 'F':
            dist = 4;
            break;
        case 'E':
            dist = 5;
            break;
        case 'D':
            dist = 7;
            break;
        case 'C':
            dist = 9;
            break;
        default:
            dist = 0;
            break;
    }

    // The way we calculate the frequency is as follows:
    // 1. If the letter is not B, meaning any letter left to A within an octave, we devide the frequency of A in this octave
    // (FORMULA A below) by 2 in power of (dist / 12)
    // 2. If the letter is B, we do the same, but instead of deviding, we now multiply by (dist / 12)
    // FORMULA A: A4_DURATION * pow(2, oct - 4) - is the formula to find the frequency of A note in any octave
    freq = (letter != 'B') ? A4_DURATION * pow(2, oct - 4) / pow(2, dist / 12.0)
           : A4_DURATION * pow(2, oct - 4) * pow(2, dist / 12.0);

    // 3. If there is an accidental, we multiply/devide the frequency of the note by 2 ^ (1 / 12),
    // depending on which accidental it is, # or b
    if (accidental != '/')
    {
        freq = (accidental == '#') ? freq * pow(2, 1.0 / 12.0) : freq / pow(2, 1.0 / 12.0);
    }

    // 4. Round to the nearest integer and return, that's it!
    return round(freq);
}

// Determines whether a string represents a rest
bool is_rest(string s)
{
    // TODO

    // According to the reference the CS50 get_string function returns "" (i.e. an empty string)
    // when user types in just a line ending (like \n - that is just hitting Enter key)
    // And those blank lines in our implementation mean the rest
    if (strlen(s) == 0)
    {
        return true;
    }
    return false;
}
