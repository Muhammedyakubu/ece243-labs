#include <stdio.h>
#include "./vector.h"
#include "./ship.h"

int main () {
    // Vector a = new_vector();
    Vector a;
    printf( "a.x = %f, a.y = %f\n", a.x, a.y );
    Vector b = {1, 2};
    printf( "b.x = %f, b.y = %f\n", b.x, b.y );
    Vector c = vec_mul(b, 2);
    printf( "c.x = %f, c.y = %f\n", c.x, c.y );
    Ship d = new_ship(a, b);
    printf( "d.position.x = %f, d.position.y = %f\n", d.position.x, d.position.y );
    printf( "d.velocity.x = %f, d.velocity.y = %f\n", d.velocity.x, d.velocity.y );
}