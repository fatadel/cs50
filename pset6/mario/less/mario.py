# Prompt user for the height
# Prompt again if it doesn't match the requirements
while True:
    n = input("Height: ")
    try:
        n = int(n)
        if n >= 0 and n <= 23:
            break
    except ValueError:
        continue
# For every line print (n - i - 1) times space
# And (i + 2) times the hash sign
for i in range(n):
    print(" " * (n - i - 1), end="")
    print("#" * (i + 2), end="")
    print()