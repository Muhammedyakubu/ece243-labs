#ifndef FINAL_PROJECT_ASTEROIDSTRUCTURE_H
#define FINAL_PROJECT_ASTEROIDSTRUCTURE_H

struct Asteroid {
    // The position of the asteroid
    Vector position;
    // The velocity of the asteroid
    Vector velocity;
    // The angle the asteroid is pointing in radians
    float angle;
    // The vertices of the asteroid relative to the center
    Vector vertices[nASTEROID_VERTICES];

    bool alive;

    float radius;

    float radius_squared;

    struct Asteroid *prev, *next;
};

typedef struct Asteroid Asteroid;

#endif //FINAL_PROJECT_ASTEROIDSTRUCTURE_H
