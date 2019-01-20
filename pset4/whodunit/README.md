# Questions

## What's `stdint.h`?

The header file defining data types, including those BMP-related ones.

## What's the point of using `uint8_t`, `uint32_t`, `int32_t`, and `uint16_t` in a program?

All the structs defined in `bmp.h` make use of these data types.

## How many bytes is a `BYTE`, a `DWORD`, a `LONG`, and a `WORD`, respectively?

One, four, four and two.

## What (in ASCII, decimal, or hexadecimal) must the first two bytes of any BMP file be? Leading bytes used to identify file formats (with high probability) are generally called "magic numbers."

`BM` in ASCII (`0x42` and `0x4D` in hexadecimal).

## What's the difference between `bfSize` and `biSize`?

`biSize` is the size of `BITMAPINFOHEADER` (must be 40 bytes), while `bfSize` is the total size of a file including headers.

## What does it mean if `biHeight` is negative?

If `biHeight` is negative, it indicats a top-down bitmap file. Otherwise, it's bottom-up.

## What field in `BITMAPINFOHEADER` specifies the BMP's color depth (i.e., bits per pixel)?

`biBitCount`

## Why might `fopen` return `NULL` in lines 24 and 32 of `copy.c`?

When reading a file it might not exit or can be broken. In this case we get `NULL`.
When writing a file, it's possible we don't have appropriate permissions to create files in that particular directory. This will
lead to `NULL` as well.

## Why is the third argument to `fread` always `1` in our code?

The third argument in `fread` is a number of elements to read. All BMP files contain **one** instance of each
struct (`BITMAPFILEHEADER` and `BITMAPINFOHEADER`). That's why we use `1` as the third argument of `fread` when reading
these structs.
The other `fread` in our code reads **one** `RGBTRIPLE` at each loop iteration. That's why we use `1` here as well.

## What value does line 65 of `copy.c` assign to `padding` if `bi.biWidth` is `3`?

`int padding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4 = (4 - (3 * 3) % 4) % 4 = (4 - 9 % 4) % 4 = (4 - 1) % 4 = 3 % 4 = 3`

## What does `fseek` do?

It's keeps track where we are in a file, just like our cursor.

## What is `SEEK_CUR`?

`SEEK_CUR` means current position in file.

## Whodunit?

It was Professor Plum with the candlestick in the library.
