#include <hashtable.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct listnode* list_createnode(char* data, int value)
{
    struct listnode* p = NULL;
    p = malloc(sizeof(*p));
    if (p != NULL) {
        p->data = strdup(data);
        p->value = value;
        p->next = NULL;
    }
    return p;
}

struct listnode* list_addfront(struct listnode* list, char* data, int value)
{
    struct listnode* newnode;
    newnode = list_createnode(data, value);
    if (newnode != NULL) {
        newnode->next = list;
        return newnode;
    }
    return list;
}

struct listnode* list_lookup(struct listnode* list, char* data)
{
    for (; list != NULL; list = list->next) {
        if (strcmp(list->data, data) == 0) {
            return list;
        }
    }
    return NULL;
}

struct listnode* list_delete(struct listnode* list, char* data)
{
    struct listnode *p, *prev = NULL;
    for (p = list; p != NULL; p = p->next) {
        if (strcmp(p->data, data) == 0) {
            if (prev == NULL)
                list = p->next;
            else
                prev->next = p->next;
            if (p->data)
                free(p->data);
            free(p);
            return list;
        }
        prev = p;
    }
    return list;
}

void list_free(struct listnode* list)
{
    for (; list;) {
        struct listnode* next = list->next;
        if (list->data)
            free(list->data);
        free(list);
        list = next;
    }
}

void list_print(struct listnode* list)
{
    for (; list; list = list->next) {
        printf("[%s]%d ", list->data, list->value);
    }
    printf("\n");
}

unsigned int ELFHash(char* key)
{
    unsigned int h = 0, g;
    while (*key) {
        h = (h << 4) + *key++;
        g = h & 0xf0000000L;
        if (g)
            h ^= g >> 24;
        h &= ~g;
    }
    return h % HASHTABLE_HEIGHT;
}

hashtable hashtable_initialize(void)
{
    hashtable head = (hashtable)malloc(sizeof(*head) * HASHTABLE_HEIGHT);
    if (head) {
        for (int i = 0; i < HASHTABLE_HEIGHT; ++i)
            head[i] = NULL;
    }
    return head;
}

bool hashtable_add(hashtable head, char* key, int value)
{
    if (head && key) {
        unsigned int h = ELFHash(key);
        head[h] = list_addfront(head[h], key, value);
        return true;
    }
    return false;
}

hashtable_node hashtable_lookup(hashtable head, char* key)
{
    if (head && key) {
        unsigned int h = ELFHash(key);
        hashtable_node ret = list_lookup(head[h], key);
        return ret;
    }
    return NULL;
}

void hashtable_delete(hashtable head, char* key)
{
    if (head && key) {
        unsigned int h = ELFHash(key);
        head[h] = list_delete(head[h], key);
    }
}

void hashtable_print(hashtable head)
{
    if (head) {
        for (int i = 0; i < HASHTABLE_HEIGHT; ++i) {
            if (head[i])
                list_print(head[i]);
        }
    }
}

void hashtable_free(hashtable head)
{
    if (head) {
        for (int i = 0; i < HASHTABLE_HEIGHT; ++i) {
            if (head[i])
                list_free(head[i]);
        }
        free(head);
    }
}
