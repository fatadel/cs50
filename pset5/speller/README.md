# Questions

## What is pneumonoultramicroscopicsilicovolcanoconiosis?

Maximum length for a word in a text.

## According to its man page, what does `getrusage` do?

Returns resource usage measures.

## Per that same man page, how many members are in a variable of type `struct rusage`?
It has 16 members:

1. ru_utime;
2. ru_stime;
3. ru_maxrss;
4. ru_ixrss;
5. ru_idrss;
6. ru_isrss;
7. ru_minflt;
8. ru_majflt;
9. ru_nswap;
10. ru_inblock;
11. ru_oublock;
12. ru_msgsnd;
13. ru_msgrcv;
14. ru_nsignals;
15. ru_nvcsw;
16. ru_nivcsw.

## Why do you think we pass `before` and `after` by reference (instead of by value) to `calculate`, even though we're not changing their contents?

I think we do so, because we want to maximize the 'purity' of the calculations and that's why we don't want to spend time on making copies of the variables,
which happens when they are passed by value.

## Explain as precisely as possible, in a paragraph or more, how `main` goes about reading words from a file. In other words, convince us that you indeed understand how that function's `for` loop works.

It iterates over each character in the text.
For every legitimate character (i.e. english alphabet letters + apostrophe) it composes a word (checking if a word's length is less than or equal to maximum length of a word).
Than it passes that word to check spelling function, calculating all benchmarks, and printing the word, if it's misspelled.

## Why do you think we used `fgetc` to read each word's characters one at a time rather than use `fscanf` with a format string like `"%s"` to read whole words at a time? Put another way, what problems might arise by relying on `fscanf` alone?

The way of using `fgetc` is more appropriate and faster in case of checking for legitimate characters. For example, for `fscanf` with a format string like `"%s"` "hellocs50" would be read into memory,
while it's just left by with `fgetc`.

## Why do you think we declared the parameters for `check` and `load` as `const` (which means "constant")?

I think it's done in order to defend from changing arguments inside the functions. Since the arguments are pointers, dereferencing them inside the functions will lead to changing contets of those pointers everywhere.
