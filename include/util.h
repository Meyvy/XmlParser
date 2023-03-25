#pragma once

// utility stuff and neccessary stdandard headers are declared here
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>

// all our global defines

#define EQUAL '='
#define EXCLAMATION '!'
#define LEFT_ANGLE '<'
#define RIGHT_ANGLE '>'
#define SLASH '/'
#define HYPHEN '-'
#define SPACE ' '
#define QUESTION '?'
#define LINE_BREAK '\n'
#define STRING_SYM '"'
#define END '\0'
#define MAX_STRING_TOK_VALUE_SIZE 64
#define META_TAG_OPENING_TOK_VAL "<?"
#define META_TAG_CLOSING_TOK_VAL "?>"
#define STARTING_TAG_OPENING_TOK_VAL "<"
#define ENDING_TAG_OPENING_TOK_VAL "</"
#define TAG_ENDING_TOK_VAL ">"
#define EQUAL_TOK_VAL "="
#define INVALID_TOK_VAL "$__INVALID_TOK__$"
#define END_TOK_VAL "$__END_OF_FILE__$"
#define NO_TOK_VAL "$__NOVAL__$"
#define META_IDENTIFER "xml"
#define SYSTEM_IDENTIFIER "system"
#define NODE_IDENTIFIER "node"
#define NAME_IDENTIFIER "name"
#define ID_IDENTIFIER "id"
#define TO_IDENTIFIER "to"
#define PORT_IDENTIFIER "port"
#define SETTING_FILE "test.xml"

// our token types
typedef enum TokenType
{
    META_TAG_OPENING_TOK,
    META_TAG_CLOSING_TOK,
    STARTING_TAG_OPENING_TOK,
    ENDING_TAG_OPENING_TOK,
    TAG_ENDING_TOK,
    QUOTED_STRING_TOK,
    STRING_TOK,
    IDENTIFIER_TOK,
    EQUAL_TOK,
    INVALID_TOK,
    NO_TOK,
    END_TOK
} TokenType;

// a state for the result of an operations (like true and false)
typedef enum State
{
    OK,
    NOT_OK
} State;

// prints the error then exits the program
void error_handle(char *msg);

// checks whether a string is an int
State check_is_int(char *data);

typedef struct Node Node; // forward declaration for node

// a defualt callback for comparison in a linked list
// it compares the memory of the two poitners
State check_node_default(Node *, void *);