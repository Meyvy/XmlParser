#pragma once

// stuff needed for creating and validating the xml tree are declared here

#include "util.h"
#include "ds.h"
#include "xml_tokenizer.h"

// functions for creating the xml tree from the tokens

XmlAttribute *get_attribute(Node **);
XmlAttributeList *get_attributes(Node **, State);
void add_meta_line(Node *, Node **);
void append_to_body_xml_node(XmlNode *, char *);
void handle_new_start_tag(Node **, Node **, Node **);
void handle_new_end_tag(Node **, Node **, Node **);
void handle_xml_node_body(Node *, Node *, Node **);
XmlTree *build_xml_tree(XmlDocument *);

// functions for validating the parsed tree of the setting

State check_port_node(Node *);
State check_node_node(Node *);
State check_system_node(Node *);
State check_root_node(Node *);
State validate_tree(XmlTree *);

// function for getting the setting graph from the xml tree

uint64_t get_node_id(XmlNode *);
XmlNode *get_node_port(XmlNode *);
State check_node_if_seen(Graph *, uint64_t);
void add_node_route(Graph *, XmlNode *);
void add_setting_to_graph(Graph *, XmlNode *);
Graph *get_setting_from_xml_tree(XmlTree *);