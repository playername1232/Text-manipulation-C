#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct WordChanges
{
    unsigned int lowerCaseBefore;
    unsigned int lowerCaseAfter;

    unsigned int upperCaseBefore;
    unsigned int upperCaseAfter;

    unsigned int spacesBefore;
    unsigned int spacesAfter;
};

#define false   0
#define true    1

#define ASCII_MASK 32

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
    if (asciiIsLowerCase(_c))
    {
        return _c - ASCII_MASK;
    }

    return _c;
}
// endregion

// region Text manipulation
int ContainsIdenticalCharactersInRow(const char* str, const unsigned int* length)
{
    if (*length == 1)
    {
        return false;
    }

    for (unsigned int i = 0; i < *length - 1; i++)
    {
        if (str[i] == str[i + 1])
        {
            return true;
        }
    }

    return false;
}

int ToUpperStr(char* str, const unsigned int* length)
{
    int changed = 0;

    for (unsigned int i = 0; i < *length; i++)
    {
        changed += asciiIsLowerCase(str[i]);
        str[i] = asciiToUpperCase(str[i]);
    }

    return changed;
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
    int len = strlen(str);

    for (int i = len - 1; i >= 0 && str[i] == ' '; i--)
    {
        str[i] = str[i + 1];
        wordChanges->spacesBefore++;
    }
}

void RemoveDuplicitSpaces(char* str, struct WordChanges* wordChanges)
{
    int increment = 1;

    for (unsigned int i = 0; str[i + increment] != '\0'; i++)
    {
        if (str[i] == ' ' && str[i + increment] == ' ')
        {
            increment++;
            i--;
        }
        else
        {
            str[i] = str[i + increment];
        }
    }
}

void NormalizeWordFirstLetterCapital(char* str, const unsigned int* start, struct WordChanges* wordChanges)
{
    if (asciiIsLowerCase(str[*start]))
    {
        str[*start] = asciiToUpperCase(str[*start]);
        wordChanges->lowerCaseBefore++;
    }
    else
    {
        wordChanges->upperCaseBefore++;
    }

    wordChanges->upperCaseAfter++;

    for (unsigned int i = *start + 1; str[i] != ' ' && str[i] != '\0'; i++)
    {
        if (asciiIsUpperCase(str[i]))
        {
            str[i] = asciiToLowerCase(str[i]);
            wordChanges->upperCaseBefore++;
        }
        else
        {
            wordChanges->lowerCaseBefore++;
        }

        wordChanges->lowerCaseAfter++;
    }
}

void PerformWordCheck(char* ptr, const unsigned int* start, struct WordChanges* wordChanges)
{
    NormalizeWordFirstLetterCapital(ptr, start, wordChanges);
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
    printf("\nlowercase: %d -> %d\n", wordChanges->lowerCaseBefore, wordChanges->lowerCaseAfter);
    printf("uppercase: %d -> %d\n", wordChanges->upperCaseBefore, wordChanges->upperCaseAfter);
    printf("spaces: %d -> %d\n", wordChanges->spacesBefore, wordChanges->spacesAfter);
}

void PerformStringCheck(char* ptr, struct WordChanges* wordChanges)
{
    TrimSpacesStart(ptr, wordChanges);
    TrimSpacesEnd(ptr, wordChanges);

    unsigned int startIdx = 0;

    for (int i = 0; ptr[i] != '\0'; i++)
    {
        if (ptr[i] == ' ')
        {
            PerformWordCheck(ptr, &startIdx, wordChanges);
            startIdx = i + 1;
            wordChanges->spacesBefore++;
            wordChanges->spacesAfter++;
        }
    }

    NormalizeWordFirstLetterCapital(ptr, &startIdx, wordChanges);

    printf("\n");
    PrintWord(ptr);
    PrintChanges(wordChanges);
}

// endregion

int main(void)
{
    char numBuffer[32];

    fgets(numBuffer, sizeof(numBuffer), stdin);

    unsigned int numberOfWords = (unsigned int)strtoul(numBuffer, NULL, 10);

    char (*words)[50] = malloc(numberOfWords * sizeof(*words));
    if (words == NULL)
    {
        return -1;
    }

    for (unsigned int i = 0; i < numberOfWords; i++)
    {
        fgets(words[i], sizeof(words[i]), stdin);
    }

    struct WordChanges* changes = calloc(numberOfWords, sizeof(struct WordChanges));
    if (changes == NULL)
    {
        free(words);
        return -1;
    }

    for (int i = 0; i < numberOfWords; i++)
    {
        PerformStringCheck(words[i], &changes[i]);
    }

    free(words);
    free(changes);
    return 0;
}
