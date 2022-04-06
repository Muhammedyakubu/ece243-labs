#ifndef FINAL_PROJECT_BULLETSTRUCTURE_H
#define FINAL_PROJECT_BULLETSTRUCTURE_H

struct Bullet {
        // The original position of the bullet
        Vector position;
        // The velocity of the bullet
        Vector velocity;
        // is the bullet alive
        bool alive;
        //next bullet
        struct Bullet *prev, *next;

};

typedef struct Bullet Bullet;


#endif //FINAL_PROJECT_BULLETSTRUCTURE_H
