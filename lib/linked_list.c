#include "../include/linked_list.h"

// implementing Node methods

Node *node_factory()
{
  Node *node = (Node *)malloc(sizeof(Node));
  node->data = NULL;
  node->next = NULL;
  node->prev = NULL;
  return node;
}

void free_node(Node **node, void (*free_data)(void *))
{
  if (*node == NULL)
  {
    return;
  }
  if ((*node)->data != NULL)
  {
    free_data((*node)->data);
  }

  free(*node);
  *node = NULL;
}

// implmenting LinkedList methods

LinkedList *linked_list_factory()
{
  LinkedList *list = (LinkedList *)malloc(sizeof(LinkedList));
  list->head = NULL;
  list->tail = NULL;
  list->size = 0;
  return list;
}

void free_linked_list(LinkedList **list, void (*free_data)(void *))
{
  if (*list == NULL)
  {
    return;
  }

  Node *cur = (*list)->head;
  Node *temp;

  while (cur)
  {
    temp = cur->next;
    free_node(&cur, free_data);
    cur = temp;
  }

  *list = NULL;
}

void add_linked_list_empty(LinkedList *list, Node *node)
{
  list->head = node;
  list->tail = node;
  list->size++;
}

void add_linked_list_head(LinkedList *list, Node *node)
{
  if (list->size == 0)
  {
    add_linked_list_empty(list, node);
    return;
  }

  node->next = list->head;
  list->head->prev = node;
  list->head = node;
  list->size++;
}

void add_linked_list_tail(LinkedList *list, Node *node)
{
  if (list->size == 0)
  {
    add_linked_list_empty(list, node);
    return;
  }

  node->prev = list->tail;
  list->tail->next = node;
  list->tail = node;
  list->size++;
}

State add_linked_list_after(LinkedList *list, Node *cur, Node *node)
{
  if (search_linked_list(list, check_node_default, (void *)cur) == NULL)
  {
    return NOT_OK;
  }

  if (list->tail == cur)
  {
    add_linked_list_tail(list, node);
    return OK;
  }

  node->prev = cur;
  node->next = cur->next;
  cur->next->prev = node;
  cur->next = node;
  list->size++;
  return OK;
}

State add_linked_list_before(LinkedList *list, Node *cur, Node *node)
{
  if (search_linked_list(list, check_node_default, (void *)cur) == NULL)
  {
    return NOT_OK;
  }

  if (list->head == cur)
  {
    add_linked_list_head(list, node);
    return OK;
  }

  node->next = cur;
  node->prev = cur->prev;
  cur->prev->next = node;
  cur->prev = node;
  list->size++;

  return OK;
}

Node *remove_linked_list_last(LinkedList *list)
{
  Node *temp = list->head;
  list->head = NULL;
  list->tail = NULL;
  list->size--;
  return temp;
}

Node *remove_linked_list_head(LinkedList *list)
{
  if (list->size == 0)
  {
    return NULL;
  }

  if (list->tail == list->head)
  {
    return remove_linked_list_last(list);
  }

  Node *temp = list->head;
  list->head->next->prev = NULL;
  list->head = list->head->next;
  list->size--;
  return temp;
}

Node *remove_linked_list_tail(LinkedList *list)
{
  if (list->size == 0)
  {
    return NULL;
  }

  if (list->tail == list->head)
  {
    return remove_linked_list_last(list);
  }

  Node *temp = list->head;
  list->tail->prev->next = NULL;
  list->tail = list->tail->prev;
  list->size--;
  return temp;
}

Node *remove_linked_list_cur(LinkedList *list, Node *cur)
{
  if (list->size == 0)
  {
    return NULL;
  }

  if (list->tail == list->head)
  {
    return remove_linked_list_last(list);
  }

  if (cur == list->head)
  {
    return remove_linked_list_head(list);
  }

  if (cur == list->tail)
  {
    return remove_linked_list_tail(list);
  }

  cur->next->prev = cur->prev;
  cur->prev->next = cur->next;
  list->size--;
  return cur;
}

Node *search_linked_list(LinkedList *list, State (*check_node)(Node *, void *),
                         void *arg)
{
  Node *cur = list->head;

  while (cur)
  {
    if (check_node(cur, arg) == OK)
    {
      return cur;
    }
    cur = cur->next;
  }

  return NULL;
}

State check_node_default(Node *node, void *data)
{
  data = (Node *)data;

  if (data == node)
  {
    return OK;
  }

  return NOT_OK;
}