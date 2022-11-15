#DONE

#ask for owed change
change = 0
while change <= 0:
    try:
        change = float(input("Change owed: "))
    except ValueError:
        pass

# Convert to cents
cents = round(change * 100)

#Loop conditions for quarters 25¢, dimes 10¢, nickels 5¢ and pennies 1¢
coins = 0
quarters = 25
dimes = 10
nickels = 5
pennies = 1

while cents >= quarters:
    cents -= quarters
    coins += 1

while cents >= dimes:
    cents -= dimes
    coins += 1

while cents >= nickels:
    cents -= nickels
    coins += 1
    
while cents >= pennies:
    cents -= pennies
    coins += 1

#print number of coins used
print(coins)




