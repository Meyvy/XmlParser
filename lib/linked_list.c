#include "../include/linked_list.h"


//implementing Node methods

Node* node_factory()
{
    Node* node=(Node*)malloc(sizeof(Node));
    node->data=NULL;
    node->next=NULL;
    node->prev=NULL;
    return node;
}

void free_node(Node** node,void (*free_data)(void*))
{
    if((*node)->data!=NULL)
    {
        free_data((*node)->data);
    }

    free(*node);
    *node=NULL;
}

//implmenting LinkedList methods

LinkedList* linked_list_factory()
{
    LinkedList* list=(LinkedList*)malloc(sizeof(LinkedList));
    list->head=NULL;
    list->tail=NULL;
    list->size=0;
    return list;
}

void free_linked_list(LinkedList** list,void (*free_data)(void*))
{
    Node* cur=(*list)->head;
    Node* temp;
    while(cur)
    {
        temp=cur->next;
        free_node(&cur,free_data);
        cur=temp;
    }

    *list=NULL;
}

void add_to_empty(LinkedList* list,Node* node)
{
    list->head=node;
    list->tail=node;
    list->size++;
}

void add_linked_list_head(LinkedList* list,Node* node)
{
    if(list->size==0)
    {
        add_to_empty(list,node);
        return;
    }

    node->next=list->head;
    list->head->prev=node;
    list->head=node;
    list->size++;
}

void add_linked_list_tail(LinkedList* list,Node* node)
{
    if(list->size==0)
    {
        add_to_empty(list,node);
        return;
    }

    node->prev=list->tail;
    list->tail->next=node;
    list->tail=node;
    list->size++;
}

void add_linked_list_after(LinkedList* list,Node* cur,Node* node)
{
    if(list->tail==cur)
    {
        add_linked_list_tail(list,node);
        return;
    }

    node->prev=cur;
    node->next=cur->next;
    cur->next->prev=node;
    cur->next=node;
    list->size++;
}

void add_linked_list_before(LinkedList* list,Node* cur,Node* node)
{
    if(list->head==cur)
    {
        add_linked_list_head(list,node);
        return;
    }

    node->next=cur;
    node->prev=cur->prev;
    cur->prev->next=node;
    cur->prev=node;
    list->size++;
}

Node* remove_linked_list_last(LinkedList* list)
{
    Node* temp=list->head;
    list->head=NULL;
    list->tail=NULL;
    list->size--;
    return temp;
}

Node* remove_linked_list_head(LinkedList* list)
{
    if(list->size==0)
    {
        return NULL;
    }

    if(list->tail==list->head)
    {
        return remove_linked_list_last(list);
    }

    Node* temp=list->head;
    list->head->next->prev=NULL;
    list->head=list->head->next;
    list->size--;
    return temp;
}

Node* remove_linked_list_tail(LinkedList* list)
{
    if(list->size==0)
    {
        return NULL;
    }

    if(list->tail==list->head)
    {
        return remove_linked_list_last(list);
    }

    Node* temp=list->head;
    list->tail->prev->next=NULL;
    list->tail=list->tail->prev;
    list->size--;
    return temp;
}

Node* remove_linked_list_cur(LinkedList* list,Node* cur)
{
    if(list->size==0)
    {
        return NULL;
    }

    if(list->tail==list->head)
    {
        return remove_linked_list_last(list);
    }

    if(cur==list->head)
    {
        return remove_linked_list_head(list);
    }

    if(cur==list->tail)
    {
        return remove_linked_list_tail(list);
    }
    cur->next->prev=cur->prev;
    cur->prev->next=cur->next;
    list->size--;
    return cur;
}

Node* search_linked_list(LinkedList* list,int (*check_node)(Node*,void*),void* arg)
{
    Node* cur=list->head;
    
    while (cur)
    {
        if(check_node(cur,arg))
        {
            return cur;
        }
        cur=cur->next;
    }
    
    return NULL;
}