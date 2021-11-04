#include <string.h>
#include <malloc.h>

#define STACK_CANARY_PROTECT
#define STACK_HASH_PROTECT
#define STACK_DUMPING

#define STACK_TYPE size_t
#include "../Stack/includes/stack.h"
#undef STACK_TYPE

void PrintSizeT(size_t s) { printf("%zu", s); }

typedef struct _List
{
    int* data;
    size_t* next;
    size_t* prev;

    size_t head;
    size_t tail;

    size_t capacity;
    size_t size;

    Stack_size_t* free;
} List;

void ListCtor(List* list, size_t capacity)
{
    list->capacity = capacity;
    list->size = 0;

    list->data = (int*) calloc(list->capacity + 1, sizeof(int));
    
    list->next = (size_t*) calloc(list->capacity + 1, sizeof(size_t));
    list->prev = (size_t*) calloc(list->capacity + 1, sizeof(size_t));

    list->free = (Stack_size_t*) calloc(1, sizeof(Stack_size_t));
    StackCtor(size_t, list->free, capacity, PrintSizeT);

    for (size_t i = capacity + 1; i > 0; --i)
        StackPush(size_t, list->free, i);

    list->size = 0;
    list->head = 0;
    list->tail = 0;
}

size_t ListPushBack(List* list, int value)
{
    size_t freeIndex = 0;
    StackPop(size_t, list->free, &freeIndex);

    list->data[freeIndex] = value;

    if (list->tail != 0)
        list->next[list->tail] = freeIndex;

    list->next[freeIndex] = 0;
    list->prev[freeIndex] = list->tail;

    if (list->head == 0)
        list->head = freeIndex;

    list->tail = freeIndex;

    return freeIndex;
}

void ListPopBack(List* list, int* out)
{
    size_t currentIndex = list->tail;

    *out = list->data[currentIndex];

    list->tail = list->prev[currentIndex];
    list->next[list->tail] = 0;

    StackPush(size_t, list->free, currentIndex);
}

size_t ListPushFront(List* list, int value)
{
    size_t freeIndex = 0;
    StackPop(size_t, list->free, &freeIndex);

    list->data[freeIndex] = value;

    // This node changes
    list->next[freeIndex] = list->head;
    list->prev[freeIndex] = 0;

    // Head node changes
    list->prev[list->head] = freeIndex;

    list->head = freeIndex;
    if (list->tail == 0)
        list->tail = freeIndex;

    return freeIndex;
}

void ListPopFront(List* list, int* out)
{
    size_t currentIndex = list->head;

    *out = list->data[currentIndex];

    list->head = list->next[currentIndex];
    list->prev[list->head] = 0;

    StackPush(size_t, list->free, currentIndex);
}

size_t ListInsert(List* list, size_t index, int value)
{
    size_t freeIndex = 0;
    StackPop(size_t, list->free, &freeIndex);

    list->data[freeIndex] = value;
    
    list->next[freeIndex] = list->next[index];
    list->prev[freeIndex] = index;

    list->prev[list->next[index]] = freeIndex;
    list->next[index] = freeIndex;

    return freeIndex;
}

void ListDelete(List* list, size_t index, int* out)
{
    *out = list->data[index];
    
    list->next[list->prev[index]] = list->next[index];
    list->prev[list->next[index]] = list->prev[index];

    StackPush(size_t, list->free, index);
}

int ListSize(List* list)
{
    return list->size;
}

void ListDump(List* list, size_t index)
{
    if (index == 0)
        return;

    printf("%zu\n", list->data[index]);
    ListDump(list, list->next[index]);
}