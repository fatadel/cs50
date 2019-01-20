import sys

ALPHABET_SIZE = 26

# Check proper usage
if len(sys.argv) != 2:
    print('Usage: python caesar.py k, where k is a non-negative number for "key"')
    sys.exit(1)

# Get the key
try:
    key = int(sys.argv[1])
except ValueError:
    key = 0

# Get the plain text from user
text = input("plaintext:  ")
print("ciphertext: ", end="")

# Calculate the cipher text and print it out char by char
for c in text:
    if c.isalpha():
        if c.islower():
            c = chr((ord(c) - ord('a') + key) % ALPHABET_SIZE + ord('a'))
        else:
            c = chr((ord(c) - ord('A') + key) % ALPHABET_SIZE + ord('A'))
    print(c, end="")

# Print a newline
print()