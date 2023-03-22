#pragma once

#include "linked_list.h"
#include "util.h"

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

typedef enum TokenType
{
  META_TAG_OPENING_TOK,
  META_TAG_CLOSING_TOK,
  STARTING_TAG_OPENING_TOK,
  ENDING_TAG_OPENING_TOK,
  TAG_ENDING_TOK,
  QUOTED_STRING,
  STRING_TOK,
  IDENTIFIER_TOK,
  EQUAL_TOK,
  INVALID_TOK,
  NO_TOK,
  END_TOK
} TokenType;

typedef struct Token
{
  TokenType type;
  char *value;
} Token;

Token *token_factory();
void free_token(Token *);

State check_symbol_is_valid(char);
void skip_white_space(char *, uint64_t *);
State skip_comment(char *, uint64_t *);
State move_to_next_char(char *, uint64_t *);
TokenType get_special_tokens(char *, uint64_t *, char *);
TokenType get_string_tok(char *, uint64_t *, char *);
void get_token_vals(char *, uint64_t *, TokenType *, char *);
Token *next_token(char *, uint64_t *);

typedef struct TokenList
{
  LinkedList *tokens;
} TokenList;

TokenList *token_list_factory();
void add_token_list(TokenList *, Token *);
void free_token_list(TokenList **);

TokenList *get_tokens(char *);