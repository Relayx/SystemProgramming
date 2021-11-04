#include "includes/fileUtilities.h"

#include <stdio.h>
#include <sys/stat.h>
#include <malloc.h>
#include <string.h>

///////////////////////////////////////////////////////////////////////////////////

FileError ReadToBuffer(const char* fileName, char** buffer)
{
    FILE* fin;
    if ((fin = fopen(fileName, "r")) == NULL)
    {
        return CANNOT_OPEN_FILE;
    }

    struct stat info;
    if (stat(fileName, &info) != OK)
        return CANNOT_OPEN_FILE;

    *buffer = (char*) calloc(info.st_size + 2, sizeof(char));
    if (*buffer == NULL)
        return OUT_OF_MEMORY;

    size_t size = fread(*buffer, sizeof(char), info.st_size, fin);
    // if (size != info.st_size)
    // {
    //     free(*buffer);
    //     return CANNOT_READ_FILE;
    // }

    *(*buffer + size) = '\n';
    *(*buffer + size + 1) = '\0';

    fclose(fin);

    return OK;
}

///////////////////////////////////////////////////////////////////////////////////

FileError ParseBuffer(char* buffer, ParsedBuffer* parsed, char symbol)
{
    parsed->size = 0;

    char* next = buffer;
    while((next = strchr(next, symbol)) != NULL)
    {
        while(*next == symbol) ++next;
        ++parsed->size;
    }

    parsed->data = (String*) calloc(parsed->size, sizeof(String));
    if (parsed->data == NULL)
        return OUT_OF_MEMORY;

    next          = buffer;
    char* last    = NULL;
    size_t length = 0;

    for (size_t i = 0; i < parsed->size; ++i)
    {
        parsed->data[i].string = next;
        last = next;

        next = strchr(next, symbol);
        length = next - last;
        parsed->data[i].length = length;

        while (*next == symbol)
        {
            *next = '\0';
            ++next;
        }
    }

    return OK;
}

///////////////////////////////////////////////////////////////////////////////////

FileError ClearFile(const char* fileName)
{
    FILE* fout;
    if ((fout = fopen(fileName, "w")) == NULL)
    {
        return CANNOT_OPEN_FILE;
    }

    fclose(fout);
    return OK;
}

///////////////////////////////////////////////////////////////////////////////////
