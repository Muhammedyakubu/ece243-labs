#ifndef FINAL_PROJECT_BULLET_H
#define FINAL_PROJECT_BULLET_H

#include "./vector.h"
#include "./bulletStructure.h"
#define BULLET_X_VELOCITY 600



//creates a new bullet
Bullet new_bullet(vector, float);
//deletes a bullet
void destroy_bullet(Bullet*);

//updates the position of the bullet
void update_bullet_position(Bullet*);

//updates the bullet repository at every frame
void update_bullet_repository(vector<Bullet> &repo);




#endif //FINAL_PROJECT_BULLET_H
