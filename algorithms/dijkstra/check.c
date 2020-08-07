// check.c
// Driver program for Dijkstra's graph search algorithm.

// attribute gnu_printf
#pragma GCC diagnostic ignored "-Wignored-attributes"

#include <check.h>
#include <stdlib.h>

#include "graph.h"
#include "dijkstra.h"

// ----------------------------------------------------------------------------
// Test Cases

START_TEST(test_dijkstra_0)
{
    graph_t* g = graph_new();

    vertex_id_t v0 = graph_add_vertex(g);
    vertex_id_t v1 = graph_add_vertex(g);
    vertex_id_t v2 = graph_add_vertex(g);
    vertex_id_t v3 = graph_add_vertex(g);
    vertex_id_t v4 = graph_add_vertex(g);

    graph_add_edge(g, v0, v1, 1);
    graph_add_edge(g, v1, v2, 1);
    graph_add_edge(g, v2, v4, 1);
    graph_add_edge(g, v0, v3, 1);
    graph_add_edge(g, v3, v4, 3);

    path_t* path = dijkstra(g, v0, v4);
    ck_assert(path != NULL);

    ck_assert(path_length(path) == 3);
    ck_assert(path_weight(path) == 3);

    path_delete(path);
    graph_delete(g);
}
END_TEST

// ----------------------------------------------------------------------------
// Infrastructure
 
Suite* dijkstra_suite(void)
{
    Suite* s = suite_create("dijkstra");
    TCase* tc_core = tcase_create("dijkstra-core");
    
    tcase_add_test(tc_core, test_dijkstra_0);
    
    suite_add_tcase(s, tc_core);
    
    return s;
}

int main(void)
{
    Suite* suite = dijkstra_suite();
    SRunner* runner = srunner_create(suite);

    srunner_run_all(runner, CK_NORMAL);    
    srunner_free(runner);
    
    return EXIT_SUCCESS;
}