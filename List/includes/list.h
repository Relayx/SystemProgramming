#include <string.h>
#include <malloc.h>

#define STACK_CANARY_PROTECT
#define STACK_HASH_PROTECT
#define STACK_DUMPING

#define STACK_TYPE size_t
#include "../Stack/includes/stack.h"
#undef STACK_TYPE

void PrintSizeT(size_t s) { printf("%zu", s); }

#define NEXT(i) list->nodes[i].next
#define PREV(i) list->nodes[i].prev

typedef struct _Node
{
    int data; // Th
    size_t next;
    size_t prev;
} Node;

typedef struct _List
{
    Node* nodes;

    size_t head;
    size_t tail;

    size_t capacity;
    size_t size;

    Stack_size_t* freeNodes;
} List;

void ListCtor(List* list, size_t capacity) //Th
{
    list->capacity = capacity;
    list->size = 0;

    list->nodes = (Node*) calloc(list->capacity + 1, sizeof(Node)); //th

    list->freeNodes = (Stack_size_t*) calloc(1, sizeof(Stack_size_t));
    StackCtor(size_t, list->freeNodes, capacity, PrintSizeT);

    for (size_t i = capacity + 1; i > 0; --i)
        StackPush(size_t, list->freeNodes, i);

    list->head = 0;
    list->tail = 0;
}

int ListIsEmpty(List* list) //Th
{
    return (list->size == 0);
}

size_t ListPushBack(List* list, int value) //Th
{
    size_t freeIndex = 0;
    StackPop(size_t, list->freeNodes, &freeIndex);

    // Insert current node
    list->nodes[freeIndex].data = value;
    list->nodes[freeIndex].next = 0;
    list->nodes[freeIndex].prev = list->tail;

    // Update previos node
    if (!ListIsEmpty(list))
        list->nodes[list->tail].next = freeIndex;

    // Update tail and head
    if (ListIsEmpty(list))
        list->head = freeIndex;
    list->tail = freeIndex;

    ++list->size;

    return freeIndex;
}

void ListPopBack(List* list, int* out) //th
{
    size_t currentIndex = list->tail;

    *out = list->nodes[currentIndex].data;

    // Update tail
    list->tail = list->nodes[currentIndex].prev;

    // Update previos node
    list->nodes[list->tail].next = 0;

    StackPush(size_t, list->freeNodes, currentIndex);

    --list->size;

    // Fill free element with poison
    // Update head???
}

size_t ListPushFront(List* list, int value)
{
    size_t freeIndex = 0;
    StackPop(size_t, list->freeNodes, &freeIndex);

    // Insert current node
    list->nodes[freeIndex].data = value;
    list->nodes[freeIndex].next = list->head;
    list->nodes[freeIndex].prev = 0;
 
    // Update next node
    if (!ListIsEmpty(list))
        list->nodes[list->head].prev = freeIndex;

    // Update tail and head
    if (ListIsEmpty(list))
        list->tail = freeIndex;
    list->head = freeIndex;

    ++list->size;

    return freeIndex;
}

void ListPopFront(List* list, int* out)
{
    size_t currentIndex = list->head;

    *out = list->nodes[currentIndex].data;

    // Update head
    list->head = list->nodes[currentIndex].next;

    // Update next node
    list->nodes[list->head].prev = 0;

    StackPush(size_t, list->freeNodes, currentIndex);

    --list->size;

    // Fill free element with poison
    // Update head???
}

size_t ListInsert(List* list, size_t index, int value)
{
    size_t freeIndex = 0;
    StackPop(size_t, list->freeNodes, &freeIndex);

    list->nodes[freeIndex].data = value;
    
    list->nodes[freeIndex].next = list->nodes[index].next;
    list->nodes[freeIndex].prev = index;

    list->nodes[list->nodes[index].next].prev = freeIndex;
    list->nodes[index].next = freeIndex;

    ++list->size;

    return freeIndex;
}

void ListDelete(List* list, size_t index, int* out)
{
    *out = list->nodes[index].data;
    
    list->nodes[list->nodes[index].prev].next = list->nodes[index].next;
    list->nodes[list->nodes[index].next].prev = list->nodes[index].prev;

    StackPush(size_t, list->freeNodes, index);

    --list->size;
}

int ListSize(List* list)
{
    return list->size;
}

void ListDump(List* list, size_t index)
{
    if (index == 0)
        return;

    printf("%zu next: %zu, prev: %zu\n", list->nodes[index].data, list->nodes[index].next, list->nodes[index].prev);
    ListDump(list, NEXT(index));
}