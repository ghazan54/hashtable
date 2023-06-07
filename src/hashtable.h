#pragma once

#define HASHTABLE_HEIGHT (200003)

struct listnode {
    char* data;
    int value;
    struct listnode* next;
};

typedef struct listnode hashtable;

hashtable* hashtable_initialize(void);
void hashtable_add(hashtable* head, char* key, int value);
void hashtable_lookup(hashtable* head, char* key);
void hashtable_delete(hashtable* head, char* key);
void hashtable_free(hashtable* head);

struct listnode* list_createnode(char* data, int value);
struct listnode* list_addfront(struct listnode* list, char* data, int value);
struct listnode* list_lookup(struct listnode* list, char* data);
struct listnode* list_delete(struct listnode* list, char* data);
void list_free(struct listnode* list);
void list_print(struct listnode* list);
