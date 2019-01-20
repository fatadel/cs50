#include <cs50.h>
#include <stdio.h>

int main(void)
{
    long long n; // the credit card number
    int count = 1; // "counter", shows current digit's position
    int sum = 0; // final "checksum"
    int penultimateDigit = -1; // After the last step of the while loop last digit will be stored in n;
    // The digit before it will be stored here in penultimateDigit
    // We knowingly initialize it with invalid number since its unknown and can be determined after the first step
    // and only if there is more than 1 digit in the number

    n = get_long_long("Number: ");

    while (n / 10 != 0) // While there are left digits in the number
    {
        if (count % 2 == 0) // logic for digits on even positions
        {
            sum += ((n % 10) * 2 % 10 + (n % 10) * 2 / 10); // multiply by 2, get its digits' sum, add to the checksum

        }
        else // logic for digits on odd positions
        {
            sum += n % 10; // just add it to the checksum
        }
        penultimateDigit = n % 10; // store the digit as penultimateDigit
        count++; // increase the current digit's position
        n /= 10; // get rid off the proccessed digit
    }

    // Add the last number to the checksum
    if (count % 2 == 0) // if it is on even position
    {
        sum += (n * 2 % 10 + n * 2 / 10);
    }
    else // if it is on odd position
    {
        sum += n;
    }

    // if the checksum is invalid, print INVALID and exit the program
    if (sum % 10 != 0)
    {
        printf("INVALID\n");
        return 0;
    }

    // Check for AmEx; AmEx cards start with 34 or 37 and has 15 digits in total
    if (((n == 3 && penultimateDigit == 4) || (n == 3 && penultimateDigit == 7)) && count == 15)
    {
        printf("AMEX\n");
    }
    // Check for MasterCard; MasterCard cards start with 51, 52, 53, 54 or 55 and has 16 digits in total
    else if (((n == 5 && penultimateDigit == 1)
              || (n == 5 && penultimateDigit == 2)
              || (n == 5 && penultimateDigit == 3)
              || (n == 5 && penultimateDigit == 4)
              || (n == 5 && penultimateDigit == 5)) && count == 16)
    {
        printf("MASTERCARD\n");
    }
    // Check for Visa; Visa cards start with 4 and has 13 or 16 digits in total
    else if (n == 4 && (count == 13 || count == 16))
    {
        printf("VISA\n");
    }
    // Otherwise the card number is invalid
    else
    {
        printf("INVALID\n");
    }
}