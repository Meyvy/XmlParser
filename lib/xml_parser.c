#include "../include/xml_parser.h"

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

XmlAttributeList *xml_attribute_list_factory()
{
  XmlAttributeList *xml_attribute_list = (XmlAttributeList *)malloc(sizeof(XmlAttributeList));
  xml_attribute_list->xml_attributes = linked_list_factory();
  return xml_attribute_list;
}

void add_xml_attribute_list(XmlAttributeList *xml_attribute_list, XmlAttribute *xml_attribute)
{
  Node *node = node_factory();
  node->data = (void *)xml_attribute;
  add_linked_list_tail(xml_attribute_list->xml_attributes, node);
}

void free_xml_attribute_list(XmlAttributeList **xml_attribute_list)
{
  if (*xml_attribute_list == NULL)
  {
    return;
  }

  if ((*xml_attribute_list)->xml_attributes != NULL)
  {
    free_linked_list(&(*xml_attribute_list)->xml_attributes, free_xml_attribute);
  }

  free(*xml_attribute_list);
  *xml_attribute_list = NULL;
}

XmlNodeList *xml_node_list_list_factory()
{
  XmlNodeList *xml_node_list = (XmlNodeList *)malloc(sizeof(XmlNodeList));
  xml_node_list->xml_nodes = linked_list_factory();
  return xml_node_list;
}

void add_xml_node_list(XmlNodeList *xml_node_list, XmlNode *xml_node)
{
  Node *node = node_factory();
  node->data = (void *)xml_node;
  add_linked_list_tail(xml_node_list->xml_nodes, node);
}

void free_xml_node_list(XmlNodeList **xml_node_list)
{
  if (*xml_node_list == NULL)
  {
    return;
  }

  if ((*xml_node_list)->xml_nodes != NULL)
  {
    free_linked_list(&(*xml_node_list)->xml_nodes, free_xml_node);
  }

  free(*xml_node_list);
  *xml_node_list = NULL;
}

XmlNode *xml_node_factory()
{
  XmlNode *xml_node = (XmlNode *)malloc(sizeof(XmlNode));
  xml_node->name = NULL;
  xml_node->body = NULL;
  xml_node->attributes = xml_attribute_list_factory();
  xml_node->children = xml_node_list_list_factory();
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
  xml_node = NULL;
}