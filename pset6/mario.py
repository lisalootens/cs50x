#DONE

#Prompt user for input: positive integer
height = 0

while height > 8 or height < 1:
    try:
        height = int(input("Height: "))
    except ValueError:
        pass

#Print pyramid hashes
for i in range(height):
    print(((" " * (height - i - 1)) + ("#" * (i + 1))) + "  " + ("#" * (i + 1)))
