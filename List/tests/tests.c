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

int PushesBackTest()
{
    List_int list = {};
    ListCtor(int, &list, 10);
    ListPushBack(int, &list, 25);
    ListPushBack(int, &list, 40);

    int temp;
    ListPopBack(int, &list, &temp);

    return temp;
}

int PushesFrontTest()
{
    List_int list = {};
    ListCtor(int, &list, 10);
    ListPushFront(int, &list, 25);
    ListPushFront(int, &list, 39);

    int temp;
    ListPopFront(int, &list, &temp);

    return temp;
}

int WithEmptyTest()
{
    List_int list = {};
    ListCtor(int, &list, 10);
    ListPushFront(int, &list, 25);
    ListPushFront(int, &list, 39);


    int temp;
    ListPopBack(int, &list, &temp);
    ListPopBack(int, &list, &temp);

    ListDump(int, &list, list.head);

    ListPushBack(int, &list, 5);
    ListPopFront(int, &list, &temp);

    return temp;
}


int VariativeTest()
{
    List_int list = {};
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

int main()
{
    TEST_EQ(PushesBackTest, 40);
    TEST_EQ(PushesFrontTest, 39);
    TEST_EQ(WithEmptyTest, 5);
    TEST_EQ(VariativeTest, 30);
    return 0;
}