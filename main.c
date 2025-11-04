#include <stdio.h>

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
int containsIdenticalCharactersInRow(const char* str, const unsigned int* length)
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

int toUpperStr(char* str, const unsigned int* length)
{
    int changed = 0;

    for (unsigned int i = 0; i < *length; i++)
    {
        changed += asciiIsLowerCase(str[i]);
        str[i] = asciiToUpperCase(str[i]);
    }

    return changed;
}

void removeDuplicitSpaces(char* str, const unsigned int* length, struct WordChanges* wordChanges)
{
    int increment = 0;

    while (str[increment++] == ' ');

    if (--increment <= 0)
    {
        increment = 1;
    }

    for (unsigned int i = 0; (i + increment) < *length; i++)
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

void normalizeFirstLetterCapital(char** str, const unsigned int* length, struct WordChanges* wordChanges)
{
    int bFirstLetter = true;

    for (unsigned int i = 0; i < *length; i++)
    {
        if (*str[i] == '_')
        {
            bFirstLetter = true;
            wordChanges->spacesBefore++;
            wordChanges->spacesAfter++;
            continue;
        }

        if (bFirstLetter)
        {
            if (asciiIsLowerCase(*str[i]))
            {
                wordChanges->lowerCaseBefore++;
                *str[i] = asciiToUpperCase(*str[i]);
            }
            else
            {
                wordChanges->upperCaseBefore++;
            }

            wordChanges->upperCaseAfter++;
            bFirstLetter = false;
        }
        else
        {
            if (asciiIsUpperCase(*str[i]))
            {
                wordChanges->upperCaseBefore++;
                *str[i] = asciiToLowerCase(*str[i]);
            }
            else
            {
                wordChanges->lowerCaseBefore++;
            }

            wordChanges->lowerCaseAfter++;
        }
    }
}

// endregion

int main(void)
{
    char test[10] = { ' ', ' ', 'a', 'v', ' ', ' ', 'f', 'a', 'c', '\0' };

    struct WordChanges changes = { 0, 0, 0, 0, 0, 0 };

    const unsigned int len = 10;

    removeDuplicitSpaces(&test[0], &len, &changes);

    for (int i = 0; i < 10; i++)
    {
        printf("'%c' ", test[i]);
    }

    return 0;
}
