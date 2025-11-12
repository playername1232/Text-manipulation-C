#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// region macro definitions

#define false   0
#define true    1

#define ASCII_MASK 32

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

void RemoveRepeatingCharacters(char* str, const unsigned int* start, struct WordChanges* wordChanges)
{
    unsigned int i = *start;
    unsigned int j = i + 1;

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
        }
        else
        {
            i = j;
            j++;
        }
    }
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
    const int len = (int)strlen(str);

    for (int i = len - 1; i >= 0 && str[i] == ' '; i--)
    {
        str[i] = str[i + 1];
        wordChanges->spacesBefore++;
    }
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

void PerformWordCheck(char* ptr, const unsigned int* start, struct WordChanges* wordChanges)
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
        RemoveRepeatingCharacters(ptr, start, wordChanges);
    }
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
            PerformWordCheck(ptr, &startIdx, wordChanges);
            startIdx = i + 1;
        }

        if (ptr[i] == '\0')
            break;
    }

    printf("\n");
    PrintWord(ptr);
    printf("\n");
    PrintChanges(wordChanges);
}

// endregion

int main(void)
{
    char numBuffer[32];

    fgets(numBuffer, sizeof(numBuffer), stdin);

    unsigned int numberOfWords = (unsigned int)strtoul(numBuffer, NULL, 10);

    char (*words)[50] = calloc(numberOfWords, sizeof(*words));
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

    for (unsigned int i = 0; i < numberOfWords; i++)
    {
        PerformStringCheck(words[i], &changes[i]);
    }

    free(words);
    free(changes);
    return 0;
}
