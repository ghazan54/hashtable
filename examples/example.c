#include <hashtable.h>
#include <stdio.h>

int main(void)
{
    hashtable head = hashtable_initialize();
    if (head) {
        hashtable_add(head, "your_key1", 2);
        hashtable_add(head, "your_key2", 876);
        hashtable_add(head, "your_key3", 10);

        hashtable_node ret = hashtable_lookup(head, "your_key2");
        printf("%d\n", ret->value); // 876
        printf("%p\n", hashtable_lookup(head, "test_key")); // (nil)

        hashtable_delete(head, "your_key1"); // deleted "your_key1"
        hashtable_delete(head, "test_key"); // no effect (the key is not in the hash table)

        hashtable_print(head);
    }
    hashtable_free(head);
    return 0;
}
