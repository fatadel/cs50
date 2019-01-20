import crypt
import string
import sys

# Check for proper usage
if len(sys.argv) != 2:
    print("Usage: python crack.py HASH, where HASH is a result of C's DES-based crypt.crypt function")
    sys.exit(1)

# Get the hash from user
userHash = sys.argv[1]
# Calculate the salt
salt = sys.argv[1][0] + sys.argv[1][1]

# Bruteforce one character passwords
for c in string.ascii_letters:
    if crypt.crypt(c, salt) == userHash:
        print(c)
        sys.exit(0)

# Bruteforce two character passwords
for c1 in string.ascii_letters:
    for c2 in string.ascii_letters:
        if crypt.crypt(c1 + c2, salt) == userHash:
            print(c1 + c2)
            sys.exit(0)

# Bruteforce three character passwords
for c1 in string.ascii_letters:
    for c2 in string.ascii_letters:
        for c3 in string.ascii_letters:
            if crypt.crypt(c1 + c2 + c3, salt) == userHash:
                print(c1 + c2 + c3)
                sys.exit(0)

# Bruteforce four character passwords
for c1 in string.ascii_letters:
    for c2 in string.ascii_letters:
        for c3 in string.ascii_letters:
            for c4 in string.ascii_letters:
                if crypt.crypt(c1 + c2 + c3 + c4, salt) == userHash:
                    print(c1 + c2 + c3 + c4)
                    sys.exit(0)

# Bruteforce five character passwords
for c1 in string.ascii_letters:
    for c2 in string.ascii_letters:
        for c3 in string.ascii_letters:
            for c4 in string.ascii_letters:
                for c5 in string.ascii_letters:
                    if crypt.crypt(c1 + c2 + c3 + c4 + c5, salt) == userHash:
                        print(c1 + c2 + c3 + c4 + c5)
                        sys.exit(0)