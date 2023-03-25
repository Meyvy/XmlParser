#include "../include/util.h"

void error_handle(char *msg)
{
    fprintf(stderr, "the program exited with the error message:\n%s.\n", msg);
    exit(0);
}

State check_is_int(char *data)
{
    long num = strtol(data, &data, 10);
    if (*data == '\0')
    {
        return OK;
    }
    return NOT_OK;
}

State check_node_default(Node *node, void *data)
{
    data = (Node *)data;

    if (data == node)
    {
        return OK;
    }

    return NOT_OK;
}