#include <math.h>
#include "./ship.h"
#include "./bullet.h"


#define dt = 1;


//updates the bullet repository at every frame
void update_bullets(Game* game) {
    bullet bhead = game->bulletHead;
    bullet b = bhead;
    while (b != NULL) {
        bullet->position = vec_add(bullet->position, vec_mul(bullet->velocity, dt));
        if (withinWindow(b->position)) {
            b->alive = true;
        }
        else {
            b->alive = false;
            removeBullet(bhead, b->position);
        }
        b = b->next;
    }
}

bool withinWindow(Vector b) {
    if ((b->position.x >= 0) && (b->position.y >= 0) && (b->position.x <= RESOLUTION_X) && (b->position.y >= RESOLUTION_Y)) {return true;}
    else {return false;}
}

void check_collision(Vector b, bullet bhead, asteroid ahead) {

} //reverse it so that you are given the asteroid and you iterate through all bullets; consider ship as well



