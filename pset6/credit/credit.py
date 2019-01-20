import sys

# "counter", shows current digit's position
count = 1
# final "checksum"
checksum = 0
# After the last step of the while loop last digit will be stored in n;
# The digit before it will be stored here in penultimateDigit
# We knowingly initialize it with invalid number since its unknown and can be determined after the first step
# and only if there is more than 1 digit in the number
penultimateDigit = -1

# Prompt user for the credit card number
# Prompt again if it's not an appropriate number
while True:
    n = input("Number: ")
    try:
        n = int(n)
        break
    except ValueError:
        continue

# Calculate the checksum
while n // 10 != 0:
    if count % 2 == 0:
        checksum += ((n % 10) * 2 % 10 + (n % 10) * 2 // 10)
    else:
        checksum += n % 10
    penultimateDigit = n % 10
    count += 1
    n //= 10

# Do the same for the last digit
if count % 2 == 0:
    checksum += (n * 2 % 10 + n * 2 // 10)
else:
    checksum += n

# Check if the checksum is correct
if checksum % 10 != 0:
    print("INVALID")
    sys.exit(0)

# Check for appropriate credit cards
if ((n == 3 and penultimateDigit == 4) or (n == 3 and penultimateDigit == 7)) and count == 15:
    print("AMEX")
elif ((n == 5 and penultimateDigit == 1)
      or (n == 5 and penultimateDigit == 2)
      or (n == 5 and penultimateDigit == 3)
      or (n == 5 and penultimateDigit == 4)
      or (n == 5 and penultimateDigit == 5)) and count == 16:
    print("MASTERCARD")
elif n == 4 and (count == 13 or count == 16):
    print("VISA")
else:
    print("INVALID")