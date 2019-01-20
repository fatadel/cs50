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
# For every row print:
# 1) (n - i - 1) times space
# 2) (i + 1) times the hash sign for first column
# 3) two spaces separating the two columns
# 4) (i + 1) times the hash sign for second column
for i in range(n):
    print(" " * (n - i - 1), end="")
    print("#" * (i + 1), end="")
    print("  ", end="")
    print("#" * (i + 1), end="")
    print()