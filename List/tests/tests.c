#include <stdio.h>

#define LIST_TYPE int
#include "../includes/list.h"
#undef LIST_TYPE

const char* const RESET ="\033[0m";
const char* const RED ="\033[1;31m";
const char* const GREEN = "\033[1;32m";
const char* const YELLOW = "\033[1;33m";
const char* const BLUE = "\033[1;34m";

#define TEST_EQ(FUNC, VALUE) \
    if (FUNC() != VALUE) printf("%s[FAILED] %s%s\n", RED, #FUNC, RESET); \
    else printf("%s[PASSED] %s%s\n", GREEN, #FUNC, RESET);

int EqualListsForce(List_int* first, List_int* second)
{
    int equal = 0;
    equal |= first->head != second->head;
    equal |= first->tail != second->tail;
    equal |= first->size != second->size;
    equal |= first->capacity != second->capacity;

    for (size_t i = 0; i < first->capacity; ++i)
    {
        equal |= first->nodes[i].prev != second->nodes[i].prev;
        equal |= first->nodes[i].next != second->nodes[i].next;
        equal |= first->nodes[i].data != second->nodes[i].data;
    }

    return equal;
}

int EqualLists(List_int* first, List_int* second)
{
    int equal = 0;
    equal |= first->head != second->head;
    equal |= first->tail != second->tail;
    equal |= first->size != second->size;
    equal |= first->capacity != second->capacity;

    size_t curFirst = first->head;
    size_t curSecond = second->head;

    for (size_t i = 0; i < first->size; ++i)
    {
        equal |= first->nodes[curFirst].data != second->nodes[curSecond].data;
        curFirst = first->nodes[curFirst].next;
        curSecond = second->nodes[curSecond].next;
    }

    return equal;
}

#define EQUAL_LISTS 0

int PushesBackTest()
{
    List_int list = {0};
    ListCtor(int, &list, 10);
    ListPushBack(int, &list, 25);
    ListPushBack(int, &list, 40);

    int temp;
    ListPopBack(int, &list, &temp);

    return temp;
}

int PushesFrontTest()
{
    List_int list = {0};
    ListCtor(int, &list, 10);
    ListPushFront(int, &list, 25);
    ListPushFront(int, &list, 39);

    int temp;
    ListPopFront(int, &list, &temp);

    return temp;
}

int WithEmptyTest()
{
    List_int list = {0};
    ListCtor(int, &list, 10);
    ListPushFront(int, &list, 25);
    ListPushFront(int, &list, 39);

    int temp;
    ListPopBack(int, &list, &temp);
    ListPopBack(int, &list, &temp);

    ListPushBack(int, &list, 5);
    ListPopFront(int, &list, &temp);

    return temp;
}


int VariativeTest()
{
    List_int list = {0};
    ListCtor(int, &list, 10);
    size_t index = ListPushBack(int, &list, 10);
    ListPushBack(int, &list, 20);

    ListInsert(int, &list, index, 30);
    size_t index2 = ListInsert(int, &list, index, 40);
    int temp;
    ListDelete(int, &list, index2, &temp);

    ListPopBack(int, &list, &temp);
    ListPopBack(int, &list, &temp);

    return temp;
} 

int EmptyEqualTest()
{
    List_int first = {0};
    ListCtor(int, &first, 10);
    ListPushFront(int, &first, 25);
    ListPushFront(int, &first, 39);


    int temp;
    ListPopBack(int, &first, &temp);
    ListPopBack(int, &first, &temp);

    List_int second = {0};
    ListCtor(int, &second, 10);

    return EqualLists(&first, &second);
}

int EmptyEqualWithFrontTest()
{
    List_int first = {0};
    ListCtor(int, &first, 10);
    ListPushFront(int, &first, 25);
    ListPushFront(int, &first, 39);


    int temp;
    ListPopFront(int, &first, &temp);
    ListPopFront(int, &first, &temp);

    List_int second = {0};
    ListCtor(int, &second, 10);

    return EqualLists(&first, &second);
}

int OperationEqualityTest()
{
    List_int first = {0};
    ListCtor(int, &first, 10);
    ListPushFront(int, &first, 5);
    ListPushFront(int, &first, 10);
    ListPushFront(int, &first, 15);

    List_int second = {0};
    ListCtor(int, &second, 10);
    ListPushBack(int, &second, 15);
    ListPushBack(int, &second, 10);
    ListPushBack(int, &second, 5);

    int temp;
    ListPopBack(int, &first, &temp);
    ListPopFront(int, &second, &temp);

    ListPopBack(int, &second, &temp);
    ListPopFront(int, &first, &temp);

    return EqualLists(&first, &second);
}

int WorkStateTest()
{
    List_int first = {0};
    ListCtor(int, &first, 10);

    ListPushBack(int, &first, 5);
    size_t index = ListPushBack(int, &first, 10);
    ListPushBack(int, &first, 15);

    ListInsert(int, &first, index, 30);

    int temp;
    ListDelete(int, &first, index, &temp);
    ListPopBack(int, &first, &temp);
    ListPopFront(int, &first, &temp);

    List_int second = {0};
    ListCtor(int, &second, 10);
    Node_int node1 = {5, 4, 0};
    Node_int node2 = {10, 4, 1};
    Node_int node3 = {15, 0, 4};
    Node_int node4 = {30, 0, 0};
    second.nodes[1] = node1;
    second.nodes[2] = node2;
    second.nodes[3] = node3;
    second.nodes[4] = node4;

    second.size = 1;
    second.head = 4;
    second.tail = 4;

    return EqualListsForce(&first, &second);
}

int main()
{
    TEST_EQ(PushesBackTest, 40);
    TEST_EQ(PushesFrontTest, 39);
    TEST_EQ(WithEmptyTest, 5);
    TEST_EQ(VariativeTest, 30);
    TEST_EQ(EmptyEqualTest, EQUAL_LISTS);
    TEST_EQ(EmptyEqualWithFrontTest, EQUAL_LISTS);
    TEST_EQ(OperationEqualityTest, EQUAL_LISTS);
    TEST_EQ(WorkStateTest, EQUAL_LISTS);
    return 0;
}