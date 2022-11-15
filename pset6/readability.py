# DONE

def main():
    # Prompt user for input
    text = input("Text: ")

    #count number of letters, words and sentences
    letters = count_letters(text)
    words = count_words(text)
    sentences = count_sen(text)

    # The Coleman-Liau index, where:
    # L is the average number of letters per 100 words in the text.
    # S is the average number of sentences per 100 words in the text.

    L = ((letters * 100) / words)
    S = ((sentences * 100) / words)
    index = 0.0588 * L - 0.296 * S - 15.8

    # Print grade
    if (grade := round(index)) < 1:
        print("Before Grade 1\n")
        return

    if (grade) < 16:
        print(f"Grade {grade}")
        return

    print("Grade 16+\n")


# Determine number of letters
def count_letters(text):
    letters = 0
    for char in text:
        if char.isalpha():
            letters += 1

    return letters


# Determine number of words
def count_words(text):
    words = 1
    for char in text:
        if char == ' ':
            words += 1

    return words


# Determine number of sentences
def count_sen(text):
    sentences = 0
    for char in text:
        if char == '.' or char == '?' or char == '!':
            sentences += 1

    return sentences

if __name__ == '__main__':
    main()