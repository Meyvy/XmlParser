#pragma once


#include "util.h"


typedef struct Node
{
    struct Node *next;
    struct Node *prev;
    void *data;
}Node;



Node* node_factory();
void free_node(Node**,void (*)(void*));

typedef struct LinkedList
{
    Node* head;
    Node* tail;
    uint64_t size;
}LinkedList;



LinkedList* linked_list_factory();
void free_linked_list(LinkedList**,void (*)(void*));
void add_to_empty(LinkedList*,Node*);
void add_linked_list_head(LinkedList*,Node*);
void add_linked_list_tail(LinkedList*,Node*);
State add_linked_list_before(LinkedList*,Node*,Node*);
State add_linked_list_after(LinkedList*,Node*,Node*);
Node* remove_linked_list_last(LinkedList*);
Node* remove_linked_list_head(LinkedList*);
Node* remove_linked_list_tail(LinkedList*);
Node* remove_linked_list_cur(LinkedList*,Node*);
Node* search_linked_list(LinkedList*,State (*)(Node*,void*),void*);
State check_node_default(Node *, void *);