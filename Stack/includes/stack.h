
/***************************************************************************
* (c)Relayx (Nikita Zvezdin)
* Free for modification and distribution with the indication of the author
* GitHub: https://github.com/Relayx
***************************************************************************/

#ifdef STACK_TYPE

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

///////////////////////////////////////////////////////////////////////////////////
// ----------------------->>> Static utilities for stack <<<-----------------------

#ifndef STACK_STATIC_UTILITIES
#define STACK_STATIC_UTILITIES

// Template defines
#define CAT(X,Y) X ## _ ## Y
#define TEMPLATE(X,Y) CAT(X,Y)

typedef uint32_t error_t;
static const uint8_t EXPAND_VALUE = 2;



#ifdef STACK_DUMPING

typedef struct _StackInfo
{
    const char*  type;
    const char*  varName;
    const char*  file;
    const char*  function;
    const size_t line;
} StackInfo;

static StackInfo StackInfoCtor(const char* type, const char* varName, const char* file,
                               const char* function, size_t line)
{
    StackInfo info = {type, varName, file, function, line};

    return info;
}

#endif // STACK_DUMPING -----------------------------------------------------------



typedef enum _StackErrors
{
    OK = 0,                    // 0
    NULL_REFERENCE,            // 1
    NULL_REFERENCE_DATA,       // 2
    OUT_OF_MEMORY,             // 3
    BAD_STACK_VALUES,          // 4
    STRUCT_LEFT_CANARY_FAULT,  // 5
    STRUCT_RIGHT_CANARY_FAULT, // 6
    DATA_LEFT_CANARY_FAULT,    // 7
    DATA_RIGHT_CANARY_FAULT,   // 8
    STRUCT_HASH_FAULT,         // 9
    DATA_HASH_FAULT,           // 10
    ALREADY_CONSTRUCTED,       // 11
    ALREADY_DELETED,           // 12
    ERROR_ENUM_SIZE            // 13
} StackErrors;

static const char* ERROR_INFO[] = 
    {
        "OK",                                      /* 0 */
        "Null pointer reference of stack",         /* 1 */
        "Null pointer reference of data of stack", /* 2 */
        "Out of memory",                           /* 3 */
        "Stack values was spoiled",                /* 4 */
        "Left canary of struct was damaged",       /* 5 */
        "Right canary of struct was damaged",      /* 6 */
        "Left canary of data was damaged",         /* 7 */
        "Right canary of data was damaged",        /* 8 */
        "Hash of struct doesn't match",            /* 9 */
        "Hash of data doesn't match",              /* 10 */
        "Stack has already constructed",           /* 11 */
        "Stack has already deleted"                /* 12 */
    };



#ifdef STACK_CANARY_PROTECT

typedef uint64_t canary_t;
static const canary_t CANARY_VALUE = 0xFEEDC0FFEE2FACE;

static inline canary_t* GetStartCanary(const char* data)
{
    return (canary_t*)(data - sizeof(canary_t));
}

static inline canary_t* GetEndCanary(const char* data, size_t dataSize)
{
    return (canary_t*)(data + dataSize);
}

#endif // STACK_CANARY_PROTECT ----------------------------------------------------



#ifdef STACK_HASH_PROTECT

typedef uint64_t hash_t;

static hash_t Hash(const char* data, size_t dataSize)
{
    hash_t hash = 5381;

    for (size_t i = 0; i < dataSize; ++i)
    {
        hash += (hash << 5) + data[i];
    }

    return hash;
}

#endif // STACK_HASH_PROTECT ------------------------------------------------------



// ----------------------->>> Wrappers for stack methods <<<-----------------------

#ifdef STACK_DUMPING // With Dumping
/*---*/
/*---*/ #define StackCtor(TYPE, STACK, CAPACITY, PRINT) \
/*---*/     TEMPLATE(_StackCtor, TYPE)(STACK, CAPACITY, PRINT, \
/*---*/     StackInfoCtor( #TYPE, #STACK, __FILE__, __PRETTY_FUNCTION__, __LINE__))
/*---*/
/*---*/ #define StackDtor(TYPE, STACK) \
/*---*/     TEMPLATE(_StackDtor, TYPE)(STACK, \
/*---*/     StackInfoCtor( #TYPE, #STACK, __FILE__, __PRETTY_FUNCTION__, __LINE__))
/*---*/
/*---*/ #define StackPush(TYPE, STACK, VALUE) \
/*---*/     TEMPLATE(_StackPush, TYPE)(STACK, VALUE, \
/*---*/     StackInfoCtor( #TYPE, #STACK, __FILE__, __PRETTY_FUNCTION__, __LINE__))
/*---*/
/*---*/ #define StackPop(TYPE, STACK, VALUE) \
/*---*/     TEMPLATE(_StackPop, TYPE)(STACK, VALUE, \
/*---*/     StackInfoCtor( #TYPE, #STACK, __FILE__, __PRETTY_FUNCTION__, __LINE__))
/*---*/
/*---*/ #define StackSize(TYPE, STACK, VALUE) \
/*---*/     TEMPLATE(_StackSize, TYPE)(STACK, VALUE, \
/*---*/     StackInfoCtor( #TYPE, #STACK, __FILE__, __PRETTY_FUNCTION__, __LINE__))
/*---*/
/*---*/ #define StackDump(TYPE, STACK) \
/*---*/     TEMPLATE(_StackDump, TYPE)(STACK, \
/*---*/     StackInfoCtor( #TYPE, #STACK, __FILE__, __PRETTY_FUNCTION__, __LINE__), \
/*---*/     TEMPLATE(_StackVerify, TYPE)(STACK))
/*---*/
/*---*/ #define STACK_OK \
/*---*/ { \
/*---*/     error_t error = TEMPLATE(_StackVerify, STACK_TYPE)(stack); \
/*---*/         if (error != OK) \
/*---*/             return TEMPLATE(_StackDump, STACK_TYPE)(stack, info, error); \
/*---*/ }
/*---*/
/*---*/ #define DROP_ERROR(EXPRESSION, ERROR) \
/*---*/     if (EXPRESSION) \
/*---*/         return TEMPLATE(_StackDump, STACK_TYPE)(stack, info, (1u << ERROR));
/*---*/
#else // Without Dumping
/*---*/
/*---*/ #define StackCtor(TYPE, STACK, CAPACITY, PRINT) \
/*---*/     TEMPLATE(_StackCtor, TYPE)(STACK, CAPACITY)
/*---*/
/*---*/ #define StackDtor(TYPE, STACK) \
/*---*/     TEMPLATE(_StackDtor, TYPE)(STACK)
/*---*/
/*---*/ #define StackPush(TYPE, STACK, VALUE) \
/*---*/     TEMPLATE(_StackPush, TYPE)(STACK, VALUE)
/*---*/
/*---*/ #define StackPop(TYPE, STACK, VALUE) \
/*---*/     TEMPLATE(_StackPop, TYPE)(STACK, VALUE)
/*---*/
/*---*/ #define StackSize(TYPE, STACK, VALUE) \
/*---*/     TEMPLATE(_StackSize, TYPE)(STACK, VALUE)
/*---*/
/*---*/ #define StackDump(TYPE, STACK)
/*---*/
/*---*/ #define STACK_OK \
/*---*/ { \
/*---*/     error_t error = TEMPLATE(_StackVerify, STACK_TYPE)(stack); \
/*---*/         if (error != OK) \
/*---*/             return error; \
/*---*/ }
/*---*/
/*---*/ #define DROP_ERROR(EXPRESSION, ERROR) \
/*---*/     if (EXPRESSION) \
/*---*/         return (1u << ERROR);
/*---*/ 
#endif // STACK_DUMPING -----------------------------------------------------------

#define StackVerify(TYPE, STACK) \
    TEMPLATE(_StackVerify, TYPE)(STACK)

#endif // STACK_STATIC_UTILITIES --------------------------------------------------
///////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////
// -------------------------->>> Stack initialization <<<--------------------------

#define STACK TEMPLATE(Stack, STACK_TYPE)
// typedef struct _STACK_T {...} Stack_T;

typedef struct TEMPLATE(_Stack, STACK_TYPE)
{
/*
          ↱ (canaryLeft)      ↱ (canaryRight)
    ---+---+---+---+---+---+---+---+---
       | L |   | D |   |   | R |   |     --->>> Canary protection for struct.
    ---+---+---+---+---+---+---+---+--- 
                 |
                 |
                 ↓
    ---+---+---+---+---+---+---+---+---+---
       |   | L |   |   |   |   |   | R |     --->>> Canary protection for data.
    ---+---+---+---+---+---+---+---+---+--- 
             ↪ (canaryLeft)          ↪ (canaryRight)
*/
#ifdef STACK_CANARY_PROTECT
    canary_t canaryLeft;
#endif // STACK_CANARY_PROTECT

    size_t      size;
    size_t      capacity;
    STACK_TYPE* data;

#ifdef STACK_DUMPING
    void (*PrintElement)(STACK_TYPE);
#endif // STACK_DUMPING

#ifdef STACK_HASH_PROTECT
    hash_t hash;
#endif // STACK_HASH_PROTECT

#ifdef STACK_CANARY_PROTECT
    canary_t canaryRight;
#endif // STACK_CANARY_PROTECT

#ifdef STACK_HASH_PROTECT
    hash_t structHash;
#endif //STACK_HASH_PROTECT

} STACK;
///////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////
// -------------------->>> Dumping and ErrorsChecking tools <<<--------------------

error_t TEMPLATE(_StackVerify, STACK_TYPE)(STACK* stack)
{
    error_t error = 0;

    if (!stack)       return (1u << NULL_REFERENCE);
    if (!stack->data) return (1u << NULL_REFERENCE_DATA);

    if (stack->size > stack->capacity) error |= (1u << BAD_STACK_VALUES);

#ifdef STACK_CANARY_PROTECT

    if (stack->canaryLeft != CANARY_VALUE)  error |= (1u << STRUCT_LEFT_CANARY_FAULT);
    if (stack->canaryRight != CANARY_VALUE) error |= (1u << STRUCT_RIGHT_CANARY_FAULT);
    
    if (*(GetStartCanary((char*)stack->data)) != CANARY_VALUE)
        error |= (1u << DATA_LEFT_CANARY_FAULT);

    if (*(GetEndCanary((char*)stack->data, 
                       stack->capacity * sizeof(STACK_TYPE))) != CANARY_VALUE)
        error |= (1u << DATA_RIGHT_CANARY_FAULT);

#endif // STACK_CANARY_PROTECT

#ifdef STACK_HASH_PROTECT

    if (stack->hash != Hash((char*)stack->data, stack->capacity * sizeof(STACK_TYPE)))
        error |= (1u << DATA_HASH_FAULT);

    if (stack->structHash != Hash((char*)stack, sizeof(STACK) - sizeof(hash_t)))
        error |= (1u << STRUCT_HASH_FAULT);

#endif // STACK_HASH_PROTECT

    return error;
}

//---------------------------------------------------------------------------------

#ifdef STACK_DUMPING

error_t TEMPLATE(_StackDump, STACK_TYPE)(STACK* stack, StackInfo info, error_t error)
{
    // Colors for console log
    const char* const RESET  ="\033[0m";
    const char* const RED    ="\033[1;31m";
    const char* const GREEN  = "\033[1;32m";
    const char* const YELLOW = "\033[1;33m";
    const char* const BLUE   = "\033[1;34m";

    printf("Stack<%s%s%s>[%s%p%s] \"%s%s%s\" at function: %s%s%s at file: %s%s(%zu)%s\n",
           YELLOW, info.type, RESET, BLUE, stack, RESET, YELLOW, info.varName, RESET, YELLOW,
           info.function, RESET, YELLOW, info.function, info.line, RESET);

    printf("Status: ");

    if (error == OK)
    {
        printf("%s%s%s\n", GREEN, ERROR_INFO[OK] ,RESET);
    }
    else
    {
        int errorsNum = 0;

        for (unsigned int i = 1; i < ERROR_ENUM_SIZE; ++i)
            if (error & (1u << i)) ++errorsNum;

        printf("%sErrors(%d)%s:\n", RED, errorsNum, RESET);

        for (unsigned int i = 1; i < ERROR_ENUM_SIZE; ++i)
        {
            if (error & (1u << i))
                printf("%s%s <<- ERROR_CODE(%d)%s\n", RED, ERROR_INFO[i], i, RESET);
            if (error & (1u << NULL_REFERENCE)) return error;
        }
    }
    
    printf("{\n");

    printf("\t%ssize%s = %zu\n\t%scapacity%s = %zu\n\t%sdata%s[%s%p%s]\n",
        YELLOW, RESET, stack->size, YELLOW, RESET,
        stack->capacity, YELLOW, RESET, BLUE, stack->data, RESET);

    if (error & (1u << NULL_REFERENCE_DATA))
    {
        printf("}\n");
        return error;
    }

    printf("\t{\n");

    for (size_t i = 0; i < stack->capacity; ++i)
    {
        if (i < stack->size) printf("\t\t%s[%zu]%s = ", GREEN, i, RESET);
        else printf("\t\t%s[%zu]%s = ", RED, i, RESET);
        
        if (stack->PrintElement == NULL)
        {
            printf("%s!Print function is not assigned!%s\n", RED, RESET);
            continue;
        }
        else stack->PrintElement(stack->data[i]);

        if (i >= stack->size) printf(" %s(TRASH)%s\n", RED, RESET);
        else printf("\n");
    }

    printf("\t}\n");
    printf("}\n");

    return error;
}

#endif // STACK_DUMPING -----------------------------------------------------------
///////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////
// ------------------------->>> Stack native functions <<<-------------------------

#ifdef STACK_DUMPING

error_t TEMPLATE(_StackCtor, STACK_TYPE)(STACK* stack, size_t capacity, 
                 void (*PrintElement)(STACK_TYPE), StackInfo info)

#else

error_t TEMPLATE(_StackCtor, STACK_TYPE)(STACK* stack, size_t capacity)

#endif // STACK_DUMPING

{
    DROP_ERROR(!stack, NULL_REFERENCE); // !Error
    DROP_ERROR(stack->data != NULL, ALREADY_CONSTRUCTED); // !Error
    DROP_ERROR(stack->capacity != 0, ALREADY_CONSTRUCTED); // !Error
    DROP_ERROR(stack->size != 0, ALREADY_CONSTRUCTED); // !Error

#ifdef STACK_CANARY_PROTECT

    char* memory = (char*) calloc((capacity * sizeof(STACK_TYPE)) + 
                                  (2 * sizeof(canary_t)), sizeof(char));
    DROP_ERROR(memory == NULL, OUT_OF_MEMORY); // !Error
    memory += sizeof(canary_t);

    stack->canaryLeft                                      = CANARY_VALUE;
    stack->canaryRight                                     = CANARY_VALUE;
    *(GetStartCanary(memory))                              = CANARY_VALUE;
    *(GetEndCanary(memory, capacity * sizeof(STACK_TYPE))) = CANARY_VALUE;

#else

    STACK_TYPE* memory = (STACK_TYPE*) calloc(capacity, sizeof(STACK_TYPE));
    DROP_ERROR(memory == NULL, OUT_OF_MEMORY); // !Error

#endif // STACK_CANARY_PROTECT

    stack->data     = (STACK_TYPE*) memory;
    stack->capacity = capacity;
    stack->size     = 0;

#ifdef STACK_DUMPING
    stack->PrintElement = PrintElement;
#endif // STACK_DUMPING

#ifdef STACK_HASH_PROTECT
    stack->hash       = Hash((char*)stack->data, stack->capacity * sizeof(STACK_TYPE));
    stack->structHash = Hash((char*)stack, sizeof(STACK) - sizeof(hash_t));
#endif // STACK_HASH_PROTECT

    STACK_OK

    return OK;
}

//---------------------------------------------------------------------------------

#ifdef STACK_DUMPING

error_t TEMPLATE(_StackDtor, STACK_TYPE)(STACK* stack, StackInfo info)

#else

error_t TEMPLATE(_StackDtor, STACK_TYPE)(STACK* stack)

#endif // STACK_DUMPING

{
    DROP_ERROR(!stack, NULL_REFERENCE); // !Error
    DROP_ERROR(stack->data == NULL && stack->capacity == 0 && stack->size == 0, 
               ALREADY_DELETED); // !Error
    STACK_OK

#ifdef STACK_CANARY_PROTECT

    char* memory = (char*)stack->data - sizeof(canary_t);
    memset(memory, '\0', (stack->capacity * sizeof(STACK_TYPE)) + 
                       + (2 * sizeof(canary_t)));

#else

    char* memory = (char*)stack->data;
    memset(memory, '\0', stack->capacity * sizeof(STACK_TYPE));

#endif // STACK_CANARY_PROTECT

    free(memory);
    stack->data     = NULL;
    stack->size     = 0;
    stack->capacity = 0;

    return OK;
}

//---------------------------------------------------------------------------------

#ifdef STACK_DUMPING

error_t TEMPLATE(_StackResize, STACK_TYPE)(STACK* stack, size_t capacity, StackInfo info)

#else

error_t TEMPLATE(_StackResize, STACK_TYPE)(STACK* stack, size_t capacity)

#endif // STACK_DUMPING

{
    STACK_OK
    
    stack->capacity = capacity;

#ifdef STACK_CANARY_PROTECT

    char* memory = (char*)stack->data - sizeof(canary_t);
    memory = (char*) realloc(memory, (stack->capacity * sizeof(STACK_TYPE)) + 
                                   + (2 * sizeof(canary_t)));
    DROP_ERROR(memory == NULL, OUT_OF_MEMORY); // !Error

    stack->data = (STACK_TYPE*)(memory + sizeof(canary_t));

    *(GetStartCanary((char*)stack->data))                                     = CANARY_VALUE;
    *(GetEndCanary((char*)stack->data, stack->capacity * sizeof(STACK_TYPE))) = CANARY_VALUE;

#else

    char* memory = (char*) realloc(stack->data, stack->capacity * sizeof(STACK_TYPE));
    DROP_ERROR(memory == NULL, OUT_OF_MEMORY); // !Error
    stack->data = (STACK_TYPE*) memory;

#endif // STACK_CANARY_PROTECT

#ifdef STACK_HASH_PROTECT
    stack->hash       = Hash((char*)stack->data, stack->capacity * sizeof(STACK_TYPE));
    stack->structHash = Hash((char*)stack, sizeof(STACK) - sizeof(hash_t));
#endif // STACK_HASH_PROTECT

    STACK_OK

    return OK;
}

//---------------------------------------------------------------------------------

#ifdef STACK_DUMPING

error_t TEMPLATE(_StackPush, STACK_TYPE)(STACK* stack, STACK_TYPE value, StackInfo info)

#else

error_t TEMPLATE(_StackPush, STACK_TYPE)(STACK* stack, STACK_TYPE value)

#endif // STACK_DUMPING

{
    STACK_OK

#ifdef STACK_DUMPING

    if (stack->size >= stack->capacity)
    {
        error_t error = TEMPLATE(_StackResize, STACK_TYPE)
            (stack, stack->capacity * EXPAND_VALUE, info);
        if (error != OK) return error;
    }

#else

    if (stack->size >= stack->capacity)
    {
        error_t error = TEMPLATE(_StackResize, STACK_TYPE)
            (stack, stack->capacity * EXPAND_VALUE);
        if (error != OK) return error;
    }

#endif // STACK_DUMPING
    
    stack->data[stack->size++] = value;

#ifdef STACK_HASH_PROTECT
    stack->hash       = Hash((char*)stack->data, stack->capacity * sizeof(STACK_TYPE));
    stack->structHash = Hash((char*)stack, sizeof(STACK) - sizeof(hash_t));
#endif // STACK_HASH_PROTECT

    STACK_OK

    return OK;
}

//---------------------------------------------------------------------------------

#ifdef STACK_DUMPING

error_t TEMPLATE(_StackPop, STACK_TYPE)(STACK* stack, STACK_TYPE* value, StackInfo info)

#else

error_t TEMPLATE(_StackPop, STACK_TYPE)(STACK* stack, STACK_TYPE* value)

#endif // STACK_DUMPING

{
    STACK_OK
    DROP_ERROR(!value, NULL_REFERENCE); // !Error
    DROP_ERROR(stack->size == 0, BAD_STACK_VALUES); // !Error

    *value = stack->data[--stack->size];

#ifdef STACK_HASH_PROTECT
    stack->hash       = Hash((char*)stack->data, stack->capacity * sizeof(STACK_TYPE));
    stack->structHash = Hash((char*)stack, sizeof(STACK) - sizeof(hash_t));
#endif // STACK_HASH_PROTECT

#ifdef STACK_DUMPING

    if (stack->size < stack->capacity / (EXPAND_VALUE * 2)) 
    {
        error_t error = TEMPLATE(_StackResize, STACK_TYPE)
            (stack, stack->capacity / EXPAND_VALUE, info);
        if (error != OK) return error;
    }

#else

    if (stack->size < stack->capacity / (EXPAND_VALUE * 2)) 
    {
        error_t error = TEMPLATE(_StackResize, STACK_TYPE)
            (stack, stack->capacity / EXPAND_VALUE);
        if (error != OK) return error;
    }

#endif // STACK_DUMPING

    STACK_OK

    return OK;
}

//---------------------------------------------------------------------------------

#ifdef STACK_DUMPING

error_t TEMPLATE(_StackSize, STACK_TYPE)(STACK* stack, size_t* size, StackInfo info)

#else

error_t TEMPLATE(_StackSize, STACK_TYPE)(STACK* stack, size_t* size)

#endif // STACK_DUMPING

{
    STACK_OK
    DROP_ERROR(!size, NULL_REFERENCE); // !Error

    *size = stack->size;

    return OK;
}

///////////////////////////////////////////////////////////////////////////////////



#undef STACK

#endif //STACK_TYPE ---------------------------------------------------------------
