#pragma once
#include "stdlib.h"
#include "stdint.h"

typedef struct Node Node;

struct Node 
{
    Node *next;
    Node *prev;
    void *data;
};

Node* node_factory();
void free_node(Node**,void (*)(void*));


struct LinkedList
{
    Node* head;
    Node* tail;
    uint64_t size;
};
typedef struct LinkedList LinkedList;

LinkedList* linked_list_factory();
void free_linked_list(LinkedList**,void (*)(void*));
void add_to_empty(LinkedList*,Node*);
void add_linked_list_head(LinkedList*,Node*);
void add_linked_list_tail(LinkedList*,Node*);
void add_linked_list_before(LinkedList*,Node*,Node*);
void add_linked_list_after(LinkedList*,Node*,Node*);
Node* remove_linked_list_last(LinkedList*);
Node* remove_linked_list_head(LinkedList*);
Node* remove_linked_list_tail(LinkedList*);
Node* remove_linked_list_cur(LinkedList*,Node*);
Node* search_linked_list(LinkedList*,int (*)(Node*,void*),void*);