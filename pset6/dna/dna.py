import csv
import sys


def main():
    # Ensure correct usage
    if len(sys.argv) != 3:
        sys.exit("Usage: python dna.py data.csv sequence.txt")

    # Open and read CSV file with all possible sequences to check into memory
    people = []
    with open(sys.argv[1], 'r', encoding='utf-8') as f:
        reader = csv.DictReader(f)

        for index, subject in enumerate(reader):
            if index == 0:
                sequences = [sequence for sequence in subject.keys() if sequence != 'name']

            people.append(subject)

    # Get the DNA from a file
    with open(sys.argv[2], 'r', encoding='utf-8') as f:
        dna = f.readline().strip()

    sequence_totals = {k:0 for k in sequences}

    for seq in sequence_totals.keys():
        sequence_length = len(seq)
        max_ = 0

        for c in range(len(dna)):
            current = 0

            if dna[c:c+sequence_length] == seq:
                current += 1
                while dna[c-sequence_length:c] == dna[c:c+sequence_length]:
                    current += 1
                    c += sequence_length

            if current > max_:
                max_ = current

        sequence_totals[seq] = max_

    for possible_match in people:

        # The amount of exact matches of sequence with a certain
        matches = 0

        for key, value in sequence_totals.items():
            if int(possible_match[key]) == value:
                matches += 1

        if matches == len(sequence_totals):
            print(possible_match['name'])
            sys.exit()

    print('No match')
    sys.exit()


if __name__ == "__main__":
    main()