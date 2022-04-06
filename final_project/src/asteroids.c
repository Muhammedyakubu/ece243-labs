/******************************************************************************
 *                               D E F I N E S                                *
 *****************************************************************************/

/* This files provides address values that exist in the system */

#define SDRAM_BASE            0xC0000000
#define FPGA_ONCHIP_BASE      0xC8000000
#define FPGA_CHAR_BASE        0xC9000000

/* Cyclone V FPGA devices */
#define LEDR_BASE             0xFF200000
#define HEX3_HEX0_BASE        0xFF200020
#define HEX5_HEX4_BASE        0xFF200030
#define SW_BASE               0xFF200040
#define KEY_BASE              0xFF200050
#define TIMER_BASE            0xFF202000
#define PIXEL_BUF_CTRL_BASE   0xFF203020
#define CHAR_BUF_CTRL_BASE    0xFF203030

/* VGA colors */
#define BLACK 0x0000
#define WHITE 0xFFFF
#define YELLOW 0xFFE0
#define RED 0xF800
#define GREEN 0x07E0
#define BLUE 0x001F
#define CYAN 0x07FF
#define MAGENTA 0xF81F
#define GREY 0xC618
#define PINK 0xFC18
#define ORANGE 0xFC00

#define ABS(x) (((x) > 0) ? (x) : -(x))

/* Screen size. */
#define RESOLUTION_X 320
#define RESOLUTION_Y 240

#define KEY_NONE 0 // No key pressed
#define KEY_RIGHT 116 // Keyboard Right Arrow
#define KEY_LEFT 107 // Keyboard Left Arrow
#define KEY_DOWN 114 // Keyboard Down Arrow
#define KEY_UP 117 // Keyboard Up Arrow
#define KEY_SPACE 41 // Keyboard Spacebar

#define PS2_BASE              0xFF200100



/******************************************************************************
 *                              I N C L U D E S                               *
 *****************************************************************************/
#define _USE_MATH_DEFINES // for C
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
/* #include "./address_map_arm.h"
#include "./usb_hid_keys.h" */


/******************************************************************************
 *                              S T R U C T S                                 *
 *****************************************************************************/

//================== V E C T O R ==================//

typedef struct Vector
{
    float x, y;
} Vector;

// Creates a new vector
Vector new_vector();
// Adds two vectors
Vector vec_add(Vector, Vector);
// Subtracts a vector from another
Vector vec_sub(Vector, Vector);
// Multiplies the vector by a scalar
Vector vec_mul(Vector, float);
// Calculates the magnitude squared
float magnitude_squared(Vector);
// Calculate the dot product
float dot_product(Vector, Vector);
// mod over the vector
Vector wrap(Vector, Vector);
// Rotates the vector in 2 dimensions
Vector rotate(Vector, float);


//================== S P A C E S H I P ==================//

#define SHIP_ROTATION_SPEED M_PI/12 // fine tune later
#define nSHIP_VERTICES 4
#define SHIP_FRICTION 0.0
#define SHIP_ACCELERATION 5
#define SHIP_MAX_SPEED 10   // fine tune later

typedef struct Ship {
    // The position of the ship
    Vector position;
    // The velocity of the ship
    Vector velocity;
    // The angle the ship is pointing in radians
    float angle;
    // The vertices of the ship
    Vector vertices[nSHIP_VERTICES];
} Ship;


// Rotates the ship left
void rotate_ship_left(Ship*);
// Rotates the ship right
void rotate_ship_right(Ship*);

void accelerate_ship(Ship*);
// updates the position and velocity of the ship
void update_ship(Ship *);

void draw_ship(Ship *);


//================== A S T E R O I D ==================//

// starting radius of asteroid in pixels
#define MAX_ASTEROID_RADIUS 32
#define MIN_ASTEROID_RADIUS 4
#define nASTEROID_VERTICES 8
#define nASTEROIDS 6
#define ASTEROID_MIN_SPEED 2
#define ASTEROID_MAX_SPEED 5

struct Asteroid {
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

    struct Asteroid *prev, *next;
};
typedef struct Asteroid Asteroid;


Asteroid *new_asteroid(Vector p, Vector v, float r);

bool point_in_asteroid(Asteroid*, int, Vector);

void draw_asteroids(Asteroid*);


//================== B U L L E T ==================//

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
        struct Bullet *next, *prev;

};

typedef struct Bullet Bullet;

void draw_bullets(Bullet*);


//================== G A M E ==================//
#define FPS 60
// #define dt 1.0/FPS
#define dt 1.0
Vector CENTER = {RESOLUTION_X/2, RESOLUTION_Y/2};
Vector SCREEN_SIZE = {RESOLUTION_X, RESOLUTION_Y};

typedef struct Game {
    Vector size;

    Ship player;

    Asteroid *asteroidHead;

    Bullet *bulletHead;

    int score;

    int lives;

    int state;

    // Vector* pModel, aModel;
} Game;

void init_game(Game*);

void reset_game(Game*);

void update_game(Game*);


void reset_ship(Game*);


int get_key_pressed();

void handle_key_press(Game*, int);


void insert_asteroid(Game*, Asteroid*);

void delete_asteroid(Game*, Asteroid*);

void delete_asteroid_list(Game*);
// updates the positions of asteroids and does collision detection
void update_asteroids(Game*);

void split_asteroid(Game* game, Asteroid* a);

bool check_collision(Game* game, Asteroid* a);


/* to be implemented (ankur) */
// void insert_bullet(Game*, Bullet*);

// void delete_bullet(Game*, Bullet*);

// void delete_bullet_list(Game*);

// void update_bullets(Game*);

void draw_game(Game*);


//================== R E N D E R I N G   &   G R A P H I C S ==================//

void swap_buffers();
void wait_for_vsync();
void clear_screen();

void plot_pixel(int x, int y, short int line_color);
void vec_plot_pixel(Vector v, short int line_color);

void draw_line(int x0, int y0, int x1, int y1, short int color);
void vec_draw_line(Vector a, Vector b, short int color);

// returns a transformed model "trans" after applying the passed transformations to "model"
void transform_model(Vector *trans, const Vector *model, int num_vertices, Vector translate, float angle, float scale);
// draws lines between each vector in the model
void draw_model(Vector *model, int num_vertices, short int color);


//================== U T I L S ==================//

void swap(int * a, int * b);

bool point_on_screen(Vector p);
// returns a reandom position outside a 32 pixel radius from the player
Vector rand_vec(Game *game);


/******************************************************************************
 *                          P R I V A T E   D A T A                           *
 *****************************************************************************/

//================== G L O B A L S ==================//

volatile int pixel_buffer_start; // global variable

Game game;

const Vector NORTH = {0, -1};


//================== M O D E L S ==================//

const Vector playerModel[] = 
{
    {0, -6},
    {4, 4},
    {0, 2},
    {-4, 4}
};

Vector asteroidModel[nASTEROID_VERTICES];

int main(void)
{
    //================== S E T U P ==================//
    
    volatile int * pixel_ctrl_ptr = (int *)0xFF203020;
    // declare other variables(not shown)
    // initialize location and direction of rectangles(not shown)
    // init();

    /* set front pixel buffer to start of FPGA On-chip memory */
    *(pixel_ctrl_ptr + 1) = 0xC8000000; // first store the address in the 
                                        // back buffer
    /* now, swap the front/back buffers, to set the front buffer location */
    wait_for_vsync();
    /* initialize a pointer to the pixel buffer, used by drawing functions */
    pixel_buffer_start = *pixel_ctrl_ptr;
    clear_screen(); // pixel_buffer_start points to the pixel buffer
    /* set back pixel buffer to start of SDRAM memory */
    *(pixel_ctrl_ptr + 1) = 0xC0000000;
    pixel_buffer_start = *(pixel_ctrl_ptr + 1); // we draw on the back buffer
    clear_screen(); // pixel_buffer_start points to the pixel buffer

    //================== M A I N   L O O P ==================//

    int key_pressed = KEY_NONE;

    init_game(&game);

    while (1)
    {   
        clear_screen();

        // draw all objects

        draw_game(&game);
        swap_buffers();

        // update all objects

        // read ps2 first byte
        key_pressed = get_key_pressed();
        // printf("%d\n", key_pressed);  

        handle_key_press(&game, key_pressed);

        update_game(&game);

        wait_for_vsync(); // swap front and back buffers on VGA vertical sync
        pixel_buffer_start = *(pixel_ctrl_ptr + 1); // new back buffer
    }
    
    return 0;
}



//================== V E C T O R ==================//

Vector new_vector() {
    Vector v = {0, 0};
    return v;
}

Vector vec_add(Vector a, Vector b) {
    Vector c = {0, 0};
    c.x = a.x + b.x;
    c.y = a.y + b.y;
    return c;
}

Vector vec_sub(Vector a, Vector b) {
    Vector c = {0, 0};
    c.x = a.x - b.x;
    c.y = a.y - b.y;
    return c;
}

Vector vec_mul(Vector a, float sf) {
    Vector c = {0, 0};
    c.x = a.x * sf;
    c.y = a.y * sf;
    return c;
}

// We don't calculate the actual magnitude as sqrt is expensive
float magnitude_squared(Vector v) {
    return v.x * v.x + v.y * v.y;
}

float dot_product(Vector a, Vector b) {
    return a.x * b.x + a.y * b.y;
}

Vector wrap(Vector size, Vector p) {
    Vector c;
    c.x = fmod(p.x + size.x, size.x);
    c.y = fmod(p.y + size.y, size.y);
    return c;
}

Vector rotate(Vector v, float a) {
    Vector c;
    c.x = v.x * cos(a) - v.y * sin(a);
    c.y = - v.x * sin(a) + v.y * cos(a);
    return c;
}


//================== S P A C E S H I P ==================//

void rotate_ship_left(Ship *ship) {
    ship->angle -= SHIP_ROTATION_SPEED;
}

void rotate_ship_right(Ship *ship) {
    ship->angle += SHIP_ROTATION_SPEED;
}

void accelerate_ship(Ship* ship) {
    ship->velocity = vec_add(ship->velocity, 
                            vec_mul(rotate(NORTH, ship->angle), 
                                    SHIP_ACCELERATION * dt));
    // printf("%f %f\n", ship->velocity.x, ship->velocity.y);
}

void update_ship(Ship *ship) {
    // consider replacing screen size with game->/.size
    ship->position = wrap(SCREEN_SIZE, vec_add(ship->position, ship->velocity));
    // add some friction to the ship
    ship->velocity = vec_mul(ship->velocity, (pow(1 - SHIP_FRICTION, dt)));    
    // printf("%f %f\n", ship->velocity.x, ship->velocity.y);

}

void draw_ship(Ship *ship) {
    transform_model(ship->vertices, playerModel, nSHIP_VERTICES, ship->position, ship->angle, 2);
    draw_model(ship->vertices, nSHIP_VERTICES, WHITE);
}

//================== A S T E R O I D ==================//
// TODO: need to change this to not assign a model and just use the generic model
Asteroid *new_asteroid(Vector position, Vector velocity, float radius) {
    Asteroid *a = (Asteroid *)malloc(sizeof(Asteroid));
    a->position = position;
    a->velocity = velocity;
    a->angle = 0;
    a->radius = radius;
    a->radius_squared = radius * radius;
    a->prev = a->next = NULL;

    return a;
}

bool point_in_asteroid(Asteroid *asteroid, int num_vertices, Vector p)
{
    // model asteroid as a circle
    return (asteroid->radius_squared >= magnitude_squared(vec_sub(p, asteroid->position)));

    // might want to do more precise collision detection later
}

void draw_asteroids(Asteroid* asteroids) {
    Asteroid *a = asteroids;
    for (; a != NULL; a = a->next) {
        transform_model(a->vertices, asteroidModel, 
                        nASTEROID_VERTICES, a->position, 
                        a->angle, a->radius);
        draw_model(a->vertices, nASTEROID_VERTICES, WHITE);
    }
}
    

//================== B U L L E T ==================//




//================== G A M E ==================//

void init_game(Game* game) {
    game->size = SCREEN_SIZE;

    reset_game(game);

    // set the asteroid model
    int i = 0;
    for (; i < nASTEROID_VERTICES; i++) {
        float rad = (float)rand()/RAND_MAX * 0.4 + 0.8;
        float angle = (float)i * 2 * M_PI / nASTEROID_VERTICES;
        asteroidModel[i].x = rad * cos(angle);
        asteroidModel[i].y = rad * sin(angle);
    }


    // add asteroids
    for (i = 0; i < nASTEROIDS; i++) {
        float size = (float)rand()/RAND_MAX * (MAX_ASTEROID_RADIUS - MIN_ASTEROID_RADIUS) + MIN_ASTEROID_RADIUS;
        // results in a speed proportional to the asteroids' size
        Vector speed = vec_mul(NORTH, 
                                ASTEROID_MIN_SPEED * (5 - (log(size) / log(2))));    
        Asteroid *a = new_asteroid(
            rand_vec(game),   // randomize and make sure it is not near the ship
            rotate(speed, (float)rand()/RAND_MAX * 2 * M_PI),
            size
        );
        insert_asteroid(game, a);
    }
}

void reset_game(Game* game) {
    reset_ship(game);

    delete_asteroid_list(game);
    // delete_bullet_list(game);
    game->score = 0;
    game->lives = 5;
}

void reset_ship(Game* game) {
    game->player.position = vec_mul(game->size, 0.5);
    game->player.velocity = new_vector();
    game->player.angle = 0;
}

void update_game(Game* game) {
    update_asteroids(game);
    // update_bullets(game);
    update_ship(&game->player);
}

void draw_game(Game *game) {
    draw_asteroids(game->asteroidHead);
    // draw_bullets(game->bulletHead);
    draw_ship(&game->player);

    /* to be implemented */
    // draw_score(game->score);
    // draw_lives(game->lives);
}

int get_key_pressed() {
    volatile int* PS2_ptr = (int*)PS2_BASE;
    int PS2_data, RVALID;
    char byte1 = 0, byte2 = 0, byte3 = 0;
    PS2_data = *(PS2_ptr); // read the Data register in the PS/2 port
    RVALID = PS2_data & 0x8000; // extract the RVALID field
    if (RVALID)
    {
        /* save the last three bytes of data */
        byte1 = byte2;
        byte2 = byte3;
        byte3 = PS2_data & 0xFF;
        // printf("%d, %d, %d\n", byte1, byte2, byte3);
        return byte3;
    }
    else return KEY_NONE;
}

void handle_key_press(Game* game, int key_pressed) {
    if (key_pressed == KEY_RIGHT)
    {
        rotate_ship_right(&game->player);
    }
    else if (key_pressed == KEY_LEFT)
    {
        rotate_ship_left(&game->player);
    }
    else if (key_pressed == KEY_UP)
    {
        accelerate_ship(&game->player);
    }
    else if (key_pressed == KEY_SPACE)
    {
        // shoot bullet
    }
}

void insert_asteroid(Game* game, Asteroid* a){
    if (game->asteroidHead) 
        game->asteroidHead->prev = a;
    a->next = game->asteroidHead;
    game->asteroidHead = a;
}

void count_asteroids (Game* game) {
    int count = 0;
    Asteroid *a = game->asteroidHead;
    for (; a != NULL; a = a->next) {
        count++;
    }
    printf("num asteroids: %d\n", count);
}

void delete_asteroid(Game* game, Asteroid* a) {
    if (game->asteroidHead == a)
        game->asteroidHead = a->next;
    if (a->prev)
        a->prev->next = a->next;
    if (a->next)
        a->next->prev = a->prev;
    free(a);
}

void update_asteroids(Game* game) {
    Asteroid *a = game->asteroidHead;
    for (; a != NULL; a = a->next) {
        a->position = vec_add(a->position, vec_mul(a->velocity, dt));
        a->position = wrap(game->size, a->position);

        if (check_collision(game, a)) {
            if (a->radius > MIN_ASTEROID_RADIUS) 
                split_asteroid(game, a);
            else 
                delete_asteroid(game, a);
        }

        // printf("asteroid position: ");
        // printf("%f, %f\n", a->position.x, a->position.y);
        // printf("asteroid velocity: ");
        // printf("%f, %f\n", a->velocity.x, a->velocity.y);
        // a->angle += 0.01;
    }
}

bool check_collision(Game* game, Asteroid* a) {

    // check collision with each bullet
    /* Bullet* b = game->bulletHead;
    for (; b != NULL; b = b->next) {
        if (point_in_asteroid(a, nASTEROID_VERTICES, b->position)) {
            // delete bullet
            delete_bullet(game, b);
            // update score
            game->score += 1;
            return true;
        }
    } */

    // check collision with ship
    int i = 0;
    for (; i < nSHIP_VERTICES; i++) {
        if (point_in_asteroid(a, nASTEROID_VERTICES, game->player.vertices[i])) {
            // update lives
            game->lives -= 1;
            // reset ship
            // may need to change this logic in case the user dies at the center of the screen
            reset_ship(game);
            return true;
        }
    }

    return false;
}

void split_asteroid(Game* game, Asteroid* a) {
    int i = 0;
    for (; i < 2; i++) {
        float size = a->radius/2;
        // results in a speed proportional to the asteroids' size
        Vector speed = vec_mul(NORTH, 
                                ASTEROID_MIN_SPEED * (5 - (log(size) / log(2))));
                                
        Asteroid *a_new = new_asteroid(
                a->position,
                rotate(speed, (float)rand()/RAND_MAX * 2 * M_PI),
                size
        );
        insert_asteroid(game, a_new);
    }
    delete_asteroid(game, a);
}

void delete_asteroid_list(Game* game) {
    Asteroid *a = game->asteroidHead;
    while (a != NULL) {
        Asteroid *next = a->next;
        free(a);
        a = next;
    }
    game->asteroidHead = NULL;
}

//================== R E N D E R I N G   &   G R A P H I C S ==================//

void swap_buffers() {
    volatile int * pixel_ctrl_ptr = (int *)0xFF203020;
    *pixel_ctrl_ptr = 1;
}

void wait_for_vsync()
{
    volatile int * pixel_ctrl_ptr = (int *)0xFF203020;
    // *pixel_ctrl_ptr = 1;

    while (*(pixel_ctrl_ptr + 3) & 1); // wait until S bit is 0
}

void vec_plot_pixel(Vector v, short int line_color)
{
    plot_pixel(v.x, v.y, line_color);
}

void plot_pixel(int x, int y, short int line_color)
{
    // if pixel not in bounds, wrap around
    if (x < 0)
        x = RESOLUTION_X + x;
    if (x >= RESOLUTION_X)
        x = x - RESOLUTION_X;
    if (y < 0)
        y = RESOLUTION_Y + y;
    if (y >= RESOLUTION_Y)
        y = y - RESOLUTION_Y;

    *(short int *)(pixel_buffer_start + (y << 10) + (x << 1)) = line_color;
}

void vec_draw_line(Vector a, Vector b, short int color)
{
    draw_line(a.x, a.y, b.x, b.y, color);
}

void draw_line(int x0, int y0, int x1, int y1, short int color) 
{
    bool is_steep = abs(y1 - y0) > abs(x1 - x0);
    
    if (is_steep) 
    {
        swap(&x0, &y0);
        swap(&x1, &y1);
    } 
        
    if (x0 > x1) 
    {
        swap(&x0, &x1);
        swap(&y0, &y1);
    }

    int deltax = x1 - x0;
    int deltay = abs(y1 - y0); 
    int error = -(deltax / 2);
    int y = y0;
    int y_step = (y0 < y1)? 1 : -1;
    
    int x = x0;
    for (; x < x1; ++x)
    {
        is_steep ? plot_pixel(y, x, color) : plot_pixel(x, y, color);

        error = error + deltay;
        
        if (error > 0) 
        {
            y = y + y_step;
            error = error - deltax;
        }
    }    
}

void clear_screen() 
{   
    int y = 0;
    for (; y < RESOLUTION_Y; y++)
    {
        int x0 = 0, x1 = RESOLUTION_X ;
        draw_line(x0, y, x1, y, BLACK);
    }
}

void transform_model(Vector *trans, const Vector *model, int num_vertices, Vector t, float a, float sf)
{
    int i = 0;
    for(; i < num_vertices; i++)
    {   
        // apply rotation
        trans[i] = rotate(model[i], a);
        // printf("rotated: trans.x = %f, trans.y = %f\n", trans[i].x, trans[i].y);

        // apply scaling
        trans[i] = vec_mul(trans[i], sf);
        // printf("scaled: trans.x = %f, trans.y = %f\n", trans[i].x, trans[i].y);

        // apply translation
        trans[i] = vec_add(trans[i], t);
        // printf("translated: trans.x = %f, trans.y = %f\n", trans[i].x, trans[i].y);

    }
}

void draw_model(Vector model[], int num_vertices, short int color)
{
    int i = 0;
    for(; i < num_vertices ; i++)
    {
        vec_draw_line(model[i], model[ (i+1) % num_vertices], color);
    }
}


//================== U T I L S ==================//

void swap(int * a, int * b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

bool point_on_screen(Vector p)
{
    return p.x >= 0 && p.x < RESOLUTION_X && p.y >= 0 && p.y < RESOLUTION_Y;
}

Vector rand_vec(Game *game)
{
    int radius = 2 * MAX_ASTEROID_RADIUS;
    Vector p;
    p.x = rand() % RESOLUTION_X;
    while (p.x <= radius + game->player.position.x && p.x >= game->player.position.x - radius)
    {
        p.x = rand() % RESOLUTION_X;
    }
    p.y = rand() % RESOLUTION_Y;
    while (p.y <= radius + game->player.position.y && p.y >= game->player.position.y - radius)
    {
        p.y = rand() % RESOLUTION_Y;
    }
    // printf("rand_vec: %f, %f\n", p.x, p.y);
    return p;
}