#include <stdio.h>

#include "../includes/list.h"

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
    List list;
    ListCtor(&list, 10);
    ListPushBack(&list, 25);
    ListPushBack(&list, 40);

    int temp;
    ListPopBack(&list, &temp);

    return temp;
}

int PushesFrontTest()
{
    List list;
    ListCtor(&list, 10);
    ListPushFront(&list, 25);
    ListPushFront(&list, 39);

    int temp;
    ListPopFront(&list, &temp);

    return temp;
}

int WithEmptyTest()
{
    List list;
    ListCtor(&list, 10);
    ListPushFront(&list, 25);
    ListPushFront(&list, 39);


    int temp;
    ListPopBack(&list, &temp);
    ListPopBack(&list, &temp);

    ListDump(&list, list.head);

    ListPushBack(&list, 5);
    ListPopFront(&list, &temp);

    return temp;
}


int VariativeTest()
{
    List list;
    ListCtor(&list, 10);
    size_t index = ListPushBack(&list, 10);
    ListPushBack(&list, 20);

    ListInsert(&list, index, 30);
    size_t index2 = ListInsert(&list, index, 40);
    int temp;
    ListDelete(&list, index2, &temp);

    ListPopBack(&list, &temp);
    ListPopBack(&list, &temp);

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