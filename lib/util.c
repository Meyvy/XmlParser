#include "../include/util.h"

void error_handle(char *msg)
{
    fprintf(stderr, "the program exited with the error message:\n%s", msg);
    exit(0);
}