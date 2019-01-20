#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

// Block size, for FAT filesystems, used in our case, it's 512 bytes
#define BLOCK_SIZE 512
// According to the task specification, the name of a file is "xyz.jpg",
// where x, y and z are integers from 0 to 9
#define FILENAME_SIZE 7

// Let's make an alias for 8 bits unsighned integer (1 byte)
// We could use type char in our buffer (it takes exactly one byte)
// But for for sake of esthetic purposes let's call it BYTE ;)
typedef uint8_t BYTE;

// Let's declare a function that composes a filename according to number of that file
char *composeFilename(int *count);

int main(int argc, char *argv[])
{
    // check appropriate usage
    if (argc != 2)
    {
        fprintf(stderr, "Usage: ./recover image, where image is the name of a forensic image from which to recover JPEGs\n");
        return 1;
    }

    // read the forensic image
    FILE *file = fopen(argv[1], "r");
    // check for errors on reading
    if (file == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", argv[1]);
        return 2;
    }

    // buffer of bytes to read/write, so that we read/write not one byte by byte but one block of 512 bytes at a time,
    // since the FAT filesystem allows us to do this
    BYTE buffer[BLOCK_SIZE];
    // boolea value showing whether we are already writing a JPEG file
    bool JPEGWriting = false;
    // counter to count files
    int count = 0;
    // file pointer used to create new files when we find an image in the forensic one
    FILE *img = NULL;
    // string for filename
    char *filename = NULL;

    // while we read exactly ONE block of BLOCK_SIZE bytes
    while (fread(buffer, BLOCK_SIZE, 1, file) == 1)
    {
        // if it's JPEG 'signature'
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            // if we have already started writing a JPEG before
            // we need to close it before starting a new JPEG file
            if (JPEGWriting)
            {
                // close previous JPEG
                fclose(img);
            }

            // open a file for new JPEG, composing the filename
            filename = composeFilename(&count);
            img = fopen(filename, "w");
            // check for errors on opening
            if (img == NULL)
            {
                fclose(file);
                fprintf(stderr, "Could not create %s.\n", filename);
                return 3;
            }
            // write to the file
            fwrite(buffer, BLOCK_SIZE, 1, img);
            // make the flag of JPEGWriting true, since we started writing a JPEG
            JPEGWriting = true;
            // increase the counter for JPEG files
            count++;

            // freed the memory allocated in composeFilename function
            free(filename);
        }
        // if it's not JPEG 'signature' check if we are already writing a JPEG
        // and if so, add new blocks to that file
        else if (JPEGWriting)
        {
            // keep writing in opened JPEG file
            fwrite(buffer, BLOCK_SIZE, 1, img);
        }
    }

    // close the forensic image
    fclose(file);
    // close the last JPEG file
    fclose(img);
    // end the program with success code
    return 0;
}

// the function that takes by reference file counter and composes the filename for it
char *composeFilename(int *count)
{
    // allocate memory enough for 'xyz.jpg' (7 characters (each one byte) + 1 byte for '\0' to terminate the string)
    char *filename = malloc((FILENAME_SIZE + 1) * sizeof(char));

    // case of two leading zeros
    if (*count < 10)
    {
        sprintf(filename, "00%d.jpg", *count);
        return filename;
    }
    // case of one leading zero
    else if (*count < 100)
    {
        sprintf(filename, "0%d.jpg", *count);
        return filename;
    }
    // case of no leading zeros
    else if (*count < 1000)
    {
        sprintf(filename, "%d.jpg", *count);
        return filename;
    }
    // let's support the case when we are overflowing,
    // although we know there are no more than 50 images in this task :)
    else
    {
        *count = 0;
        sprintf(filename, "00%d.jpg", *count);
        return filename;
    }
}