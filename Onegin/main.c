#include <stdio.h>
#include "includes/fileUtilities.h"

const char* SKIP_SYMBOLS = "!?.,;:-_\"'«»() ";

int DirectStrCmp(const String* str1, const String* str2)
{
    // assert(str1 != NULL);
    // assert(str2 != NULL);

    char* s1 = str1->string;
    char* s2 = str2->string;

    do
    {
        s1 += strspn(s1, SKIP_SYMBOLS);
        s2 += strspn(s2, SKIP_SYMBOLS);

        if (*s1 == '\0')
            return 0;

        if (*s1 == *s2)
        {
            ++s1;
            ++s2;
        }

    } while(*s1 == *s2);

    return (unsigned char)*s1 - (unsigned char)*s2;
}

int inverseStrCmp(const String* str1, const String* str2)
{
//    assert(x1 != NULL);
//    assert(x2 != NULL);

    char* s1 = str1->string + str1->length - 1;
    char* s2 = str2->string + str2->length - 1;

    do
    {
        while(strspn(s1, SKIP_SYMBOLS)) --s1;
        while(strspn(s2, SKIP_SYMBOLS)) --s2;

        if (*s1 == '\0')
            return 0;

        if (*s1 == *s2)
        {
            --s1;
            --s2;
        }

    } while (*s1 == *s2);

    return (unsigned char)*s1 - (unsigned char)*s2;
}

void StringSort(ParsedBuffer* parsed, int (*cmp)(const void*, const void*))
{
    for(int i = 0; i < parsed->size - 1; ++i)
        for(int j = i; j < parsed->size; ++j)
        {
            if (cmp(&parsed->data[i], &parsed->data[j]) > 0)
            {
                String temp = parsed->data[i];
                parsed->data[i] = parsed->data[j];
                parsed->data[j] = temp;
            }
        }
}

int main()
{
    char* buffer = NULL;
    char* fileName = "input.txt";
    ParsedBuffer data = {0};

    if (ReadToBuffer(fileName, &buffer) != OK)
    {
        printf("YOU DIED\n");
    }
    ParseBuffer(buffer, &data, '\n');

    StringSort(&data, (int(*) (const void *, const void *))inverseStrCmp);

    for (size_t i = 0; i < data.size; ++i)
        printf("%s\n", data.data[i].string, data.data[i].length);

    return 0;
}