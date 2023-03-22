#include "../include/xml_tokenizer.h"

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

State check_symbol_is_valid(char symbol)
{

  char allowed_symbols[] = {EQUAL, LEFT_ANGLE, RIGHT_ANGLE, SLASH,
                            STRING_SYM, EXCLAMATION, SPACE, QUESTION,
                            LINE_BREAK, END};

  if (isalnum(symbol))
  {
    return OK;
  }

  uint64_t size = sizeof(allowed_symbols) / sizeof(allowed_symbols[0]);
  for (uint64_t i = 0; i < size; i++)
  {
    if (allowed_symbols[i] == symbol)
    {
      return OK;
    }
  }

  return NOT_OK;
}

void skip_white_space(char *data, uint64_t *cur)
{
  while (data[*cur] == SPACE || data[*cur] == LINE_BREAK)
  {
    (*cur)++;
  }
}

State skip_comment(char *data, uint64_t *cur)
{
  if (data[*cur] == LEFT_ANGLE &&
      (data[*cur + 1] && data[*cur + 1] == EXCLAMATION) &&
      (data[*cur + 2] && data[*cur + 2] == HYPHEN) &&
      (data[*cur + 3] && data[*cur + 3] == HYPHEN))
  {
    (*cur) = (*cur) + 4;
    while (data[*cur])
    {
      if (data[*cur] == HYPHEN &&
          (data[*cur + 1] && data[*cur + 1] == HYPHEN) &&
          (data[*cur + 2] && data[*cur + 2] == RIGHT_ANGLE))
      {
        *cur = *cur + 3;
        return OK;
      }
      (*cur)++;
    }
  }

  return NOT_OK;
}

State move_to_next_char(char *data, uint64_t *cur)
{

  while (1)
  {
    if (data[*cur] == SPACE || data[*cur] == LINE_BREAK)
    {
      skip_white_space(data, cur);
    }
    if (skip_comment(data, cur) == NOT_OK)
    {
      break;
    }
  }

  return check_symbol_is_valid(data[*cur]);
}

TokenType get_special_tokens(char *data, uint64_t *cur, char *value)
{
  if (data[*cur] == LEFT_ANGLE)
  {
    if (data[*cur + 1] == SLASH)
    {
      strcpy(value, ENDING_TAG_OPENING_TOK_VAL);
      *cur = *cur + 2;
      return ENDING_TAG_OPENING_TOK;
    }
    if (data[*cur + 1] == QUESTION)
    {
      strcpy(value, META_TAG_OPENING_TOK_VAL);
      *cur = *cur + 2;
      return META_TAG_OPENING_TOK;
    }
    (*cur)++;
    strcpy(value, STARTING_TAG_OPENING_TOK_VAL);
    return STARTING_TAG_OPENING_TOK;
  }
  else if (data[*cur] == RIGHT_ANGLE)
  {
    (*cur)++;
    strcpy(value, TAG_ENDING_TOK_VAL);
    return TAG_ENDING_TOK;
  }
  else if (data[*cur] == EQUAL)
  {
    (*cur)++;
    strcpy(value, EQUAL_TOK_VAL);
    return EQUAL_TOK;
  }
  else if (data[*cur] == QUESTION)
  {
    if (data[*cur + 1] == RIGHT_ANGLE)
    {
      *cur = *cur + 2;
      value = strcpy(value, META_TAG_CLOSING_TOK_VAL);
      return META_TAG_CLOSING_TOK;
    }
    strcpy(value, INVALID_TOK_VAL);
    return INVALID_TOK;
  }
  else if (data[*cur] == END)
  {
    strcpy(value, END_TOK_VAL);
    return END_TOK;
  }
  else
  {
    strcpy(value, NO_TOK_VAL);
    return NO_TOK;
  }
}

TokenType get_string_tok(char *data, uint64_t *cur, char *value)
{
  char buff[MAX_STRING_TOK_VALUE_SIZE];
  uint64_t index = 0;
  if (data[*cur] == STRING_SYM)
  {
    (*cur)++;
    while (data[*cur] != STRING_SYM)
    {
      if (data[*cur] == LINE_BREAK || data[*cur] == END ||
          index > MAX_STRING_TOK_VALUE_SIZE - 2)
      {
        strcpy(value, INVALID_TOK_VAL);
        return INVALID_TOK;
      }

      buff[index++] = data[(*cur)++];
    }
    (*cur)++;
    buff[index] = END;
    strcpy(value, buff);
    return QUOTED_STRING;
  }
  else
  {
    while (data[*cur] != END && data[*cur] != SPACE &&
           data[*cur] != LINE_BREAK && isalnum(data[*cur]))
    {
      if (index > MAX_STRING_TOK_VALUE_SIZE - 2)
      {
        strcpy(value, INVALID_TOK_VAL);
        return INVALID_TOK;
      }

      buff[index++] = data[(*cur)++];
    }
  }

  buff[index] = END;
  strcpy(value, buff);
  return STRING_TOK;
}

void get_token_vals(char *data, uint64_t *cur, TokenType *token_type,
                    char *value)
{
  if (move_to_next_char(data, cur) == NOT_OK)
  {
    *token_type = INVALID_TOK;
    strcpy(value, INVALID_TOK_VAL);
    return;
  }

  *token_type = get_special_tokens(data, cur, value);

  if (*token_type == INVALID_TOK)
  {
    return NOT_OK;
  }

  if (*token_type != NO_TOK)
  {
    return OK;
  }

  *token_type = get_string_tok(data, cur, value);

  if (*token_type == INVALID_TOK)
  {
    return NOT_OK;
  }

  return OK;
}

Token *next_token(char *data, uint64_t *cur)
{
  char buff[MAX_STRING_TOK_VALUE_SIZE];
  Token *token = token_factory();
  TokenType token_type;

  get_token_vals(data, cur, &token_type, buff);
  token->type = token_type;
  token->value = strdup(buff);
  return token;
}

TokenList *token_list_factory()
{
  TokenList *token_list = (TokenList *)malloc(sizeof(TokenList));
  token_list->tokens = linked_list_factory();
  return token_list;
}

void add_token_list(TokenList *token_list, Token *token)
{
  Node *node = node_factory();
  node->data = (void *)token;
  add_linked_list_tail(token_list->tokens, node);
}

void free_token_list(TokenList **token_list)
{
  if (*token_list == NULL)
  {
    return;
  }

  if ((*token_list)->tokens != NULL)
  {
    free_linked_list(&(*token_list)->tokens, free_token);
  }

  free(*token_list);
  *token_list = NULL;
}

TokenList *get_tokens(char *data)
{
  uint64_t cur = 0;
  Token *token;
  TokenList *token_list = token_list_factory();

  while (1)
  {
    token = next_token(data, &cur);
    if (token->type == END_TOK)
    {
      break;
    }
    if (token->type == INVALID_TOK)
    {
      error_handle("invalid token in the input");
    }

    add_token_list(token_list, token);
  }

  return token_list;
}
