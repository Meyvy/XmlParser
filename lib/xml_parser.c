#include "../include/xml_parser.h"

XmlAttribute *get_attribute(Node **cur_node)
{
  char name[MAX_STRING_TOK_VALUE_SIZE];
  char value[MAX_STRING_TOK_VALUE_SIZE];
  XmlAttribute *xml_attribute;

  if (((Token *)((*cur_node)->data))->type == STRING_TOK)
  {
    strcpy(name, ((Token *)((*cur_node)->data))->value);
    *cur_node = (*cur_node)->next;

    if (((Token *)((*cur_node)->data))->type != EQUAL_TOK)
    {
      error_handle("invalid attributes");
    }

    *cur_node = (*cur_node)->next;

    if (((Token *)((*cur_node)->data))->type != QUOTED_STRING_TOK)
    {
      error_handle("invalid attributes");
    }

    strcpy(value, ((Token *)((*cur_node)->data))->value);
    *cur_node = (*cur_node)->next;
    xml_attribute = xml_attribute_factory();
    xml_attribute->name = strdup(name);
    xml_attribute->value = strdup(value);

    return xml_attribute;
  }

  return NULL;
}

XmlAttributeList *get_attributes(Node **cur_node, State is_meta)
{

  XmlAttributeList *xml_attributes = xml_attribute_list_factory();
  XmlAttribute *xml_attribute;

  while (1)
  {
    if (((Token *)((*cur_node)->data))->type == META_TAG_CLOSING_TOK)
    {
      if (is_meta == OK)
      {
        *cur_node = (*cur_node)->next;
        break;
      }
      error_handle("invalid attributes");
    }
    if (((Token *)((*cur_node)->data))->type == TAG_ENDING_TOK)
    {
      if (is_meta == NOT_OK)
      {
        *cur_node = (*cur_node)->next;
        break;
      }
      error_handle("invalid attributes");
    }

    xml_attribute = get_attribute(cur_node);

    if (xml_attribute == NULL)
    {
      error_handle("invalid attributes");
    }

    add_xml_attribute_list(xml_attributes, xml_attribute);
  }

  return xml_attributes;
}

void add_meta_line(Node *root, Node **cur_tok)
{
  XmlNode *xml_node = NULL;

  if (((Token *)((*cur_tok)->data))->type == META_TAG_OPENING_TOK)
  {
    *cur_tok = (*cur_tok)->next;

    if (((Token *)((*cur_tok)->data))->type != STRING_TOK)
    {
      error_handle("invalid meta tag");
    }
    if (strcmp(META_IDENTIFER, ((Token *)((*cur_tok)->data))->value))
    {
      error_handle("invalid meta tag");
    }

    xml_node = xml_node_factory();
    xml_node->name = strdup(META_IDENTIFER);
    *cur_tok = (*cur_tok)->next;
    xml_node->attributes = get_attributes(cur_tok, OK);
  }

  if (xml_node != NULL)
  {
    add_xml_node_list(((XmlNode *)root->data)->children, xml_node);
  }
}

void append_to_body_xml_node(XmlNode *xml_node, char *data)
{
  uint64_t size = strlen(data);
  if (xml_node->body != NULL)
  {
    size += 2 + strlen(xml_node->body);
  }

  char *temp = (char *)malloc(sizeof(char) * size);
  uint64_t temp_index = 0, data_index = 0;

  while (xml_node->body && xml_node->body[data_index])
  {
    temp[temp_index++] = xml_node->body[data_index++];
  }

  temp[temp_index++] = SPACE;
  data_index = 0;

  while (data[data_index])
  {
    temp[temp_index++] = data[data_index++];
  }

  temp[temp_index] = END;
  free(xml_node->body);
  xml_node->body = temp;
}

void handle_new_start_tag(Node **cur_par, Node **cur, Node **cur_tok)
{
  *cur_tok = (*cur_tok)->next;

  if (((Token *)((*cur_tok)->data))->type != STRING_TOK)
  {
    error_handle("invalid  tags");
  }

  XmlNode *new_xml_node = xml_node_factory();
  Node *new_node = node_factory();
  new_node->data = (void *)new_xml_node;

  new_xml_node->name = strdup(((Token *)((*cur_tok)->data))->value);
  *cur_tok = (*cur_tok)->next;
  new_xml_node->attributes = get_attributes(cur_tok, NOT_OK);

  if (*cur == NULL)
  {
    new_xml_node->parent = *cur_par;
    *cur = new_node;
    return;
  }

  new_xml_node->parent = *cur;
  *cur_par = *cur;
  *cur = new_node;
}

void handle_new_end_tag(Node **cur_par, Node **cur, Node **cur_tok)
{
  *cur_tok = (*cur_tok)->next;

  if (((Token *)((*cur_tok)->data))->type != STRING_TOK)
  {
    error_handle("invalid  tags");
  }
  if (*cur)
  {
    if (strcmp(((XmlNode *)(*cur)->data)->name, ((Token *)((*cur_tok)->data))->value))
    {
      error_handle("invalid  tags");
    }
  }
  else
  {
    if (is_root_xml_tree(*cur_par) == OK || strcmp(((XmlNode *)(*cur_par)->data)->name,
                                                   ((Token *)((*cur_tok)->data))->value))
    {
      error_handle("invalid  tags");
    }
    *cur = *cur_par;
    *cur_par = ((XmlNode *)(*cur_par)->data)->parent;
  }

  *cur_tok = (*cur_tok)->next;

  if (((Token *)((*cur_tok)->data))->type != TAG_ENDING_TOK)
  {
    error_handle("invalid  tags");
  }

  *cur_tok = (*cur_tok)->next;
  add_linked_list_tail(((XmlNode *)(*cur_par)->data)->children, *cur);
  *cur = NULL;
}

void handle_xml_node_body(Node *cur_par, Node *cur, Node **cur_tok)
{
  if (cur == NULL)
  {
    if (is_root_xml_tree(cur_par) == OK)
    {
      error_handle("invalid  body");
    }

    append_to_body_xml_node((XmlNode *)cur_par->data, ((Token *)((*cur_tok)->data))->value);
    *cur_tok = (*cur_tok)->next;
    return;
  }
  append_to_body_xml_node((XmlNode *)cur->data, ((Token *)((*cur_tok)->data))->value);
  *cur_tok = (*cur_tok)->next;
}

XmlTree *build_xml_tree(XmlDocument *xml)
{
  TokenList *token_list;
  XmlTree *xml_tree = xml_tree_factory();
  token_list = get_tokens(xml->data);

  Node *cur_tok = token_list->head;
  add_meta_line(xml_tree->root, &cur_tok);

  Node *cur_par = xml_tree->root;
  Node *cur = NULL;
  TokenType token_type = ((Token *)(cur_tok->data))->type;

  while (token_type != END_TOK)
  {
    if (token_type == STARTING_TAG_OPENING_TOK)
    {
      handle_new_start_tag(&cur_par, &cur, &cur_tok);
    }
    else if (token_type == ENDING_TAG_OPENING_TOK)
    {
      handle_new_end_tag(&cur_par, &cur, &cur_tok);
    }
    else if (token_type == STRING_TOK || token_type == QUOTED_STRING_TOK)
    {
      handle_xml_node_body(cur_par, cur, &cur_tok);
    }
    else
    {
      error_handle("invalid  tags");
    }
    token_type = ((Token *)(cur_tok->data))->type;
  }

  if (cur != NULL)
  {
    error_handle("invalid  tags");
  }

  free_token_list(&token_list);
  return xml_tree;
}

State check_port_node(Node *node)
{
  XmlNode *xml_node = (XmlNode *)node->data;
  if (is_leaf_xml_tree(node) == OK || xml_node->body)
  {
    return NOT_OK;
  }

  Node *child = xml_node->children->head;

  while (child)
  {
    xml_node = (XmlNode *)child->data;
    if (strcmp(xml_node->name, TO_IDENTIFIER) || xml_node->children->size || !xml_node->body)
    {
      return NOT_OK;
    }

    if (check_is_int(xml_node->body) == NOT_OK)
    {
      return NOT_OK;
    }

    child = child->next;
  }
  return OK;
}

State check_node_node(Node *node)
{
  XmlNode *xml_node = (XmlNode *)node->data;

  if (strcmp(xml_node->name, NODE_IDENTIFIER) || xml_node->children->size < 3 || xml_node->body)
  {
    return NOT_OK;
  }

  State has_name = NOT_OK, has_id = NOT_OK, has_port = NOT_OK;
  Node *child = xml_node->children->head;

  while (child)
  {
    xml_node = (XmlNode *)child->data;

    if (!strcmp(xml_node->name, NAME_IDENTIFIER) && has_name == NOT_OK)
    {
      has_name = OK;

      if (is_leaf_xml_tree(child) == NOT_OK || !xml_node->body)
      {
        return NOT_OK;
      }
    }
    else if (!strcmp(xml_node->name, ID_IDENTIFIER) && has_id == NOT_OK)
    {
      has_id = OK;

      if (is_leaf_xml_tree(child) == NOT_OK || !xml_node->body || check_is_int(xml_node->body) == NOT_OK)
      {
        return NOT_OK;
      }
    }
    else if (!strcmp(xml_node->name, PORT_IDENTIFIER) && has_port == NOT_OK)
    {
      has_port = OK;
      if (check_port_node(child) == NOT_OK)
      {
        return NOT_OK;
      }
    }
    else
    {
      return NOT_OK;
    }

    child = child->next;
  }

  if (has_name == NOT_OK || has_id == NOT_OK || has_port == NOT_OK)
  {
    return NOT_OK;
  }
  return OK;
}

State check_system_node(Node *system)
{
  XmlNode *xml_node = (XmlNode *)system->data;

  if (xml_node->body || strcmp(xml_node->name, SYSTEM_IDENTIFIER))
  {
    return NOT_OK;
  }

  if (xml_node->children->size < 2)
  {
    return NOT_OK;
  }
  Node *child = xml_node->children->head;

  while (child)
  {
    xml_node = (XmlNode *)child->data;

    if (check_node_node(child) == NOT_OK)
    {
      return NOT_OK;
    }
    child = child->next;
  }

  return OK;
}

State check_root_node(Node *root)
{
  if (!root)
  {

    return NOT_OK;
  }

  XmlNode *xml_node = (XmlNode *)root->data;
  Node *child = xml_node->children->head;

  if (xml_node->children->size == 1)
  {
    xml_node = (XmlNode *)child->data;
    return check_system_node(child);
  }
  else if (xml_node->children->size == 2)
  {
    xml_node = (XmlNode *)child->data;
    if (strcmp(xml_node->name, META_IDENTIFER))
    {
      return NOT_OK;
    }
    child = child->next;
    return check_system_node(child);
  }
  else
  {
    return NOT_OK;
  }
}

State validate_tree(XmlTree *xml_tree)
{
  if (!xml_tree)
  {
    return NOT_OK;
  }
  return check_root_node(xml_tree->root);
}

uint64_t get_node_id(XmlNode *node_node)
{
  Node *cur = node_node->children->head;
  XmlNode *xml_node;
  while (cur)
  {
    xml_node = (XmlNode *)cur->data;
    if (!strcmp(xml_node->name, ID_IDENTIFIER))
    {
      break;
    }
    cur = cur->next;
  }
  return strtol(xml_node->body, &xml_node->body, 10);
}

XmlNode *get_node_port(XmlNode *node_node)
{
  Node *cur = node_node->children->head;
  XmlNode *xml_node;
  while (cur)
  {
    xml_node = (XmlNode *)cur->data;
    if (!strcmp(xml_node->name, PORT_IDENTIFIER))
    {
      return xml_node;
    }
    cur = cur->next;
  }
}

State check_node_if_seen(Graph *graph, uint64_t current_id)
{
  if (graph->data[current_id][0] == -1)
  {
    for (size_t i = 0; i < graph->size; i++)
    {
      graph->data[current_id][i] = 0;
    }
    return OK;
  }

  return NOT_OK;
}

void add_node_route(Graph *graph, XmlNode *node_node)
{
  int current_id = get_node_id(node_node);
  int current_val;

  if (current_id < 0 || current_id >= graph->size || check_node_if_seen(graph, current_id) == NOT_OK)
  {
    error_handle("invalid setting values");
  }

  XmlNode *xml_node = get_node_port(node_node);
  Node *cur = xml_node->children->head;
  int flag = 0;

  while (cur)
  {
    xml_node = (XmlNode *)cur->data;
    current_val = strtol(xml_node->body, &xml_node->body, 10);

    if (current_val < -1 || current_val >= (int)graph->size || current_id == current_val)
    {
      error_handle("invalid setting values");
    }

    if (current_val == -1 && flag == 0)
    {
      flag = -1;
      for (size_t i = 0; i < graph->size; i++)
      {
        graph->data[current_id][i] = 0;
      }
    }
    else if (current_val != -1 && flag != -1)
    {
      flag = 1;
      graph->data[current_id][current_val] = 1;
    }
    else
    {
      error_handle("invalid setting values");
    }
    cur = cur->next;
  }
}

void add_setting_to_graph(Graph *graph, XmlNode *system_node)
{
  Node *cur = system_node->children->head;
  XmlNode *xml_node;
  while (cur)
  {
    xml_node = (XmlNode *)cur->data;
    add_node_route(graph, xml_node);
    cur = cur->next;
  }
}

Graph *get_setting_from_xml_tree(XmlTree *xml_tree)
{
  Graph *graph = graph_factory();

  Node *cur = xml_tree->root;
  XmlNode *xml_node = (XmlNode *)cur->data;
  cur = xml_node->children->head;
  xml_node = (XmlNode *)cur->data;

  if (!strcmp(xml_node->name, META_IDENTIFER))
  {
    cur = cur->next;
    xml_node = (XmlNode *)cur->data;
  }

  build_base_graph(graph, xml_node->children->size);
  add_setting_to_graph(graph, xml_node);

  return graph;
}