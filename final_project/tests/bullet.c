#include <math.h>
#include "./ship.h"
#include "./bullet.h"

#define dt = 1;


//updates the bullet repository at every frame
void update_bullets(Game* game) {
    Bullet* b = game->bulletHead;
    while (b != NULL) {
        bullet->position = vec_add(bullet->position, vec_mul(bullet->velocity, dt));
        if (withinWindow(b->position)) {
            b->alive = true;
        }
        else {
            b->alive = false;
            delete_bullet(game, b);
        }
        b = b->next;
    }
}

bool withinWindow(Vector b) {
    if ((b.x >= 0) && (b.y >= 0) && (b.x <= RESOLUTION_X) && (b.y >= RESOLUTION_Y)) {return true;}
    else {return false;}
}



