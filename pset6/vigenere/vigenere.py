import sys

ALPHABET_SIZE = 26


def getKey(keyword, count):
    """Calculating positions to shift in plain text for a particular letter according to"""
    """the KEYWORD and amount of alphabetical letters passed in the plain text (COUNT)"""
    count %= len(keyword)
    return ord(keyword[count]) - ord('a') if keyword[count].islower() else ord(keyword[count]) - ord('A')


# Check proper usage
if len(sys.argv) != 2 or not sys.argv[1].isalpha():
    print("Usage: python vigenere.py KEYWORD, where KEYWORD is a non-zero sequence of alphabetical characters")
    sys.exit(1)

# Get the key
keyword = sys.argv[1]

# Get the plain text from user
text = input("plaintext:  ")
print("ciphertext: ", end="")

count = 0

# Calculate the cipher text and print it out char by char
for c in text:
    if c.isalpha():
        if c.islower():
            c = chr((ord(c) - ord('a') + getKey(keyword, count)) % ALPHABET_SIZE + ord('a'))
        else:
            c = chr((ord(c) - ord('A') + getKey(keyword, count)) % ALPHABET_SIZE + ord('A'))
        count += 1
    print(c, end="")

# Print a newline
print()

