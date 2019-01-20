// In our case there is no other way of cracking the passwords other than bruteforcing them
// So we need to generate plain text passwords with length up to 5 characters (inclusive)
// Taking into account all possible characters - only alphabetical ones but both upper and lower cases
// Generate hashes for them and then compare the two hashes - the one we generated with the one user provided
// Once they are equal - we successfully cracked the password
// Let's start cracking! ;)

#define _XOPEN_SOURCE // needed for use of DES-based crypt function

#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h> // needed for use of DES-based crypt function

int main(int argc, string argv[])
{
    // finish program with error if it was started in an unappropriate way
    if (argc != 2)
    {
        printf("Usage: ./crack HASH, where HASH is a result of C\'s DES-based crypt function\n");
        return 1;
    }

    string hash = argv[1]; // get the hash to crack

    // get the salt; in our case salt is the first two characters of the hash
    char salt[3];
    salt[0] = hash[0];
    salt[1] = hash[1];
    salt[2] = '\0';

    char p[6]; // an array of characters for generating plain text

    // firstly we generate only one character trying all variations of one-char passwords
    // so we "terminate" our array to use only its first byte (p[0])
    p[1] = '\0';

    // loop for generating plain-text
    // trying one-char password variations
    for (int i = 'A'; i <= 'z'; i++)
    {
        // skip characters (see ASCII table) between capital letters and lowercase ones
        // that are not alphabetical (according to specification of the task)
        if (i > 'Z' && i < 'a')
        {
            continue;
        }

        p[0] = i;

        if (strcmp(crypt(p, salt), hash) == 0) // compare the two hashes
        {
            printf("%s\n", p); // print the password and finish the program if we found the password
            return 0;
        }
    }

    // now let's terminate the array for two bytes - p[0] and p[1]
    // and try two-char passwords
    p[2] = '\0';

    for (int i = 'A'; i <= 'z'; i++) // outer loop for first byte
    {
        if (i > 'Z' && i < 'a')
        {
            continue;
        }

        p[0] = i;

        for (int j = 'A'; j <= 'z'; j++) // inner loop for second byte
        {
            if (j > 'Z' && j < 'a')
            {
                continue;
            }

            p[1] = j;

            if (strcmp(crypt(p, salt), hash) == 0)
            {
                printf("%s\n", p);
                return 0;
            }
        }
    }

    // now let's terminate the array for three bytes - p[0], p[1] and p[2]
    // and try three-char passwords
    p[3] = '\0';

    for (int i = 'A'; i <= 'z'; i++) // outer loop for first byte
    {
        if (i > 'Z' && i < 'a')
        {
            continue;
        }

        p[0] = i;

        for (int j = 'A'; j <= 'z'; j++) // first inner loop for second byte
        {
            if (j > 'Z' && j < 'a')
            {
                continue;
            }

            p[1] = j;

            for (int x = 'A'; x <= 'z'; x++) // second inner loop for third byte
            {
                if (x > 'Z' && x < 'a')
                {
                    continue;
                }

                p[2] = x;

                if (strcmp(crypt(p, salt), hash) == 0)
                {
                    printf("%s\n", p);
                    return 0;
                }
            }
        }
    }

    // the same for four bytes and four-char passwords
    p[4] = '\0';

    for (int i = 'A'; i <= 'z'; i++) // outer loop for first byte
    {
        if (i > 'Z' && i < 'a')
        {
            continue;
        }

        p[0] = i;

        for (int j = 'A'; j <= 'z'; j++) // first inner loop for second byte
        {
            if (j > 'Z' && j < 'a')
            {
                continue;
            }

            p[1] = j;

            for (int x = 'A'; x <= 'z'; x++) // second inner loop for third byte
            {
                if (x > 'Z' && x < 'a')
                {
                    continue;
                }

                p[2] = x;

                for (int y = 'A'; y <= 'z'; y++) // third inner loop for fourth byte
                {
                    if (y > 'Z' && y < 'a')
                    {
                        continue;
                    }

                    p[3] = y;

                    if (strcmp(crypt(p, salt), hash) == 0)
                    {
                        printf("%s\n", p);
                        return 0;
                    }
                }
            }
        }
    }

    // and finally five bytes and five-char passwords
    // the hardest and most time-consuming ones
    p[5] = '\0';

    for (int i = 'A'; i <= 'z'; i++) // outer loop for first byte
    {
        if (i > 'Z' && i < 'a')
        {
            continue;
        }

        p[0] = i;

        for (int j = 'A'; j <= 'z'; j++) // first inner loop for second byte
        {
            if (j > 'Z' && j < 'a')
            {
                continue;
            }

            p[1] = j;

            for (int x = 'A'; x <= 'z'; x++) // second inner loop for third byte
            {
                if (x > 'Z' && x < 'a')
                {
                    continue;
                }

                p[2] = x;

                for (int y = 'A'; y <= 'z'; y++) // third inner loop for fourth byte
                {
                    if (y > 'Z' && y < 'a')
                    {
                        continue;
                    }

                    p[3] = y;

                    for (int z = 'A'; z <= 'z'; z++) // forth inner loop for fifth byte
                    {
                        if (z > 'Z' && z < 'a')
                        {
                            continue;
                        }

                        p[4] = z;

                        if (strcmp(crypt(p, salt), hash) == 0)
                        {
                            printf("%s\n", p);
                            return 0;
                        }
                    }
                }
            }
        }
    }

    // if we reached here means we could not crack the hash
    // let's inform the user and finish the program
    printf("Could not find a password matching the hash.\nPlease try again or provide another hash.\n");
    return 0;
}