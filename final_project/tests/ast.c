#include "./ast.h"
void update_asteroids(Game* game) {
    Asteroid *ahead = game->asteroidHead;
    Asteroid *a = ahead;
    while (a != NULL) {
        asteroid->position = vec_add(asteroid->position, vec_mul(asteroid->velocity, dt));
        check_collision(game, a);
        b = b->next;
    }
}

void check_collision(Game* game, Asteroid* a) {
    Bullet* b = game->bulletHead;
    while (b != NULL) {
        if (point_in_asteroid(a, nASTEROID_VERTICES, b->position)) {
            removeBullet(bhead, b);
            if (a->radius <= MIN_ASTEROID_RADIUS) {
                delete_asteroid(game, a);
            }
            else {
                split_asteroid(game, a);
            }
        }
        else {
            int i = 0;
            for (i; i < nSHIP_VERTICES; i++) {
                if (point_in_asteroid(a, nASTEROID_VERTICES, game->player.vertices[i])) {
                    game.lives--;
                    reset_ship(game);
                }
            }
        }
    }
}

split_asteroid(Game* game, Asteroid* a) {
    int i = 0;
    for (i; i < 2; i++) {
        float size = a->radius/2;
        // results in a speed proportional to the asteroids' size
        Vector speed = vec_mul(NORTH, (5 - log(size) / log(2)));
        Asteroid *a1 = new_asteroid(
                a->position,   // randomize and make sure it is not near the ship
                rotate(speed, (float)rand()/RAND_MAX * 2 * M_PI),
                size
        );
        insert_asteroid(game, a1);
    }
    delete_asteroid(game, a);
}