/*!
\file
\brief Header file with description of the functions
*/

/***************************************************************************
* (c)Relayx (Nikita Zvezdin)
* Free for modification and distribution with the indication of the author
* GitHub: https://github.com/Relayx
***************************************************************************/

#ifndef RELAYX_FILE_UTIL_INCLUDED
#define RELAYX_FILE_UTIL_INCLUDED

#include <string.h>

typedef enum {
    OK = 0,
    CANNOT_OPEN_FILE,
    CANNOT_READ_FILE,
    OUT_OF_MEMORY
} FileError;

typedef struct _String
{
    char* string;
    size_t length;
} String;


typedef struct _ParsedBuffer
{
    String* data;
    size_t size;
} ParsedBuffer;

FileError ReadToBuffer(const char* fileName, char** buffer);
FileError WriteFromBuffer(const char* fileName, ParsedBuffer* parsed);

FileError ParseBuffer(char* buffer, ParsedBuffer* parsed, char symbol);
FileError CopyBuffer(ParsedBuffer* from, ParsedBuffer* to);

FileError ClearFile(const char* fileName);
FileError Write(const char* fileName, const char* string);

struct File_Api_v1_0
{
    FileError (*ReadToBuffer)(const char* fileName, char** buffer);
    FileError (*ParseBuffer)(char* buffer, ParsedBuffer* parsed, char symbol);
    FileError (*ClearFile)(const char* fileName);
    FileError (*WriteFromBuffer)(const char* fileName, ParsedBuffer* parsed);
    FileError (*Write)(const char* fileName, const char* string);
    FileError (*CopyBuffer)(ParsedBuffer* from, ParsedBuffer* to);
};

typedef const struct File_Api_v1_0* FileApi;
static FileApi FileApiInit()
{
    static const struct File_Api_v1_0 FileApi = { ReadToBuffer,
                                                  ParseBuffer,
                                                  ClearFile,
                                                  WriteFromBuffer,
                                                  Write,
                                                  CopyBuffer
                                                };
    return &FileApi;
}
    
#endif
