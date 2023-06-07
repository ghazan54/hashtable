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

CTEST(hashtable, initialize)
{
    hashtable head = hashtable_initialize();
    ASSERT_NOT_NULL(head);
    for (int i = 0; i < HASHTABLE_HEIGHT; ++i) {
        ASSERT_NULL(head[i]);
    }
    hashtable_free(head);
}

CTEST(hashtable, add)
{
    hashtable head = hashtable_initialize();
    ASSERT_TRUE(hashtable_add(head, "key1", 10));
    ASSERT_TRUE(hashtable_add(head, "key2", 20));

    hashtable_node node1 = hashtable_lookup(head, "key1");
    ASSERT_NOT_NULL(node1);
    ASSERT_STR("key1", node1->data);
    ASSERT_EQUAL(10, node1->value);

    hashtable_node node2 = hashtable_lookup(head, "key2");
    ASSERT_NOT_NULL(node2);
    ASSERT_STR("key2", node2->data);
    ASSERT_EQUAL(20, node2->value);

    hashtable_free(head);
}

CTEST(hashtable, lookup)
{
    hashtable head = hashtable_initialize();
    hashtable_add(head, "key1", 10);
    hashtable_add(head, "key2", 20);

    hashtable_node node1 = hashtable_lookup(head, "key1");
    ASSERT_NOT_NULL(node1);
    ASSERT_STR("key1", node1->data);
    ASSERT_EQUAL(10, node1->value);

    hashtable_node node2 = hashtable_lookup(head, "key2");
    ASSERT_NOT_NULL(node2);
    ASSERT_STR("key2", node2->data);
    ASSERT_EQUAL(20, node2->value);

    ASSERT_NULL(hashtable_lookup(head, "key3"));

    hashtable_free(head);
}

CTEST(hashtable, delete)
{
    hashtable head = hashtable_initialize();
    hashtable_add(head, "key1", 10);
    hashtable_add(head, "key2", 20);

    ASSERT_NOT_NULL(hashtable_lookup(head, "key1"));
    ASSERT_NOT_NULL(hashtable_lookup(head, "key2"));

    hashtable_delete(head, "key1");

    ASSERT_NULL(hashtable_lookup(head, "key1"));
    ASSERT_NOT_NULL(hashtable_lookup(head, "key2"));

    hashtable_delete(head, "key2");

    ASSERT_NULL(hashtable_lookup(head, "key2"));

    hashtable_free(head);
}

CTEST(hashtable, free)
{
    hashtable head = hashtable_initialize();
    hashtable_add(head, "key1", 10);
    hashtable_add(head, "key2", 20);

    hashtable_free(head);

    ASSERT_NULL(head);
}

int main(int argc, const char** argv)
{
    return ctest_main(argc, argv);
}
