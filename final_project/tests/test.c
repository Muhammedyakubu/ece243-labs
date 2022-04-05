#include <stdio.h>
#include <stdbool.h>
#include <./vector.h>

int main () {
    Vector a = new_vector();
    printf( "a.x = %d, a.y = %d\n", a.x, a.y );
    Vector b = {1, 2};
    printf( "b.x = %d, b.y = %d\n", b.x, b.y );
    Vector c = vec_add(a, b);
    printf( "c.x = %d, c.y = %d\n", c.x, c.y );
}