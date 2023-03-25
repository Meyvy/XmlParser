#include "../include/app.h"

void print(Graph *graph)
{
    for (size_t i = 0; i < graph->size; i++)
    {
        for (size_t j = 0; j < graph->size; j++)
        {
            printf("%d ", graph->data[i][j]);
        }
        printf("\n");
    }
}

void app()
{
    XmlDocument *xml = xml_factory();

    if (load_xml(xml, SETTING_FILE))
    {
        error_handle("the xml setting could not be loaded ");
    }

    XmlTree *xml_tree = build_xml_tree(xml);

    if (validate_tree(xml_tree) == NOT_OK)
    {
        error_handle("invalid xml setting");
    }
    Graph *graph = get_setting_from_xml_tree(xml_tree);
    NodePath *node_path = bfs_graph(graph, 1, 0);
}