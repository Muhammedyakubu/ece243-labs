#ifndef FINAL_PROJECT_ASTEROID_H
#define FINAL_PROJECT_ASTEROID_H

#define MAX_ASTEROID_RADIUS 16
#define MIN_ASTEROID_RADIUS 4
#define nASTEROID_VERTICES 8

typedef struct Asteroid {
    // The position of the asteroid
    Vector position;
    // The velocity of the asteroid
    Vector velocity;
    // The angle the asteroid is pointing in radians
    float angle;
    // The vertices of the asteroid relative to the center
    Vector vertices[nASTEROID_VERTICES];

    float radius;

    float radius_squared;

    struct Asteroid *next;
} Asteroid;


#endif //FINAL_PROJECT_ASTEROID_H
