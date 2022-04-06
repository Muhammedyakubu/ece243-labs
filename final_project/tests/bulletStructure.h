#ifndef FINAL_PROJECT_BULLETSTRUCTURE_H
#define FINAL_PROJECT_BULLETSTRUCTURE_H

struct Bullet {
        // The original position of the bullet
        Vector position;
        // The velocity of the bullet
        Vector velocity;
        // The angle the bullet is pointing at
        float angle;
        // is the bullet alive
        bool alive;
        //next bullet
        struct Bullet *next;

} Bullet;

//typedef struct Bullet *bullet; //Define bullet as pointer of data type struct BulletRepository

#endif //FINAL_PROJECT_BULLETSTRUCTURE_H
