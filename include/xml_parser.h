#pragma once

#include "util.h"
#include "linked_list.h"

typedef struct XmlDocument
{
    char *source;
    char *data;
    uint64_t size;
} XmlDocument;

XmlDocument *xml_factory();
void free_xml(XmlDocument **);
State load_xml(XmlDocument *, char *);

typedef struct XmlAttribute
{
    char *name;
    char *value;
} XmlAttribute;

XmlAttribute *xml_attribute_factory();
void free_xml_attribute(XmlAttribute **);

typedef struct XmlAttributeList
{
    LinkedList *xml_attributes;
} XmlAttributeList;

XmlAttributeList *xml_attribute_list_factory();
void add_xml_attribute_list(XmlAttributeList *, XmlAttribute *);
void free_xml_attribute_list(XmlAttributeList **);

typedef struct XmlNode XmlNode;

typedef struct XmlNodeList
{
    LinkedList *xml_nodes;
} XmlNodeList;

XmlNodeList *xml_node_list_list_factory();
void add_xml_node_list(XmlNodeList *, XmlNode *);
void free_xml_node_list(XmlNodeList **);

typedef struct XmlNode
{
    char *name;
    char *body;
    XmlAttributeList *attributes;
    XmlNodeList *children;
} XmlNode;

XmlNode *xml_node_factory();
void free_xml_node(XmlNode **);

typedef struct XmlTree
{
    XmlNode *root;
} XmlTree;