#include "asteroidStructure.h"

Asteroid *new_asteroid(Vector position, Vector velocity, float radius) {
    Asteroid *a = (Asteroid *)malloc(sizeof(Asteroid));
    a->position = position;
    a->velocity = velocity;
    a->angle = 0;
    a->radius = radius;
    a->radius_squared = radius * radius;
    a->prev = a->next = NULL;

    // assign vertices
    int i = 0;
    for (; i < nASTEROID_VERTICES; i++) {
        float rad = (float)rand()/RAND_MAX * 0.4 + 0.8;
        float angle = (float)i * 2 * M_PI / nASTEROID_VERTICES;
        a->vertices[i].x = rad * cos(angle);
        a->vertices[i].y = rad * sin(angle);
    }

    return a;
}

void insert_asteroid(Game* game, Asteroid* a){
    if(game->asteroidHead == NULL){game->asteroidHead = a;}
    else{
        a->next = game->asteroidHead;
        game->asteroidHead = a;
    }
}

void delete_asteroid(Game* game, Asteroid* a) {
    Asteroid* a1 = game->bulletHead;
    if (head == NULL) {return;}
    while (a1->next != NULL) {
        if (a1 == a) {
            if (a->prev == NULL) {
                game->bulletHead = a->next;
            }
            else {
                a->prev->next = a->next;
            }
            free(a);
        }
    }
}