#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// region macro definitions

#define false   0
#define true    1

#define ASCII_MASK 32

#define MAX_CHARS 50
#define STR_BUFFER_SIZE (MAX_CHARS + 2)

// endregion

struct WordChanges
{
    int lowerCaseBefore;
    int lowerCaseAfter;

    int upperCaseBefore;
    int upperCaseAfter;

    int spacesBefore;
    int spacesAfter;
};

// region DEBUG Helper functions

struct WordChanges wordChangesCtorStack()
{
    return (struct WordChanges){0, 0, 0, 0, 0, 0};
}

struct WordChanges* wordChangesCtorHeap()
{
    return calloc(1, sizeof(struct WordChanges));
}

// endregion

// region ascii conversion

// Returns 0 -> Character isn't lower case ('a' - 'z')
// Returns 1 -> Character is lower case ('a' - 'z')
int asciiIsLowerCase(char _c)
{
    return _c >= 'a' && _c <= 'z';
}

// Returns 0 -> Character isn't upper case ('A' - 'Z')
// Returns 1 -> Character is upper case ('A' - 'Z')
int asciiIsUpperCase(char _c)
{
    return _c >= 'A' && _c <= 'Z';
}

// Expects upper case ASCII character 'A' - 'Z' - Checkable via asciiIsUpperCase(char) function
// Returns lower case ASCII character e.g. 'A' -> 'a', 'B' -> 'b'
char asciiToLowerCase(char _c)
{
    return _c + ASCII_MASK;
}

// Expects lower case ASCII character 'a' - 'z' - Checkable via asciiIsLowerCase(char) function
// Returns upper case ASCII character e.g. 'a' -> 'A', 'b' -> 'B'
char asciiToUpperCase(char _c)
{
    return _c - ASCII_MASK;
}
// endregion

// region Text manipulation
int ContainsIdenticalCharactersInRow(const char* str, const unsigned int* start)
{
    for (unsigned int i = *start; str[i + 1] != '\0' && str[i + 1] != ' '; i++)
    {
        if (str[i] == str[i + 1])
        {
            return true;
        }
    }

    return false;
}

int ContainsOnlyLowerCase(const char* str, const unsigned int* start)
{
    for (unsigned int i = *start; str[i] != '\0' && str[i] != ' '; i++)
    {
        if (asciiIsUpperCase(str[i]))
        {
            return false;
        }
    }

    return true;
}

void ToUpperWord(char* str, const unsigned int* start, struct WordChanges* wordChanges)
{
    for (unsigned int i = *start; str[i] != '\0' && str[i] != ' '; i++)
    {
        if (asciiIsLowerCase(str[i]))
        {
            wordChanges->lowerCaseBefore++;
            wordChanges->upperCaseAfter++;

            str[i] = asciiToUpperCase(str[i]);
        }
        else if (asciiIsUpperCase(str[i]))
        {
            wordChanges->upperCaseBefore++;
            wordChanges->upperCaseAfter++;
        }
    }
}

unsigned int RemoveRepeatingCharacters(char* str, const unsigned int* start, struct WordChanges* wordChanges)
{
    unsigned int i = *start;
    unsigned int j = i + 1;
    unsigned int removedCounter = 0;

    while (str[j] != '\0' && str[j] != ' ')
    {
        if (str[j] == str[i])
        {
            for (unsigned int k = j; str[k] != '\0'; k++)
            {
                str[k] = str[k + 1];
            }

            if (asciiIsLowerCase(str[i]))
            {
                wordChanges->lowerCaseAfter--;
            }
            else if (asciiIsUpperCase(str[i]))
            {
                wordChanges->upperCaseAfter--;
            }

            removedCounter++;
        }
        else
        {
            i = j;
            j++;
        }
    }

    return removedCounter;
}

void TrimSpacesStart(char* str, struct WordChanges* wordChanges)
{
    int switchIndex = 0;

    for (int i = 0; str[i] != '\0' && str[i] == ' '; i++)
    {
        switchIndex++;
    }

    for (int i = 0;  str[i] != '\0'; i++)
    {
        str[i] = str[i + switchIndex];
    }

    wordChanges->spacesBefore += switchIndex;
}

void TrimSpacesEnd(char* str, struct WordChanges* wordChanges)
{
    int idx = (int)strlen(str) - 1;

    // testing + kelvin results have failed due to whitespace characters (0xA, 0x10, 0x32, new line, tab etc.)
    while (idx >= 0 && isspace((unsigned char)str[idx]))
    {
        if (str[idx] == ' ')
        {
            wordChanges->spacesBefore++;
        }

        idx--;
    }

    str[++idx] = '\0';
}

void RemoveDuplicitSpaces(char* str, struct WordChanges* wordChanges)
{
    int j = 0;
    int space = false;

    for (int i = 0; str[i] != '\0'; i++)
    {
        if (str[i] == ' ')
        {
            wordChanges->spacesBefore++;

            if (!space)
            {
                str[j++] = ' ';
                space = true;
                wordChanges->spacesAfter++;
            }
        }
        else
        {
            str[j++] = str[i];

            space = false;
        }
    }

    str[j] = '\0';
}



void NormalizeWordFirstLetterCapital(char* str, const unsigned int* start, struct WordChanges* wordChanges)
{
    if (asciiIsLowerCase(str[*start]))
    {
        str[*start] = asciiToUpperCase(str[*start]);
        wordChanges->lowerCaseBefore++;
        wordChanges->upperCaseAfter++;
    }
    else if (asciiIsUpperCase(str[*start]))
    {
        wordChanges->upperCaseBefore++;
        wordChanges->upperCaseAfter++;
    }

    for (unsigned int i = *start + 1; str[i] != ' ' && str[i] != '\0'; i++)
    {
        if (asciiIsUpperCase(str[i]))
        {
            str[i] = asciiToLowerCase(str[i]);
            wordChanges->upperCaseBefore++;
            wordChanges->lowerCaseAfter++;
        }
        else if (asciiIsLowerCase(str[i]))
        {
            wordChanges->lowerCaseBefore++;
            wordChanges->lowerCaseAfter++;
        }
    }
}

unsigned int PerformWordCheck(char* ptr, const unsigned int* start, struct WordChanges* wordChanges)
{
    if (ContainsOnlyLowerCase(ptr, start))
    {
        ToUpperWord(ptr, start, wordChanges);
    }
    else
    {
        NormalizeWordFirstLetterCapital(ptr, start, wordChanges);
    }

    if (ContainsIdenticalCharactersInRow(ptr, start))
    {
        return RemoveRepeatingCharacters(ptr, start, wordChanges);
    }

    return 0;
}

void PrintWord(const char* str)
{
    for (int i = 0; str[i] != '\0'; i++)
    {
        printf("%c", str[i]);
    }
}

void PrintChanges(struct WordChanges* wordChanges)
{
    printf("lowercase: %d -> %d\n", wordChanges->lowerCaseBefore, wordChanges->lowerCaseAfter);
    printf("uppercase: %d -> %d\n", wordChanges->upperCaseBefore, wordChanges->upperCaseAfter);
    printf("spaces: %d -> %d\n", wordChanges->spacesBefore, wordChanges->spacesAfter);
}

void PerformStringCheck(char* ptr, struct WordChanges* wordChanges)
{
    TrimSpacesStart(ptr, wordChanges);
    TrimSpacesEnd(ptr, wordChanges);
    RemoveDuplicitSpaces(ptr, wordChanges);

    unsigned int startIdx = 0;

    for (unsigned int i = 0; ; i++)
    {
        if (ptr[i] == ' ' || ptr[i] == '\0')
        {
            const unsigned int decrement = PerformWordCheck(ptr, &startIdx, wordChanges);
            startIdx = i + 1 - decrement;
        }

        if (ptr[i] == '\0')
            break;
    }

    PrintWord(ptr);
    printf("\n");
    PrintChanges(wordChanges);
    printf("\n");
}

// endregion

int main(void)
{
    char numBuffer[sizeof(unsigned int) * 8];

    fgets(numBuffer, sizeof(numBuffer), stdin);

    unsigned int numberOfWords = (unsigned int)strtoul(numBuffer, NULL, 10);

    char (*words)[STR_BUFFER_SIZE] = calloc(numberOfWords, sizeof(*words));
    if (words == NULL)
    {
        fprintf(stderr, "Failed to allocate memory for words");
        return EXIT_FAILURE;
    }

    for (unsigned int i = 0; i < numberOfWords; i++)
    {
        if (!fgets(words[i], STR_BUFFER_SIZE, stdin))
        {
            fprintf(stderr, "Unexpected EOF on line %u\n", i);
            free(words);
            return EXIT_FAILURE;
        }

        const size_t len = strlen(words[i]);
        if (len > 0 && words[i][len - 1] == '\n')
        {
            words[i][len - 1] = '\0';
        }
    }

    struct WordChanges* changes = calloc(numberOfWords, sizeof(struct WordChanges));
    if (changes == NULL)
    {
        free(words);
        fprintf(stderr, "Failed to allocate memory for changes");
        return EXIT_FAILURE;
    }

    for (unsigned int i = 0; i < numberOfWords; i++)
    {
        PerformStringCheck(words[i], &changes[i]);
    }

    free(words);
    free(changes);
    return 0;
}
