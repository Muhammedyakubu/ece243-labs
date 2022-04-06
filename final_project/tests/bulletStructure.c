#include "./bulletStructure.h"

bullet createBullet(){
    bullet newBullet = (bullet)malloc(sizeof(struct BulletList)); // allocate memory using malloc()
    newBullet->next = NULL;// make next point to NULL
    return newBullet;//return the new node
}

bullet addBullet(bullet head, Vector pos){
    bullet b1 = createBullet(pos);
    bullet b2 = NULL;
    b1->position = pos;
    if(head == NULL){head = b1;}
    else{
        b2 = head;
        while(b2->next != NULL){
            b2 = b2->next;
        }
        b2->next = temp;
    }
    return head;
}

//deletes a bullet
void removeBullet(bullet head, Vector pos) {
    bullet b1 = head;
    bullet b2 = NULL;
    if (head == NULL) {return;}
    while (b1->next != NULL) {
        b2 = b1->next;
        if (b2->position == pos) {
            b1->next = b2->next;
            delete b2;
            break;
        }
        b1 = b1->next;
    }
}

//to traverse:
//bullet b;
//b = head;
//while(b != NULL){
//    b = b->next;
//}