// check.c
// Driver program for breadth-first graph search tests.

// attribute gnu_printf
#pragma GCC diagnostic ignored "-Wignored-attributes"

#include <check.h>
#include <stdlib.h>

#include "bfs.h"
#include "graph.h"
#include "spanning_tree.h"

// ----------------------------------------------------------------------------
// Test Cases

START_TEST(test_bfs_0)
{
    graph_t* g = graph_new();

    vertex_id_t v0 = graph_add_vertex(g);
    vertex_id_t v1 = graph_add_vertex(g);
    vertex_id_t v2 = graph_add_vertex(g);
    vertex_id_t v3 = graph_add_vertex(g);
    vertex_id_t v4 = graph_add_vertex(g);
    vertex_id_t v5 = graph_add_vertex(g);

    graph_add_edge(g, v0, v1, 1);
    graph_add_edge(g, v1, v2, 1);
    graph_add_edge(g, v2, v3, 1);
    graph_add_edge(g, v0, v4, 1);
    graph_add_edge(g, v4, v5, 1);

    spanning_tree_t* tree = breadth_first_search(g, v0);
    
    ck_assert(tree != NULL);
    ck_assert(spanning_tree_count(tree) == 6);

    spanning_tree_delete(tree);
    graph_delete(g);
}
END_TEST

// ----------------------------------------------------------------------------
// Infrastructure
 
Suite* bfs_suite(void)
{
    Suite* s = suite_create("bfs");
    TCase* tc_core = tcase_create("bfs-core");
    
    tcase_add_test(tc_core, test_bfs_0);
    
    suite_add_tcase(s, tc_core);
    
    return s;
}

int main(void)
{
    Suite* suite = bfs_suite();
    SRunner* runner = srunner_create(suite);

    srunner_run_all(runner, CK_NORMAL);    
    srunner_free(runner);
    
    return EXIT_SUCCESS;
}