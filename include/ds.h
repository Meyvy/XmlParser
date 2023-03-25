#pragma once

#include "util.h"

//  all the data structures are declared here

// node structure is a basis for all the other nodes
// he other node structures save their payload in data
typedef struct Node
{
    struct Node *next;
    struct Node *prev;
    void *data;
} Node;

// Node methods
Node *node_factory();
void free_node(Node **, void (*)(void *));

// LinkedList is a basis for all the other lists
// the other list structures have to maintain their own nodes
typedef struct LinkedList
{
    Node *head;
    Node *tail;
    uint64_t size;
} LinkedList;

// LinkedList methods
LinkedList *linked_list_factory();
void add_to_empty(LinkedList *, Node *);
void add_linked_list_head(LinkedList *, Node *);
void add_linked_list_tail(LinkedList *, Node *);
State add_linked_list_before(LinkedList *, Node *, Node *);
State add_linked_list_after(LinkedList *, Node *, Node *);
Node *remove_linked_list_last(LinkedList *);
Node *remove_linked_list_head(LinkedList *);
Node *remove_linked_list_tail(LinkedList *);
Node *remove_linked_list_cur(LinkedList *, Node *);
Node *search_linked_list(LinkedList *, State (*)(Node *, void *), void *);
void free_linked_list(LinkedList **, void (*)(void *));

// a data structure for token data
typedef struct Token
{
    TokenType type;
    char *value;
} Token;

// Token methods

Token *token_factory();
void free_token(Token *);

// use typedef to create a dummy struct for a list of tokens
typedef struct LinkedList TokenList;

// TokenList methods
TokenList *token_list_factory();
void add_token_list(TokenList *, Token *);
void free_token_list(TokenList **);

// a data structure for managing xml documents
typedef struct XmlDocument
{
    char *source;
    char *data;
    uint64_t size;
} XmlDocument;

// XmlDocument methods
XmlDocument *xml_factory();
State load_xml(XmlDocument *, char *);
void free_xml(XmlDocument **);

// use typedef to create a dummy struct for a list of xml attributes
typedef LinkedList XmlAttributeList; // forward declaration
// use typedef to create a dumm struct for a list of xml nodes
typedef LinkedList XmlNodeList; // forward declaration

// a data structure for managing xml node data
typedef struct XmlNode
{
    char *name;
    char *body;
    Node *parent;
    XmlAttributeList *attributes;
    XmlNodeList *children;
} XmlNode;

// XmlNode methods
XmlNode *xml_node_factory();
void free_xml_node(XmlNode **);

// a data structure for managing attributes of xml nodes
typedef struct XmlAttribute
{
    char *name;
    char *value;
} XmlAttribute;

// XmlAttribute methods
XmlAttribute *xml_attribute_factory();
void free_xml_attribute(XmlAttribute **);

// XmlAttributeList
XmlAttributeList *xml_attribute_list_factory();
void add_xml_attribute_list(XmlAttributeList *, XmlAttribute *);
void free_xml_attribute_list(XmlAttributeList **);

// XmlNodeList methods

XmlNodeList *xml_node_list_factory();
void add_xml_node_list(XmlNodeList *, XmlNode *);
void free_xml_node_list(XmlNodeList **);

// a tree like structure to store the xml nodes
typedef struct XmlTree
{
    Node *root;
} XmlTree;

// XmlTree methods

XmlTree *xml_tree_factory();
State is_root_xml_tree(Node *);
State is_leaf_xml_tree(Node *);
void free_xml_tree(XmlTree **);

// a graph data structure for routing between nodes
typedef struct Graph
{
    uint64_t size;
    int **data;
} Graph;

Graph *graph_factory();
void free_graph(Graph **);
void build_base_graph(Graph *, uint64_t);

typedef struct NodePath
{

    uint64_t id;
    LinkedList *path;
} NodePath;

NodePath *node_path_factory();
void free_node_path(NodePath **);

NodePath *bfs_graph(Graph *, uint64_t, uint64_t);