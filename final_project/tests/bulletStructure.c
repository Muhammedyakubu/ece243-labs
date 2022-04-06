#include "./bulletStructure.h"

Bullet *new_bullet(Vector position, float angle){
    Bullet *b = (Bullet *)malloc(sizeof(Bullet));
    b->position = position;
    b->velocity = vec_mul(rotate(NORTH, angle), BULLET_SPEED);
    b->prev = b->next = NULL;
    return b;
}


void insert_bullet(Game* game, Bullet* b){
    b->next = game->bulletHead;

    if(game->bulletHead != NULL)
        game->bulletHead->prev = b;

    game->bulletHead = b;
}

//deletes a bullet
void delete_bullet(Game* game, Bullet* b) {
    if (game->bulletHead == b)
        game->bulletHead = b->next;
    if (b->prev)
        b->prev->next = b->next;
    if (b->next)
        b->next->prev = b->prev;
    free(b);
}

