// check.c
// Driver program for graph data structure tests.

// attribute gnu_printf
#pragma GCC diagnostic ignored "-Wignored-attributes"

#include <check.h>
#include <stdlib.h>

#include "graph.h"

// ----------------------------------------------------------------------------
// Test Cases

START_TEST(test_graph_new)
{
    graph_t* g = graph_new();
    ck_assert_msg(g != NULL, "graph_new() failed");

    ck_assert_msg(graph_vertex_count(g) == 0, "graph_vertex_count() returned unexpected value");
    ck_assert_msg(graph_edge_count(g) == 0, "graph_edge_count() returned unexpected value");

    graph_delete(g);
}
END_TEST

START_TEST(test_graph_build_0)
{
    graph_t* g = graph_new();
    ck_assert_msg(g != NULL, "graph_new() failed");

    // construct a simple graph

    const vertex_id_t v0 = graph_add_vertex(g);
    const vertex_id_t v1 = graph_add_vertex(g);
    const vertex_id_t v2 = graph_add_vertex(g);
    const vertex_id_t v3 = graph_add_vertex(g);
    const vertex_id_t v4 = graph_add_vertex(g);

    ck_assert(v0 != NULL_VERTEX_DEGREE);
    ck_assert(v1 != NULL_VERTEX_DEGREE);
    ck_assert(v2 != NULL_VERTEX_DEGREE);
    ck_assert(v3 != NULL_VERTEX_DEGREE);
    ck_assert(v4 != NULL_VERTEX_DEGREE);

    ck_assert(graph_add_edge(g, v0, v1, 1));
    ck_assert(graph_add_edge(g, v0, v2, 2));
    ck_assert(graph_add_edge(g, v0, v3, 3));
    ck_assert(graph_add_edge(g, v1, v2, 4));
    ck_assert(graph_add_edge(g, v2, v3, 5));
    ck_assert(graph_add_edge(g, v1, v4, 6));
    ck_assert(graph_add_edge(g, v3, v4, 7));

    // query expected properties of the graph

    ck_assert(graph_vertex_count(g) == 5);
    ck_assert(graph_edge_count(g) == 7);

    ck_assert(graph_has_edge(g, v0, v1));
    ck_assert(graph_has_edge(g, v0, v2));
    ck_assert(graph_has_edge(g, v0, v3));
    ck_assert(graph_has_edge(g, v1, v2));
    ck_assert(graph_has_edge(g, v2, v3));
    ck_assert(graph_has_edge(g, v1, v4));
    ck_assert(graph_has_edge(g, v3, v4));

    ck_assert(graph_get_edge_weight(g, v0, v1) == 1);
    ck_assert(graph_get_edge_weight(g, v0, v2) == 2);
    ck_assert(graph_get_edge_weight(g, v0, v3) == 3);
    ck_assert(graph_get_edge_weight(g, v1, v2) == 4);
    ck_assert(graph_get_edge_weight(g, v2, v3) == 5);
    ck_assert(graph_get_edge_weight(g, v1, v4) == 6);
    ck_assert(graph_get_edge_weight(g, v3, v4) == 7);

    ck_assert(graph_get_vertex_degree(g, v0) == 3);
    ck_assert(graph_get_vertex_degree(g, v1) == 3);
    ck_assert(graph_get_vertex_degree(g, v2) == 3);
    ck_assert(graph_get_vertex_degree(g, v3) == 3);
    ck_assert(graph_get_vertex_degree(g, v4) == 2);

    graph_delete(g);
}
END_TEST

START_TEST(test_graph_build_1)
{
    graph_t* g = graph_new();
    ck_assert_msg(g != NULL, "graph_new() failed");

    // construct a simple graph

    const vertex_id_t v0 = graph_add_vertex(g);
    const vertex_id_t v1 = graph_add_vertex(g);
    const vertex_id_t v2 = graph_add_vertex(g);

    ck_assert(v0 != NULL_VERTEX_DEGREE);
    ck_assert(v1 != NULL_VERTEX_DEGREE);
    ck_assert(v2 != NULL_VERTEX_DEGREE);

    ck_assert(graph_add_edge(g, v0, v1, 1));
    ck_assert(graph_add_edge(g, v0, v2, 2));
    ck_assert(graph_add_edge(g, v1, v2, 3));

    // query initial expected properties

    ck_assert(graph_vertex_count(g) == 3);
    ck_assert(graph_edge_count(g) == 3);

    ck_assert(graph_has_edge(g, v0, v1));
    ck_assert(graph_has_edge(g, v0, v2));
    ck_assert(graph_has_edge(g, v1, v2));

    ck_assert(graph_get_edge_weight(g, v0, v1) == 1);
    ck_assert(graph_get_edge_weight(g, v0, v2) == 2);
    ck_assert(graph_get_edge_weight(g, v1, v2) == 3);

    ck_assert(graph_get_vertex_degree(g, v0) == 2);
    ck_assert(graph_get_vertex_degree(g, v1) == 2);
    ck_assert(graph_get_vertex_degree(g, v2) == 2);

    // modify the graph

    ck_assert(graph_remove_vertex(g, v2));

    // re-query the expected graph properties

    ck_assert(graph_vertex_count(g) == 2);
    ck_assert(graph_edge_count(g) == 1);    

    ck_assert(graph_has_edge(g, v0, v1));
    ck_assert(!graph_has_edge(g, v0, v2));
    ck_assert(!graph_has_edge(g, v1, v2));

    ck_assert(graph_get_edge_weight(g, v0, v1) == 1);
    ck_assert(graph_get_edge_weight(g, v0, v2) == NULL_EDGE_WEIGHT);
    ck_assert(graph_get_edge_weight(g, v1, v2) == NULL_EDGE_WEIGHT);

    ck_assert(graph_get_vertex_degree(g, v0) == 1);
    ck_assert(graph_get_vertex_degree(g, v1) == 1);
    ck_assert(graph_get_vertex_degree(g, v2) == NULL_VERTEX_DEGREE);

    graph_delete(g);
}
END_TEST

// ----------------------------------------------------------------------------
// Infrastructure
 
Suite* graph_suite(void)
{
    Suite* s = suite_create("graph");
    TCase* tc_core = tcase_create("graph-core");
    
    tcase_add_test(tc_core, test_graph_new);
    tcase_add_test(tc_core, test_graph_build_0);
    tcase_add_test(tc_core, test_graph_build_1);
    
    suite_add_tcase(s, tc_core);
    
    return s;
}

int main(void)
{
    Suite* suite = graph_suite();
    SRunner* runner = srunner_create(suite);

    srunner_run_all(runner, CK_NORMAL);    
    srunner_free(runner);
    
    return EXIT_SUCCESS;
}