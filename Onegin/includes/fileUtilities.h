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
FileError ParseBuffer(char* buffer, ParsedBuffer* parsed, char symbol);
FileError ClearFile(const char* fileName);
