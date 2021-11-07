
/***************************************************************************
* (c)Relayx (Nikita Zvezdin)
* Free for modification and distribution with the indication of the author
* GitHub: https://github.com/Relayx
***************************************************************************/

#ifdef LIST_TYPE

#include <string.h>
#include <malloc.h>

#define STACK_CANARY_PROTECT
#define STACK_HASH_PROTECT
#define STACK_DUMPING

#define STACK_TYPE size_t
#include "../Stack/includes/stack.h"
#undef STACK_TYPE

void PrintSizeT(size_t s) { printf("%zu", s); }

// Template defines
#define CAT(X,Y) X ## _ ## Y
#define TEMPLATE(X,Y) CAT(X,Y)



////////////////////////////////////////////////////////////////////////////////////
// ----------------------------->>> List definition <<<-----------------------------

#define NEXT(i) list->nodes[i].next
#define PREV(i) list->nodes[i].prev
#define DATA(i) list->nodes[i].data

#define NODE TEMPLATE(Node, LIST_TYPE)
typedef struct TEMPLATE(_Node, LIST_TYPE)
{
    LIST_TYPE data;
    size_t next;
    size_t prev;
} NODE;

#define LIST TEMPLATE(List, LIST_TYPE)
typedef struct TEMPLATE(_List, LIST_TYPE)
{
    NODE* nodes;

    size_t head;
    size_t tail;

    size_t capacity;
    size_t size;

    Stack_size_t* freeNodes;
} LIST;
////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////
// ------------------------------>>> List wrappers <<<------------------------------

#define ListIsEmpty(TYPE, LIST) \
    TEMPLATE(_ListIsEmpty, TYPE)(LIST)

#define ListCtor(TYPE, LIST, CAPACITY) \
    TEMPLATE(_ListCtor, TYPE)(LIST, CAPACITY)

#define ListDtor(TYPE, LIST) \
    TEMPLATE(_ListDtor, TYPE)(LIST)

#define ListPushBack(TYPE, LIST, VALUE) \
    TEMPLATE(_ListPushBack, TYPE)(LIST, VALUE)

#define ListPushFront(TYPE, LIST, VALUE) \
    TEMPLATE(_ListPushFront, TYPE)(LIST, VALUE)

#define ListPopBack(TYPE, LIST, OUT) \
    TEMPLATE(_ListPopBack, TYPE)(LIST, OUT)

#define ListPopFront(TYPE, LIST, OUT) \
    TEMPLATE(_ListPopFront, TYPE)(LIST, OUT)

#define ListInsert(TYPE, LIST, INDEX, VALUE) \
    TEMPLATE(_ListInsert, TYPE)(LIST, INDEX, VALUE)

#define ListDelete(TYPE, LIST, INDEX, OUT) \
    TEMPLATE(_ListDelete, TYPE)(LIST, INDEX, OUT)

#define ListDump(TYPE, LIST, START) \
    TEMPLATE(_ListDump, TYPE)(LIST, START)
////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////
// -------------------------->>> List native functions <<<--------------------------

void TEMPLATE(_ListCtor, LIST_TYPE)(LIST* list, size_t capacity)
{
    list->capacity = capacity;
    list->size = 0;

    list->nodes = (NODE*) calloc(list->capacity + 1, sizeof(NODE));

    list->freeNodes = (Stack_size_t*) calloc(1, sizeof(Stack_size_t));
    StackCtor(size_t, list->freeNodes, capacity, PrintSizeT);

    for (size_t i = capacity + 1; i > 0; --i)
        StackPush(size_t, list->freeNodes, i);

    list->head = 0;
    list->tail = 0;
}

//---------------------------------------------------------------------------------

void TEMPLATE(_ListDtor, LIST_TYPE)(LIST* list)
{
    memset(list->nodes, '\0', list->capacity * sizeof(NODE));
    free(list->nodes);
    StackDtor(size_t, list->freeNodes);

    list->capacity = 0;
    list->size = 0;

    list->head = 0;
    list->tail = 0;
}

//---------------------------------------------------------------------------------

void TEMPLATE(_ListResize, LIST_TYPE)(LIST* list, size_t capacity)
{
    list->capacity = capacity + 1;
    NODE* memory = (NODE*) realloc(list->nodes, list->capacity * sizeof(NODE));
    if (memory == NULL) return;
    list->nodes = memory;

    for (size_t i = list->size; i < list->capacity; ++i)
        StackPush(size_t, list->freeNodes, i);
}

//---------------------------------------------------------------------------------

int TEMPLATE(_ListIsEmpty, LIST_TYPE)(LIST* list)
{
    return (list->size == 0);
}

//---------------------------------------------------------------------------------

size_t TEMPLATE(_ListPushBack, LIST_TYPE)(LIST* list, int value)
{

    if (list->size == list->capacity - 1)
        TEMPLATE(_ListResize, LIST_TYPE)(list, (list->capacity - 1) * 2);

    size_t freeIndex = 0;
    StackPop(size_t, list->freeNodes, &freeIndex);

    // Insert current node
    DATA(freeIndex) = value;
    NEXT(freeIndex) = 0;
    PREV(freeIndex) = list->tail;

    // Update previos node
    if (!ListIsEmpty(LIST_TYPE, list))
        NEXT(list->tail) = freeIndex;

    // Update tail and head
    if (ListIsEmpty(LIST_TYPE, list))
        list->head = freeIndex;
    list->tail = freeIndex;

    ++list->size;

    return freeIndex;
}

//---------------------------------------------------------------------------------

void TEMPLATE(_ListPopBack, LIST_TYPE)(LIST* list, int* out)
{
    size_t currentIndex = list->tail;

    *out = DATA(currentIndex);

    // Update tail
    list->tail = PREV(currentIndex);

    // Update previos node
    NEXT(list->tail) = 0;

    StackPush(size_t, list->freeNodes, currentIndex);

    --list->size;

    if (ListIsEmpty(LIST_TYPE, list))
        list->head = 0;

    // Fill free element with poison
}

//---------------------------------------------------------------------------------

size_t TEMPLATE(_ListPushFront, LIST_TYPE)(LIST* list, int value)
{

    if (list->size == list->capacity - 1)
        TEMPLATE(_ListResize, LIST_TYPE)(list, (list->capacity - 1) * 2);

    size_t freeIndex = 0;
    StackPop(size_t, list->freeNodes, &freeIndex);

    // Insert current node
    DATA(freeIndex) = value;
    NEXT(freeIndex) = list->head;
    PREV(freeIndex) = 0;
 
    // Update next node
    if (!ListIsEmpty(LIST_TYPE, list))
        PREV(list->head) = freeIndex;

    // Update tail and head
    if (ListIsEmpty(LIST_TYPE, list))
        list->tail = freeIndex;
    list->head = freeIndex;

    ++list->size;

    return freeIndex;
}

//---------------------------------------------------------------------------------

void TEMPLATE(_ListPopFront, LIST_TYPE)(LIST* list, int* out)
{
    size_t currentIndex = list->head;

    *out = DATA(currentIndex);

    // Update head
    list->head = NEXT(currentIndex);

    // Update next node
    PREV(list->head) = 0;

    StackPush(size_t, list->freeNodes, currentIndex);

    --list->size;

    if (ListIsEmpty(LIST_TYPE, list))
        list->tail = 0;

    // Fill free element with poison
}

//---------------------------------------------------------------------------------

size_t TEMPLATE(_ListInsert, LIST_TYPE)(LIST* list, size_t index, int value)
{

    if (list->size == list->capacity - 1)
        TEMPLATE(_ListResize, LIST_TYPE)(list, (list->capacity - 1) * 2);

    size_t freeIndex = 0;
    StackPop(size_t, list->freeNodes, &freeIndex);

    list->nodes[freeIndex].data = value;
    
    NEXT(freeIndex) = NEXT(index);
    PREV(freeIndex) = index;

    PREV(NEXT(index)) = freeIndex;
    NEXT(index) = freeIndex;

    ++list->size;

    return freeIndex;
}

//---------------------------------------------------------------------------------

void TEMPLATE(_ListDelete, LIST_TYPE)(LIST* list, size_t index, int* out)
{
    *out = list->nodes[index].data;
    
    NEXT(PREV(index)) = NEXT(index);
    PREV(NEXT(index)) = PREV(index);

    StackPush(size_t, list->freeNodes, index);

    --list->size;
}

//---------------------------------------------------------------------------------

int TEMPLATE(_ListSize, LIST_TYPE)(LIST* list)
{
    return list->size;
}

//---------------------------------------------------------------------------------

void TEMPLATE(_ListDump, LIST_TYPE)(LIST* list, size_t index)
{
    if (index == 0)
        return;

    printf("%zu next: %zu, prev: %zu\n", list->nodes[index].data, list->nodes[index].next, list->nodes[index].prev);
    ListDump(LIST_TYPE, list, NEXT(index));
}

//---------------------------------------------------------------------------------

void TEMPLATE(_ListImage, LIST_TYPE)(LIST* list)
{
    FILE* fout;
    if ((fout = fopen("input.txt", "w")) == NULL)
    {
        return;
    }

    fprintf(fout, "digraph list {\nnode [colorscheme=paired12]\nedge [colorscheme=paired12]\nrankdir=LR\n");
    for (size_t i = 0; i < list->size; ++i)
    {
        fprintf(fout, "node%zu -> ", i);
    }
    fprintf(fout, "node%zu [style=invis]\n", list->size);

    for (size_t i = 0; i <= list->size; ++i)
    {
        fprintf(fout, "node%zu [shape=\"record\", style=\"rounded, bold\", color = %zu, label=\" <prev> prev | %d | <next> next \" ];\n",
                       i, i % 12 + 1, list->nodes[i].data);
    }

    for (size_t i = 0; i <= list->size; ++i)
    {
        fprintf(fout, "node%zu:<prev> -> node%zu [color = %zu]\n", i, list->nodes[i].prev, i % 12 + 1);
        fprintf(fout, "node%zu:<next> -> node%zu [color = %zu]\n", i, list->nodes[i].next, i % 12 + 1);
    }

    fprintf(fout, "}\n");
    fclose(fout);

    system("R:\\Programming\\C\\SystemProgramming\\List\\Graphviz\\bin\\dot.exe input.txt -Tpng -o test.png");
    system("start .\\test.png");
}

////////////////////////////////////////////////////////////////////////////////////


#undef LIST
#undef NODE

#endif //LIST_TYPE -----------------------------------------------------------------
