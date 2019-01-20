#include <cs50.h>
#include <stdio.h>
#include <math.h>

int main(void)
{
    float n; // number of change owed
    int left; // how much charge is left, in cents, thus integer, we need integer (cents) to avoid floating-point number problems
    int coins = 0; // number of coins needed

    // Prompt user for the number of change owed
    // Prompt once and prompt again if the number is negative
    do
    {
        n = get_float("Change owed: ");
    }
    while (n < 0);

    left = round(n * 100);
    while (left > 0) // do until we owe any money
    {
        if (left - 25 >= 0) // try with the biggest coins aka quarters first
        {
            left -= 25;
        }
        else if (left - 10 >= 0) // if the biggest coins are too much try lowers, that is dimes
        {
            left -= 10;
        }
        else if (left - 5 >= 0) // if dimes are too much, try nickels
        {
            left -= 5;
        }
        else // use the lowest ones, that is pennies, if no one before was suitable
        {
            left -= 1;
        }
        coins++; // increase amount of used coins on every step
    }

    printf("%i\n", coins);
}