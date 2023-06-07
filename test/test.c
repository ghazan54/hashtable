#define CTEST_MAIN
#define CTEST_COLOR_OK

#include <ctest.h>
#include <hashtable.h>

CTEST(listnode, create_node)
{
    struct listnode* node = list_createnode("data", 10);

    ASSERT_NOT_NULL(node);
    ASSERT_STR("data", node->data);
    ASSERT_EQUAL(10, node->value);
    ASSERT_NULL(node->next);

    free(node->data);
    free(node);
}

CTEST(listnode, add_front)
{
    struct listnode* list = NULL;
    list = list_addfront(list, "data1", 10);
    list = list_addfront(list, "data2", 20);

    ASSERT_NOT_NULL(list);
    ASSERT_STR("data2", list->data);
    ASSERT_EQUAL(20, list->value);

    ASSERT_NOT_NULL(list->next);
    ASSERT_STR("data1", list->next->data);
    ASSERT_EQUAL(10, list->next->value);
    ASSERT_NULL(list->next->next);

    list = list_delete(list, "data1");
    list = list_delete(list, "data2");

    ASSERT_NULL(list);
}

CTEST(listnode, lookup)
{
    struct listnode* list = NULL;
    list = list_addfront(list, "data1", 10);
    list = list_addfront(list, "data2", 20);

    struct listnode* result1 = list_lookup(list, "data1");
    ASSERT_NOT_NULL(result1);
    ASSERT_STR("data1", result1->data);
    ASSERT_EQUAL(10, result1->value);

    struct listnode* result2 = list_lookup(list, "data2");
    ASSERT_NOT_NULL(result2);
    ASSERT_STR("data2", result2->data);
    ASSERT_EQUAL(20, result2->value);

    struct listnode* result3 = list_lookup(list, "data3");
    ASSERT_NULL(result3);

    list = list_delete(list, "data1");
    list = list_delete(list, "data2");

    ASSERT_NULL(list);
}

CTEST(listnode, delete_node)
{
    struct listnode* list = NULL;
    list = list_addfront(list, "data1", 10);
    list = list_addfront(list, "data2", 20);

    list = list_delete(list, "data1");

    ASSERT_NOT_NULL(list);
    ASSERT_STR("data2", list->data);
    ASSERT_EQUAL(20, list->value);
    ASSERT_NULL(list->next);

    list = list_delete(list, "data2");

    ASSERT_NULL(list);
}

CTEST(listnode, free)
{
    struct listnode* list = NULL;
    list = list_addfront(list, "data1", 10);
    list = list_addfront(list, "data2", 20);

    list_free(list);

    ASSERT_NULL(list);
}

int main(int argc, const char** argv)
{
    return ctest_main(argc, argv);
}
