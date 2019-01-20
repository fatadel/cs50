# Prompt user for the change owed
# Prompt again if it's a negative number
while True:
    n = input("Change owed: ")
    try:
        n = float(n)
        if n >= 0:
            break
    except ValueError:
        continue

# Change owed in cents
left = round(n * 100)

# Take integer division of left for 25
# Take integer division of the rest for 10
# The same for 5 and 1
# Add all
# That's the answer! :)
print(left // 25 + left % 25 // 10 + left % 25 % 10 // 5 + left % 25 % 10 % 5)