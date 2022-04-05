#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

typedef struct point
{
    int x, y;
} Point;

typedef struct box
{
    Point;
    Point dir;
} Box;


int main(void) {
    Point p = {1, 2};
    Box b = {p, {3, 4}};
    printf("%d %d %d %d\n", b.x, b.y, b.dir.x, b.dir.y);
    /* Point q = p;
    q.x = 3;
    printf("%d %d %d %d \n", p.x, p.y, q.x, q.y);
    q = q + p;
    printf("%d %d %d %d \n", p.x, p.y, q.x, q.y); */
    return 0;
}