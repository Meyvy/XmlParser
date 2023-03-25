#include "../include/ds.h"

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

// implementng Token methods

Token *token_factory()
{
  Token *token = (Token *)malloc(sizeof(Token));
  token->type = NO_TOK;
  token->value = NULL;
  return token;
}

void free_token(Token *token)
{
  if (token == NULL)
  {
    return;
  }

  if (token->value != NULL)
  {
    free(token->value);
  }
  free(token);
}

// implementing TokenList methods

TokenList *token_list_factory()
{
  return linked_list_factory();
}

void add_token_list(TokenList *token_list, Token *token)
{
  Node *node = node_factory();
  node->data = (void *)token;
  add_linked_list_tail(token_list, node);
}

void free_token_list(TokenList **token_list)
{
  free_linked_list(token_list, free_token);
}

// impleneting XmlDocument methods

XmlDocument *xml_factory()
{
  XmlDocument *xml = (XmlDocument *)malloc(sizeof(XmlDocument));
  xml->data = NULL;
  xml->source = NULL;
  xml->size = 0;
  return xml;
}

void free_xml(XmlDocument **xml)
{
  if (*xml == NULL)
  {
    return;
  }
  if ((*xml)->data != NULL)
  {
    free((*xml)->data);
  }
  if ((*xml)->source != NULL)
  {
    free((*xml)->source);
  }

  free(*xml);
  *xml = NULL;
}

State load_xml(XmlDocument *xml, char *path)
{
  FILE *file = fopen(path, "r");

  if (!file)
  {
    return NOT_OK;
  }

  fseek(file, 0, SEEK_END);
  uint64_t size = ftell(file);
  fseek(file, 0, SEEK_SET);

  xml->source = strdup(path);
  xml->size = size;
  xml->data = (char *)malloc(sizeof(char) * size + 1);

  fread(xml->data, 1, size, file);
  xml->data[size] = '\0';

  fclose(file);
  return OK;
}

// implementing XmlNode methods

XmlNode *xml_node_factory()
{
  XmlNode *xml_node = (XmlNode *)malloc(sizeof(XmlNode));
  xml_node->name = NULL;
  xml_node->body = NULL;
  xml_node->parent = NULL;
  xml_node->attributes = NULL;
  xml_node->children = xml_node_list_factory();
  return xml_node;
}

void free_xml_node(XmlNode **xml_node)
{
  if (*xml_node == NULL)
  {
    return;
  }
  if ((*xml_node)->body != NULL)
  {
    free((*xml_node)->body);
  }
  if ((*xml_node)->name != NULL)
  {
    free((*xml_node)->name);
  }

  free_xml_attribute_list(&(*xml_node)->attributes);
  free_xml_node_list(&(*xml_node)->children);
  free(*xml_node);
  *xml_node = NULL;
}

// implementing XmlAttribute methods

XmlAttribute *xml_attribute_factory()
{
  XmlAttribute *xml_attribute = (XmlAttribute *)malloc(sizeof(XmlAttribute));
  xml_attribute->name = NULL;
  xml_attribute->value = NULL;
  return xml_attribute;
}

void free_xml_attribute(XmlAttribute **xml_attribute)
{
  if (*xml_attribute == NULL)
  {
    return;
  }
  if ((*xml_attribute)->name != NULL)
  {
    free((*xml_attribute)->name);
  }
  if ((*xml_attribute)->value != NULL)
  {
    free((*xml_attribute)->value);
  }

  free(*xml_attribute);
  *xml_attribute = NULL;
}

// implementing XmlAttributeList methods

XmlAttributeList *xml_attribute_list_factory()
{
  return linked_list_factory();
}

void add_xml_attribute_list(XmlAttributeList *xml_attribute_list, XmlAttribute *xml_attribute)
{
  Node *node = node_factory();
  node->data = (void *)xml_attribute;
  add_linked_list_tail(xml_attribute_list, node);
}

void free_xml_attribute_list(XmlAttributeList **xml_attribute_list)
{
  free_linked_list(xml_attribute_list, free_xml_attribute);
}

// implementing XmlNodeList methods

XmlNodeList *xml_node_list_factory()
{
  return linked_list_factory();
}

void add_xml_node_list(XmlNodeList *xml_node_list, XmlNode *xml_node)
{
  Node *node = node_factory();
  node->data = (void *)xml_node;
  add_linked_list_tail(xml_node_list, node);
}

void free_xml_node_list(XmlNodeList **xml_node_list)
{
  free_linked_list(xml_node_list, free_xml_node);
}

// implementing XmlTree methods

XmlTree *xml_tree_factory()
{
  XmlTree *xml_tree = (XmlTree *)malloc(sizeof(XmlTree));
  xml_tree->root = node_factory();
  xml_tree->root->data = (void *)xml_node_factory();
  return xml_tree;
}

State is_root_xml_tree(Node *cur_node)
{
  if (cur_node && ((XmlNode *)cur_node->data)->parent == NULL)
  {
    return OK;
  }

  return NOT_OK;
}

State is_leaf_xml_tree(Node *cur_node)
{
  if (cur_node && !((XmlNode *)cur_node->data)->children->size)
  {
    return OK;
  }

  return NOT_OK;
}

void free_xml_tree(XmlTree **xml_tree)
{

  if (*xml_tree == NULL)
  {
    return;
  }

  Node *parent = NULL;
  Node *cur = (*xml_tree)->root;
  Node *temp;
  XmlNode *xml_node;
  State flag = NOT_OK;
  while (1)
  {
    xml_node = (XmlNode *)cur->data;
    if (is_leaf_xml_tree(cur) == OK || flag == OK)
    {
      temp = cur->next;
      printf("name:%s | body:%s\n", xml_node->name, xml_node->body);
      // free_node(&cur, free_xml_node);

      if (temp == NULL)
      {
        flag = OK;
        cur = parent;
        parent = ((XmlNode *)parent->data)->parent;

        if (is_root_xml_tree(cur) == OK)
        {
          break;
        }
      }
      else
      {
        flag = NOT_OK;
        cur = temp;
      }
    }
    else
    {
      temp = xml_node->children->head;
      parent = cur;
      cur = temp;
    }
  }
  free(*xml_tree);
  *xml_tree = NULL;
}

// implementing graph methods

Graph *graph_factory()
{
  Graph *graph = (Graph *)malloc(sizeof(Graph));
  graph->data = NULL;
  graph->size = 0;
  return graph;
}

void free_graph(Graph **graph)
{
  if (*graph == NULL)
  {
    return;
  }
  for (size_t i = 0; i < (*graph)->size; i++)
  {
    if ((*graph)->data[i] != NULL)
    {
      free((*graph)->data[i]);
    }
  }

  free(*graph);
  *graph = NULL;
}

void build_base_graph(Graph *graph, uint64_t size)
{
  if (graph->data)
  {
    free_graph(&graph);
  }
  graph->size = size;
  graph->data = (int **)malloc(sizeof(int) * size);

  for (uint64_t i = 0; i < size; i++)
  {
    graph->data[i] = (int *)malloc(sizeof(int) * size);
  }

  for (uint64_t i = 0; i < size; i++)
  {
    for (uint64_t j = 0; j < size; j++)
    {
      graph->data[i][j] = -1;
    }
  }
}

// NodePath methods

NodePath *node_path_factory()
{
  NodePath *node_path = (NodePath *)malloc(sizeof(NodePath));
  node_path->id = 0;
  node_path->path = linked_list_factory();
  return node_path;
}

void free_node_path(NodePath **node_path)
{
  if (*node_path == NULL)
  {
    return;
  }
  free_linked_list((*node_path)->path, free_node_path);
  free(*node_path);
  *node_path = NULL;
}

LinkedList *copy_linked_list(LinkedList *list)
{
  LinkedList *new_list = linked_list_factory();
  Node *temp = list->head;
  while (temp)
  {
    add_linked_list_tail(new_list, temp);
    temp = temp->next;
  }
  return new_list;
}

NodePath *bfs_graph(Graph *graph, uint64_t src, uint64_t goal)
{
  if (!graph)
  {
    return NULL;
  }

  uint64_t *visited = (uint64_t *)malloc(sizeof(graph->size));

  for (size_t i = 0; i < graph->size; i++)
  {
    visited[i] = 0;
  }

  Node *temp = node_factory();
  temp->data = (void *)node_path_factory();
  NodePath *node_path = (NodePath *)temp->data;
  node_path->id = src;
  add_linked_list_tail(node_path->path, temp);

  Node *cur;
  uint64_t cur_id;

  LinkedList *queue = linked_list_factory();
  add_linked_list_tail(queue, temp);

  while (queue->size)
  {

    cur = queue->head;
    node_path = (NodePath *)cur->data;
    cur_id = node_path->id;

    if (cur_id == goal)
    {
      return node_path;
    }

    for (size_t i = 0; i < graph->size; i++)
    {
      if (graph->data[cur_id][i] == 1 && visited[i] == 0)
      {
        temp = node_factory();
        temp->data = (void *)node_path_factory();
        node_path = (NodePath *)temp->data;
        node_path->id = i;
        node_path->path = copy_linked_list(((NodePath *)cur->data)->path);
        add_linked_list_tail(node_path->path, temp);
        add_linked_list_tail(queue, temp);
      }
    }
    visited[cur_id] = 1;
    remove_linked_list_head(queue);
  }

  return NULL;
}