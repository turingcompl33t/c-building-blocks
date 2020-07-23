#include <stdio.h>
#include <stdlib.h>
#include "set.h"
#include <stdint.h>

typedef struct point{  //structure
    uint64_t x;
    uint64_t y;

} point_t;

static point_t* make_point(uint64_t x, uint64_t y)
{ //calls and allocates for an actual point p
    point_t* p = malloc(sizeof(point_t));
    p->x = x;
    p->y = y;

    return p;
}

static void delete_point(void* p)
{
    free(p);
}

static hash_t hash_point(void* p)
{
    point_t* as_point = (point_t*)p;
    return as_point->x;
}


//-------------------------------------------//
int main(void){
    set_t* s = set_new(delete_point, hash_point);

    point_t * p1 = make_point(30, 16);
    point_t * p2 = make_point(187, 43);
    point_t * p3 = make_point(17, 2);
    point_t * p4 = make_point(187, 43);
    point_t * p5 = make_point(19, 11);

    set_add(s,p1);
    set_add(s,p2);
    set_add(s,p3);
    set_add(s,p4);
    set_add(s,p5);

    set_remove(s,p1);
    free(p1);

    set_contains(s,p4);

    set_delete(s);
    return 0;
}
