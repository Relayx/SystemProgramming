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

/*!
    Read data from file to buffer
    @param[in] fileName name of file
    @param[out] buffer destination of buffer to write data

    @return Error code
    @version 1.0
    @authors Relayx
*/
FileError ReadToBuffer(const char* fileName, char** buffer);

/*!
    Write data from buffer to file
    @param[in] fileName name of file
    @param[in] parsed parsed buffer [use ParseBuffer to parse buffer]

    @return Error code
    @version 1.0
    @authors Relayx
*/
FileError WriteFromBuffer(const char* fileName, ParsedBuffer* parsed);

//////////////////////////////////////////////////////////////////////////////////

/*!
    Parse data of buffer
    @param[in] buffer buffer for parsing
    @param[out] parsed empty object of ParsedBuffer
    @param[in] symbol delim for parsing

    @return Error code
    @version 1.0
    @authors Relayx
*/
FileError ParseBuffer(char* buffer, ParsedBuffer* parsed, char symbol);

/*!
    Copy data of parsedBuffer to another parsedBuffer
    @param[in] from current buffer
    @param[out] to destination buffer

    @return Error code
    @version 1.0
    @authors Relayx
*/
FileError CopyBuffer(ParsedBuffer* from, ParsedBuffer* to);

//////////////////////////////////////////////////////////////////////////////////

/*!
    Clear file
    @param[in] fileName name of file

    @return Error code
    @version 1.0
    @authors Relayx
*/
FileError ClearFile(const char* fileName);

/*!
    Write c-string to file
    @param[in] fileName name of file
    @param[in] string data to write

    @return Error code
    @version 1.0
    @authors Relayx
*/
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
