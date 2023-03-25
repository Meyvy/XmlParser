#pragma once

// stuff needed for lexical analysis are declared here

#include "ds.h"
#include "util.h"





// functions needed to read and tokenize the xml document

State check_symbol_is_valid(char);
void skip_white_space(char *, uint64_t *);
State skip_comment(char *, uint64_t *);
State move_to_next_char(char *, uint64_t *);
TokenType get_special_tokens(char *, uint64_t *, char *);
TokenType get_string_tok(char *, uint64_t *, char *);
void get_token_vals(char *, uint64_t *, TokenType *, char *);
Token *next_token(char *, uint64_t *);
TokenList *get_tokens(char *);