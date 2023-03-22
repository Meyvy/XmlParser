#pragma once

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>

typedef enum State
{
    OK,
    NOT_OK
} State;

void error_handle(char *msg);