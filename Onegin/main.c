#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "includes/fileUtilities.h"


#define CHECK(FUNC)               \
{                                 \
    FileError code = FUNC;        \
    if (code != OK) return code;  \
}


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

FileError OneginsProblem(const char* inputFile, const char* outputFile)
{
    FileApi api;
    if((api = FileApiInit()) == NULL)
    {
        assert(!"Fatal Error! File lib was not found");
        return -1;
    }

    char* buffer = NULL;
    FileError error = OK;

    CHECK( api->ClearFile(outputFile) )
    CHECK( api->ReadToBuffer(inputFile, &buffer) )

    ParsedBuffer parsed = {0};
    CHECK( ParseBuffer(buffer, &parsed, '\n') )

    ParsedBuffer standartText;
    CopyBuffer(&parsed, &standartText);

    StringSort(&parsed, (int(*) (const void *, const void *)) DirectStrCmp);
    CHECK( api->WriteFromBuffer(outputFile, &parsed) )

    //EmptyString
    CHECK( api->Write(outputFile, "\n") )

    qsort(parsed.data, parsed.size, sizeof(String), (int(*) (const void *, const void *)) inverseStrCmp);
    CHECK( api->WriteFromBuffer(outputFile, &parsed) )

    //EmptyString
    CHECK( api->Write(outputFile, "\n") )

    CHECK( api->WriteFromBuffer(outputFile, &standartText) )

    free(buffer);
    free(parsed.data);
    free(standartText.data);

    return OK;
}

int main()
{
    printf("Onegin's problem\n");
    printf("(c)Relayx (Nikita Zvezdin) 2021 v1.0\n\n");

    const char input[] = "input.txt";
    const char output[] = "output.txt";

    FileError code = OneginsProblem(input, output);
    if (code == OK)
    {
        printf("Success!\n");
        return 0;
    }

    char* errorCode = "";
    switch(code)
    {
    case CANNOT_OPEN_FILE:
        errorCode = "Couldn't open the file :(";
        break;
    case CANNOT_READ_FILE:
        errorCode = "Couldn't read the file :/";
        break;
    case OUT_OF_MEMORY:
        errorCode = "Not enough memory :<";
        break;
    default:
        errorCode = "Something went wrong... :P";
        break;
    }

    printf("\nAn error occurred!\n\tError code: %s\n", errorCode);

    return 0;
}