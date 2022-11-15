-- Keep a log of any SQL queries you execute as you solve the mystery
-- theft took place on July 28, 2021 and it took place on Humphrey Street

-- search through crime scene reports for date and location
SELECT description
FROM crime_scene_reports
WHERE month = 7 AND day = 28 AND year = 2021 AND street = 'Humphrey Street';
/* Description output:
    Time:               10:15am
    Location:           Humphrey Street bakery
    No. of witnesses    3
*/

-- Let's see what the 3 witnesses had to say by looking through the interviews mentioning the bakery
SELECT name, transcript
FROM interviews
WHERE transcript LIKE '%bakery%' AND day = 28 AND month = 7 AND year = 2021;

-- Witness Eugene: Thief was withdrawing money earlier that morning on Leggett Street
-- Get the account information from the transactions
WITH people_who_withdrew_money AS (SELECT * FROM people
INNER JOIN bank_accounts ON people.id = bank_accounts.person_id
INNER JOIN atm_transactions ON bank_accounts.account_number = atm_transactions.account_number
WHERE atm_transactions.atm_location = 'Leggett Street' AND atm_transactions.day = 28 AND atm_transactions.month = 7 AND atm_transactions.year = 2021 AND atm_transactions.transaction_type = "withdraw"
),

-- Witness Raymond: As the thief was leaving the bakery, they called someone who talked to them for less than a minute
-- Find out whom of the above made a phone call and who did he/she call for their flight plans?
calls AS (SELECT * FROM phone_calls WHERE caller IN (
    SELECT phone_number FROM people_who_withdrew_money
    ) AND duration < 60 AND day = 28 AND month = 7 AND year = 2021
),

-- Witness Raymond: They were planning to take the earliest flight out of Fiftyville tomorrow
flight AS (SELECT * FROM flights
    WHERE day = 29 AND month = 7 AND year = 2021
    AND origin_airport_id = (
        SELECT id FROM airports WHERE city = "Fiftyville"
    )
    ORDER BY hour ASC, minute ASC
    LIMIT 1
),

-- Determinde destination by flight id and destination id
destination AS (SELECT city FROM airports
    WHERE id = (
    SELECT destination_airport_id FROM flight
    )
),

-- Find name of thief
thief AS (SELECT name FROM people
    WHERE phone_number = (
    SELECT caller FROM calls
    )
),

-- Find name of accomplice
accomplice AS (SELECT name FROM people
    WHERE phone_number = (
    SELECT receiver FROM calls
    )
)

SELECT name AS thief FROM thief
UNION
SELECT name AS accomplice FROM accomplice
UNION
SELECT city AS destination FROM destination;
