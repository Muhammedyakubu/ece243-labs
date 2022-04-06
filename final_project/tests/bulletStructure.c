#include "./bulletStructure.h"

Bullet *new_bullet(Vector position, float angle){
    Bullet *b = (Bullet *)malloc(sizeof(Bullet));
    b->position = position;
    b->velocity = vec_mul(rotate(NORTH, angle), BULLET_SPEED);
    b->prev = b->next = NULL;
    return b;
}


void insert_bullet(Game* game, Bullet* b){
    if(game->bulletHead == NULL){game->bulletHead = b;}
    else{
        b->next = game->bulletHead;
        game->bulletHead = b;
    }
}

//deletes a bullet
void delete_bullet(Game* game, Bullet* b) {
    Bullet* b1 = game->bulletHead;
    if (head == NULL) {return;}
    while (b1->next != NULL) {
        if (b1 == b) {
            if (b->prev == NULL) {
                game->bulletHead = b->next;
            }
            else {
                b->prev->next = b->next;
            }
            free(b);
        }
    }
}

