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
#define AUDIO_BASE            0xFF203040

//#include "address_map_arm.h"
#define BUF_SIZE 80000 // about 10 seconds of buffer (@ 8K samples/sec)
#define BUF_THRESHOLD 96 // 75% of 128 word buffer



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
#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

/* Screen size. */
#define RESOLUTION_X 320
#define RESOLUTION_Y 240

#define PS2_BASE              0xFF200100

/******************************************************************************
 *                              I N C L U D E S                               *
 *****************************************************************************/
#define _USE_MATH_DEFINES // for C
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>


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

#define nSHIP_VERTICES_THRUST 12
#define nSHIP_VERTICES 4
#define SHIP_COLOR CYAN
#define SHIP_SCALE 1.3
#define SHIP_LENGTH 10
#define SHIP_WIDTH 6

#define SHIP_ROTATION_P_SEC 1.4 * M_PI // 0.7 rotations per second
#define SHIP_FRICTION 0.55
#define SHIP_ACCELERATION 150
#define SHIP_MAX_SPEED 170  // based on real game speed

typedef struct Ship {
    // The position of the ship
    Vector position;
    // The velocity of the ship
    Vector velocity;
    // The angle the ship is pointing in radians
    float angle;
    // The vertices of the ship
    Vector vertices[nSHIP_VERTICES_THRUST];

    Vector old_vertices[nSHIP_VERTICES_THRUST];

    bool thrusting;
} Ship;


// Rotates the ship left
void rotate_ship_left(Ship*);
// Rotates the ship right
void rotate_ship_right(Ship*);

void accelerate_ship(Ship*);
// updates the position and velocity of the ship
void update_ship(Ship *);

void draw_ship(Ship *, short int);


//================== A S T E R O I D ==================//

// starting radius of asteroid in pixels
#define ASTEROID_MAX_RADIUS 18
#define ASTEROID_MIN_RADIUS 6

#define ASTEROID_MAX_SPEED 30
#define ASTEROID_MIN_SPEED 10

#define nASTEROID_VERTICES 12
#define nASTEROIDS 4

#define ASTEROID_COLOR ORANGE   

#define ASTEROID_MIN_SCORE 25

struct Asteroid {
    // The position of the asteroid
    Vector position;
    // The velocity of the asteroid
    Vector velocity;
    // The angle the asteroid is pointing in radians
    float angle;
    // The vertices of the asteroid relative to the center
    Vector vertices[nASTEROID_VERTICES];

    Vector old_vertices[nASTEROID_VERTICES];

    float radius;

    float radius_squared;

    bool dead;

    struct Asteroid *prev, *next;
};
typedef struct Asteroid Asteroid;


Asteroid *new_asteroid(Vector p, Vector v, float r);

bool point_in_asteroid(Asteroid*, int, Vector);

void draw_asteroids(Asteroid*);


//================== B U L L E T ==================//

#define BULLET_SPEED 200
#define BULLET_SIZE 2
#define BULLET_COLOR PINK
#define BULLET_COOLDOWN 0.05

struct Bullet {
    // The original position of the bullet
    Vector position;

    Vector old_position;
    // The velocity of the bullet
    Vector velocity;
    // is the bullet alive
    bool alive;
    //next bullet
    struct Bullet *next, *prev;

};

typedef struct Bullet Bullet;


Bullet *new_bullet(Vector position, float angle);

void draw_bullets(Bullet*);

//================== A L I E N ==================//

#define nALIEN_VERTICES_THRUST 20
#define nALIEN_VERTICES 12
#define ALIEN_COLOR ORANGE
#define ALIEN_SCALE 2
#define ALIEN_LENGTH 10
#define ALIEN_WIDTH 16

#define ALIEN_ACCELERATION 150
#define ALIEN_MAX_SPEED 170  // based on real game speed

double alienshootcooldown = 0.1;
double alientimer = 10;

typedef struct Alien {
    // The position of the ship
    Vector position;
    // The velocity of the ship
    Vector velocity;
    // The angle the ship is pointing in radians
    float angle;
    // The vertices of the ship
    Vector vertices[nALIEN_VERTICES_THRUST];

    Vector old_vertices[nALIEN_VERTICES_THRUST];

    float radius;

    float radius_squared;

    bool thrusting;

    bool alive;
} Alien;


// Rotates the ship left
void rotate_alien_left(Alien*);
// Rotates the ship right
void rotate_alien_right(Alien*);

void accelerate_alien(Alien*);
// updates the position and velocity of the ship

void draw_alien(Alien *, short int);

void clear_alien(Alien*);


//================== G A M E ==================//
#define FPS 60

#define COLLISION_BULLET 1
#define COLLISION_SHIP 2

const Vector SCREEN_SIZE = {RESOLUTION_X, RESOLUTION_Y};
const Vector CENTER = {RESOLUTION_X/2, RESOLUTION_Y/2};

#define nLIVES 5

typedef struct Game {
    Vector size;

    Ship player;

    Alien alien;

    Asteroid *asteroidHead;

    Bullet *bulletHead;

    int score;
    
    int level;

    int lives, bonus_lives;

    bool running;

} Game;

void init_game(Game*);

void main_screen(Game*);
void clear_main_screen(Game*);
void game_over(Game*);
void clear_game_over(Game*);
void draw_score(Game*);
void reset_game(Game*);

void update_game(Game*);


void reset_ship(Game*);
void reset_alien(Game*);



void insert_asteroid(Game*, Asteroid*);

void delete_asteroid(Game*, Asteroid*);

void delete_asteroid_list(Game*);
// updates the positions of asteroids and does collision detection
void update_asteroids(Game*);

void split_asteroid(Game* game, Asteroid* a);

bool check_collision(Game* game, Asteroid* a);

bool check_collision_alien(Game* game);

void add_random_asteroids(Game*, int);


Bullet *new_bullet(Vector p, float angle);

void shoot_bullet(Game*);

void update_alien(Alien *, Game*);

void shoot_alien_bullet(Game*);

void insert_bullet(Game*, Bullet*);

void delete_bullet(Game*, Bullet*);

void delete_bullet_list(Game*);

void update_bullets(Game*);


void draw_game(Game*);

void draw_lives(Game*);


//================== K E Y S ==================//

#define KEY_NONE 0 // No key pressed
#define KEY_TAB 0x0D //keyboard TAB
#define KEY_R 0x2D //keyboard R
#define KEY_Q 21 //keyboard Q
#define KEY_RIGHT 116 // Keyboard Right Arrow
#define KEY_LEFT 107 // Keyboard Left Arrow
#define KEY_DOWN 114 // Keyboard Down Arrow
#define KEY_UP 117 // Keyboard Up Arrow
#define KEY_SPACE 41 // Keyboard Spacebar
#define KEY_ESC 0x76 // Keyboard Esc

#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3
#define SPACE 4
#define ESC 5
#define TAB 6
#define R 7
#define Q 8

#define nKEYS 9

typedef struct Key
{
    char code;
    bool is_down;
} Key;

Key keys[] = 
{
    [UP] = {KEY_UP, false},
    [DOWN] = {KEY_DOWN, false},
    [LEFT] = {KEY_LEFT, false},
    [RIGHT] = {KEY_RIGHT, false},
    [SPACE] = {KEY_SPACE, false},
    [ESC] = {KEY_ESC, false},
    [TAB] = {KEY_TAB, false},
    [R] = {KEY_R, false},
    [Q] = {KEY_Q, false},
};


void reset_keys();

void update_pressed_keys();

void handle_pressed_keys(Game*);


//================== R E N D E R I N G   &   G R A P H I C S ==================//

void swap_buffers();
void wait_for_vsync();
void clear_screen();
void clear_screen_fast(Game*);

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

// DEBUGGING DEFINES

#define CLEAR_FAST
// #define PRINT_KEYS
// #define PRINT_VELOCITY
// #define PRINT_FPS
#define CPULATOR




volatile int pixel_buffer_start; // global variable

// initialize time increment
float dt = 1.0/FPS;

// the main game object
Game game;

// bullet cooldown
double b_cooldown = BULLET_COOLDOWN;

double alienbulletcooldown = BULLET_COOLDOWN;

// north vector for calculations
const Vector NORTH = {0, -1};

// for processing ps2 input
char byte1 = 0, byte2 = 0, byte3 = 0;



//================== M O D E L S ==================//

/* const Vector playerModel[] = 
{
    {0, 0},
    {3, 2},
    {0, -8},
    {-3, 2},
    {0, 0},
    {-1, 2},
    {0, 8},
    {1, 2},
}; */
const Vector playerModel[] =
{
    {0, 0},
    {3, 2},
    {0, -8},
    {-3, 2},
    {0, 0},

    {-0.5, 2},
    {0, 6},
    {0.5, 2},
    {0, 0},
    {-1, 2},
    {0, 8},
    {1, 2},
};

const Vector alienModel[] = {
        {0, 0},
        {6, 2},
        {8, 0},
        {6, -2},
        {3, -2},
        {2, -6},
        {0, -8},
        {-2, -6},
        {-3, -2},
        {-6, -2},
        {-8, 0},
        {-6, 2},
        {0, 0},

        {-0.5, 2},
        {0, 6},
        {0.5, 2},
        {0, 0},
        {-1, 2},
        {0, 8},
        {1, 2},
};


Vector alienpositiontemp = {100, 100};
Vector asteroidModel[nASTEROID_VERTICES];

int main(void)
{
    //================== V G A   S E T U P ==================//
    
    volatile int * pixel_ctrl_ptr = (int *)0xFF203020;

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


    //================== G A M E   S E T U P ==================//

    volatile int * led_ptr = (int *)LEDR_BASE;
    volatile int * sw_ptr = (int *)SW_BASE;

    // indicate that game is being run when when any switch is toggled
    while (*sw_ptr == 0);
    *led_ptr = *sw_ptr;

    clock_t last_drawn, now;
    last_drawn = clock();

    //================== M A I N   L O O P ==================// 

    while (1) {
        clear_screen();
        main_screen(&game);
        wait_for_vsync(); // swap front and back buffers on VGA vertical sync

        while (!keys[TAB].is_down) {
            update_pressed_keys();
        }

        clear_screen();
        wait_for_vsync(); // swap front and back buffers on VGA vertical sync

        init_game(&game);

        while (game.lives > 0 && game.running)
        {
            // indicate that game is running
            *led_ptr = *sw_ptr;

            // clear screen
            #ifdef CLEAR_FAST
            clear_screen_fast(&game);
            #else
            clear_screen();
            #endif

            // update all objects
            update_game(&game);

            // draw all objects
            draw_game(&game);

            // new handle key press
            update_pressed_keys();
            handle_pressed_keys(&game);

            wait_for_vsync(); // swap front and back buffers on VGA vertical sync
            pixel_buffer_start = *(pixel_ctrl_ptr + 1); // new back buffer

            // time taken for draw
            now = clock();
            dt = (float)(now - last_drawn) / CLOCKS_PER_SEC;
            last_drawn = now;
            #ifdef PRINT_FPS
            printf("seconds per frame: %f, fps: %f\n", dt, 1.0/dt);
            #endif
        }

        clear_screen();

        game_over(&game);
        draw_score(&game);

        wait_for_vsync(); // swap front and back buffers on VGA vertical sync

        while (!keys[TAB].is_down) {
            update_pressed_keys();
        }

        reset_game(&game);

        clear_screen();
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

    #ifdef CPULATOR
    c.y = - v.x * sin(a) + v.y * cos(a);
    #else
    c.y = - (v.x * sin(a) + v.y * cos(a));
    #endif
    return c;
}


//================== S P A C E S H I P ==================//

void rotate_ship_left(Ship *ship) {
    #ifdef CPULATOR
    ship->angle -= SHIP_ROTATION_P_SEC * dt;
    #else
    ship->angle += SHIP_ROTATION_P_SEC * dt;
    #endif
}

void rotate_ship_right(Ship *ship) {
    #ifdef CPULATOR
    ship->angle += SHIP_ROTATION_P_SEC * dt;
    #else
    ship->angle -= SHIP_ROTATION_P_SEC * dt;
    #endif
}

void bound_speed(Ship *ship) {
    if (ship->velocity.x > SHIP_MAX_SPEED)
        ship->velocity.x = SHIP_MAX_SPEED;
    else if (ship->velocity.x < -SHIP_MAX_SPEED)
        ship->velocity.x = -SHIP_MAX_SPEED;
    if (ship->velocity.y > SHIP_MAX_SPEED)
        ship->velocity.y = SHIP_MAX_SPEED;
    else if (ship->velocity.y < -SHIP_MAX_SPEED)
        ship->velocity.y = -SHIP_MAX_SPEED;
}

void accelerate_ship(Ship* ship) {
    ship->velocity = vec_add(ship->velocity, 
                            vec_mul(rotate(NORTH, ship->angle), 
                                    SHIP_ACCELERATION * dt));
    bound_speed(ship);
}

void update_ship(Ship *ship) {
    // consider replacing screen size with game->/.size
    ship->position = wrap(
        SCREEN_SIZE, 
        vec_add(ship->position, vec_mul(ship->velocity, dt))
    );

    // add some friction to the ship when it is not accelerating
    if (ship->thrusting) 
        accelerate_ship(ship);

    ship->velocity = vec_mul(ship->velocity, (pow(1 - SHIP_FRICTION, dt)));

    #ifdef PRINT_VELOCITY
    printf("ship velocity: %f %f\n", ship->velocity.x, ship->velocity.y );
    #endif

}

void draw_ship(Ship *ship, short int color) {
    transform_model(ship->vertices, playerModel, nSHIP_VERTICES_THRUST, ship->position, ship->angle, SHIP_SCALE);
    draw_model(ship->vertices, nSHIP_VERTICES, color);

    if (!ship->thrusting) return;

    // draw thruster
    draw_model(ship->vertices + 4, nSHIP_VERTICES, RED);
    draw_model(ship->vertices + 8, nSHIP_VERTICES, YELLOW);
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
    a->dead = false;
    transform_model(a->vertices, asteroidModel, nASTEROID_VERTICES, position, 0, radius);
    transform_model(a->old_vertices, asteroidModel, nASTEROID_VERTICES, position, 0, radius);

    return a;
}

bool point_in_asteroid(Asteroid *asteroid, int num_vertices, Vector p)
{   
    // model asteroid as a circle
    if (asteroid->radius_squared >= magnitude_squared(vec_sub(p, asteroid->position))) 
        return true;

    // testing how much this affects the performance
    /* for (int i = -1; i <= 1; ++i) {
        for (int j = -1; j <= 1; ++j) {
            Vector q = {p.x + i * SCREEN_SIZE.x, p.y + j * SCREEN_SIZE.y};
            if (asteroid->radius_squared >= magnitude_squared(vec_sub(q, asteroid->position)))
                return true;
        }
    } */

    return false;

    // might want to do more precise collision detection later
}

void draw_asteroids(Asteroid* asteroids) {
    Asteroid *a = asteroids;
    for (; a != NULL; a = a->next) {
        #ifdef CLEAR_FAST
        if (a->dead) continue;
        #endif
        transform_model(a->vertices, asteroidModel, 
                        nASTEROID_VERTICES, a->position, 
                        a->angle, a->radius);
        draw_model(a->vertices, nASTEROID_VERTICES, ASTEROID_COLOR);
    }
}
    

//================== B U L L E T ==================//

Bullet *new_bullet(Vector position, float angle){
    Bullet *b = (Bullet *)malloc(sizeof(Bullet));
    b->old_position = b->position = position;
    b->alive = true;
    b->velocity = vec_mul(rotate(NORTH, angle), BULLET_SPEED);
    b->prev = b->next = NULL;
    return b;
}

void draw_bullet(Vector p, short int color) {
    int i, j;
    for (i = 0; i < BULLET_SIZE; i++) {
        for (j = 0; j < BULLET_SIZE; j++) {
            plot_pixel(p.x + i, p.y + j, color);
        }
    }
}

void draw_bullets(Bullet* bullets) {
    Bullet *b = bullets;
    for (; b != NULL; b = b->next) {
        #ifdef CLEAR_FAST
        if (!b->alive) continue;
        #endif

        draw_bullet(b->position, BULLET_COLOR);
    }
}


//================== G A M E ==================//

void press_tab(Game* game);

void main_screen(Game* game) {
    int i = 2;
    int w = SCREEN_SIZE.x;
    int h = SCREEN_SIZE.y;
    int a = (w / 2 - 78);
    int b = (w / 2 - 14 - 69);
    int c = (w / 2 + 78);
    int d = (w / 2 + 14 - 69);
    Vector e = {a, b};
    Vector f = {a, d};
    Vector g = {c, b};
    Vector j = {c, d};
    vec_draw_line(e, f, WHITE);
    vec_draw_line(e, g, WHITE);
    vec_draw_line(j, g, WHITE);
    vec_draw_line(j, f, WHITE);
    for (; i < 11; i++) {
        int x = (w / 2 - 78) + 13*i;
        int y = (w / 2 - 14) + 6 - 69;
            if (i==2) {
                Vector a = {x, y};
                Vector b = {x - 4, y + 17};
                Vector c = {x + 4, y + 17};
                Vector d = {x, y + 13};
                vec_draw_line(a, b, MAGENTA);
                vec_draw_line(a, c, MAGENTA);
                vec_draw_line(b, d, MAGENTA);
                vec_draw_line(c, d, MAGENTA);
            }
            if (i==3) {
                Vector a = {x + 4, y - 1};
                Vector b = {x - 4, y + 4 - 1};
                Vector c = {x + 4, y + 13 - 1};
                Vector d = {x - 4, y + 17 - 1};
                vec_draw_line(a, b, CYAN);
                vec_draw_line(b, c, CYAN);
                vec_draw_line(c, d, CYAN);
            }
            if (i==4) {
                Vector a = {x - 4, y};
                Vector b = {x + 4, y};
                Vector c = {x, y};
                Vector d = {x, y + 17};
                vec_draw_line(a, b, CYAN);
                vec_draw_line(c, d, CYAN);
            }
            if (i==5) {
                Vector a = {x - 4, y};
                Vector b = {x + 4, y};
                Vector c = {x - 4, y + 17};
                Vector d = {x + 4, y + 17};
                Vector e = {x - 4, y + 9};
                Vector f = {x + 4, y + 9};
                vec_draw_line(a, b, CYAN);
                vec_draw_line(a, c, CYAN);
                vec_draw_line(c, d, CYAN);
                vec_draw_line(e, f, CYAN);
            }
            if (i==6) {
                Vector a = {x - 4, y};
                Vector b = {x - 4, y + 17};
                Vector c = {x + 4, y + 4};
                Vector d = {x - 4, y + 9};
                Vector e = {x + 4, y + 17};
                vec_draw_line(a, b, CYAN);
                vec_draw_line(a, c, CYAN);
                vec_draw_line(c, d, CYAN);
                vec_draw_line(d, e, CYAN);
            }
            if (i==7) {
                Vector a = {x - 4, y};
                Vector b = {x + 4, y};
                Vector c = {x - 4, y + 17};
                Vector d = {x + 4, y + 17};
                vec_draw_line(a, b, CYAN);
                vec_draw_line(a, c, CYAN);
                vec_draw_line(c, d, CYAN);
                vec_draw_line(b, d, CYAN);
            }
            if (i==8) {
                Vector a = {x, y};
                Vector b = {x, y + 17};
                vec_draw_line(a, b, CYAN);
            }
            if (i==9) {
                Vector a = {x - 4, y};
                Vector b = {x - 4, y + 17};
                Vector c = {x + 4, y + 9};
                vec_draw_line(a, b, CYAN);
                vec_draw_line(a, c, CYAN);
                vec_draw_line(c, b, CYAN);
            }
            if (i==10) {
                Vector a = {x + 4, y - 1};
                Vector b = {x - 4, y + 4 - 1};
                Vector c = {x + 4, y + 13 - 1};
                Vector d = {x - 4, y + 17 - 1};
                vec_draw_line(a, b, CYAN);
                vec_draw_line(b, c, CYAN);
                vec_draw_line(c, d, CYAN);
            }
    }
    press_tab(game);
}
void clear_main_screen(Game* game) {
    int i = 2;
    int w = game->size.x;
    int h = game->size.y;
    int a = (w / 2 - 78);
    int b = (h / 2 - 14);
    int c = (w / 2 + 78);
    int d = (h / 2 + 14);
    Vector e = {a, b};
    Vector f = {a, d};
    Vector g = {c, b};
    Vector j = {c, d};
    vec_draw_line(e, f, BLACK);
    vec_draw_line(e, g, BLACK);
    vec_draw_line(j, g, BLACK);
    vec_draw_line(j, f, BLACK);
    for (; i < 11; i++) {
        int x = (w / 2 - 78) + 13*i;
        int y = (h / 2 - 14) + 6;
        if (i==2) {
            Vector a = {x, y};
            Vector b = {x - 4, y + 17};
            Vector c = {x + 4, y + 17};
            Vector d = {x - 2, y + 9};
            Vector e = {x + 2, y + 9};
            vec_draw_line(a, b, BLACK);
            vec_draw_line(a, c, BLACK);
            vec_draw_line(d, e, BLACK);
        }
        if (i==3) {
            Vector a = {x + 4, y - 1};
            Vector b = {x - 4, y + 4 - 1};
            Vector c = {x + 4, y + 13 - 1};
            Vector d = {x - 4, y + 17 - 1};
            vec_draw_line(a, b, BLACK);
            vec_draw_line(b, c, BLACK);
            vec_draw_line(c, d, BLACK);
        }
        if (i==4) {
            Vector a = {x - 4, y};
            Vector b = {x + 4, y};
            Vector c = {x, y};
            Vector d = {x, y + 17};
            vec_draw_line(a, b, BLACK);
            vec_draw_line(c, d, BLACK);
        }
        if (i==5) {
            Vector a = {x - 4, y};
            Vector b = {x + 4, y};
            Vector c = {x - 4, y + 17};
            Vector d = {x + 4, y + 17};
            Vector e = {x - 4, y + 9};
            Vector f = {x + 4, y + 9};
            vec_draw_line(a, b, BLACK);
            vec_draw_line(a, c, BLACK);
            vec_draw_line(c, d, BLACK);
            vec_draw_line(e, f, BLACK);
        }
        if (i==6) {
            Vector a = {x - 4, y};
            Vector b = {x - 4, y + 17};
            Vector c = {x + 4, y + 4};
            Vector d = {x - 4, y + 9};
            Vector e = {x + 4, y + 17};
            vec_draw_line(a, b, BLACK);
            vec_draw_line(a, c, BLACK);
            vec_draw_line(c, d, BLACK);
            vec_draw_line(d, e, BLACK);
        }
        if (i==7) {
            Vector a = {x - 4, y};
            Vector b = {x + 4, y};
            Vector c = {x - 4, y + 17};
            Vector d = {x + 4, y + 17};
            vec_draw_line(a, b, BLACK);
            vec_draw_line(a, c, BLACK);
            vec_draw_line(c, d, BLACK);
            vec_draw_line(b, d, BLACK);
        }
        if (i==8) {
            Vector a = {x, y};
            Vector b = {x, y + 17};
            vec_draw_line(a, b, BLACK);
        }
        if (i==9) {
            Vector a = {x - 4, y};
            Vector b = {x - 4, y + 17};
            Vector c = {x + 4, y + 9};
            vec_draw_line(a, b, BLACK);
            vec_draw_line(a, c, BLACK);
            vec_draw_line(c, b, BLACK);
        }
        if (i==10) {
            Vector a = {x + 4, y - 1};
            Vector b = {x - 4, y + 4 - 1};
            Vector c = {x + 4, y + 13 - 1};
            Vector d = {x - 4, y + 17 - 1};
            vec_draw_line(a, b, BLACK);
            vec_draw_line(b, c, BLACK);
            vec_draw_line(c, d, BLACK);
        }
    }
}

void game_over(Game* game) {
    int i = 2;
    int w = game->size.x;
    int h = game->size.y;
    int a = (w / 2 - 78);
    int b = (w / 2 - 14 - 69);
    int c = (w / 2 + 78);
    int d = (w / 2 + 14 - 69);
    Vector e = {a, b};
    Vector f = {a, d};
    Vector g = {c, b};
    Vector j = {c, d};
    vec_draw_line(e, f, WHITE);
    vec_draw_line(e, g, WHITE);
    vec_draw_line(j, g, WHITE);
    vec_draw_line(j, f, WHITE);
    for (; i < 11; i++) {
        int x = (w / 2 - 78) + 13*i;
        int y = (w / 2 - 14) + 6 - 69;
        if (i==2) { //G
            Vector a = {x - 4, y};
            Vector b = {x + 4, y};
            Vector c = {x - 4, y + 17};
            Vector d = {x + 4, y + 17};
            Vector e = {x + 4, y + 8};
            Vector f = {x, y + 8};
            Vector g = {x + 4, y + 3};
            vec_draw_line(a, b, WHITE);
            vec_draw_line(a, c, WHITE);
            vec_draw_line(b, g, WHITE);
            vec_draw_line(c, d, WHITE);
            vec_draw_line(d, e, WHITE);
            vec_draw_line(e, f, WHITE);
        }
        if (i==3) { //A
            Vector a = {x, y};
            Vector b = {x - 4, y + 17};
            Vector c = {x + 4, y + 17};
            Vector d = {x - 2, y + 9};
            Vector e = {x + 2, y + 9};
            vec_draw_line(a, b, WHITE);
            vec_draw_line(a, c, WHITE);
            vec_draw_line(d, e, WHITE);
        }
        if (i==4) { //M
            Vector a = {x - 4, y};
            Vector b = {x - 4, y + 17};
            Vector c = {x + 4, y};
            Vector d = {x + 4, y + 17};
            Vector e = {x, y + 8};
            vec_draw_line(a, b, WHITE);
            vec_draw_line(c, d, WHITE);
            vec_draw_line(a, e, WHITE);
            vec_draw_line(c, e, WHITE);
        }
        if (i==5) { //E
            Vector a = {x - 4, y};
            Vector b = {x + 4, y};
            Vector c = {x - 4, y + 17};
            Vector d = {x + 4, y + 17};
            Vector e = {x - 4, y + 9};
            Vector f = {x + 4, y + 9};
            vec_draw_line(a, b, WHITE);
            vec_draw_line(a, c, WHITE);
            vec_draw_line(c, d, WHITE);
            vec_draw_line(e, f, WHITE);
        }
        if (i==7) { //O
            Vector a = {x - 4, y};
            Vector b = {x + 4, y};
            Vector c = {x - 4, y + 17};
            Vector d = {x + 4, y + 17};
            vec_draw_line(a, b, WHITE);
            vec_draw_line(a, c, WHITE);
            vec_draw_line(c, d, WHITE);
            vec_draw_line(b, d, WHITE);
        }
        if (i==8) { //V
            Vector a = {x - 4, y};
            Vector b = {x + 4, y};
            Vector c = {x, y + 17};
            vec_draw_line(a, c, WHITE);
            vec_draw_line(b, c, WHITE);
        }
        if (i==9) { //E
            Vector a = {x - 4, y};
            Vector b = {x + 4, y};
            Vector c = {x - 4, y + 17};
            Vector d = {x + 4, y + 17};
            Vector e = {x - 4, y + 9};
            Vector f = {x + 4, y + 9};
            vec_draw_line(a, b, WHITE);
            vec_draw_line(a, c, WHITE);
            vec_draw_line(c, d, WHITE);
            vec_draw_line(e, f, WHITE);
        }
        if (i==10) { //R
            Vector a = {x - 4, y};
            Vector b = {x - 4, y + 17};
            Vector c = {x + 4, y + 4};
            Vector d = {x - 4, y + 9};
            Vector e = {x + 4, y + 17};
            vec_draw_line(a, b, WHITE);
            vec_draw_line(a, c, WHITE);
            vec_draw_line(c, d, WHITE);
            vec_draw_line(d, e, WHITE);
        }
    }
    press_tab(game);
}
void clear_game_over(Game* game) {
    int i = 2;
    int w = game->size.x;
    int h = game->size.y;
    int a = (w / 2 - 78);
    int b = (w / 2 - 14);
    int c = (w / 2 + 78);
    int d = (w / 2 + 14);
    Vector e = {a, b};
    Vector f = {a, d};
    Vector g = {c, b};
    Vector j = {c, d};
    vec_draw_line(e, f, BLACK);
    vec_draw_line(e, g, BLACK);
    vec_draw_line(j, g, BLACK);
    vec_draw_line(j, f, BLACK);
    for (; i < 11; i++) {
        int x = (w / 2 - 78) + 13*i;
        int y = (w / 2 - 14) + 6;
        if (i==2) { //G
            Vector a = {x - 4, y};
            Vector b = {x + 4, y};
            Vector c = {x - 4, y + 17};
            Vector d = {x + 4, y + 17};
            Vector e = {x + 4, y + 8};
            Vector f = {x, y + 8};
            Vector g = {x + 4, y + 3};
            vec_draw_line(a, b, BLACK);
            vec_draw_line(a, c, BLACK);
            vec_draw_line(b, g, BLACK);
            vec_draw_line(c, d, BLACK);
            vec_draw_line(d, e, BLACK);
            vec_draw_line(e, f, BLACK);
        }
        if (i==3) { //A
            Vector a = {x, y};
            Vector b = {x - 4, y + 17};
            Vector c = {x + 4, y + 17};
            Vector d = {x - 2, y + 9};
            Vector e = {x + 2, y + 9};
            vec_draw_line(a, b, BLACK);
            vec_draw_line(a, c, BLACK);
            vec_draw_line(d, e, BLACK);
        }
        if (i==4) { //M
            Vector a = {x - 4, y};
            Vector b = {x - 4, y + 17};
            Vector c = {x + 4, y};
            Vector d = {x + 4, y + 17};
            Vector e = {x, y + 8};
            vec_draw_line(a, b, BLACK);
            vec_draw_line(c, d, BLACK);
            vec_draw_line(a, e, BLACK);
            vec_draw_line(c, e, BLACK);
        }
        if (i==5) { //E
            Vector a = {x - 4, y};
            Vector b = {x + 4, y};
            Vector c = {x - 4, y + 17};
            Vector d = {x + 4, y + 17};
            Vector e = {x - 4, y + 9};
            Vector f = {x + 4, y + 9};
            vec_draw_line(a, b, BLACK);
            vec_draw_line(a, c, BLACK);
            vec_draw_line(c, d, BLACK);
            vec_draw_line(e, f, BLACK);
        }
        if (i==7) { //O
            Vector a = {x - 4, y};
            Vector b = {x + 4, y};
            Vector c = {x - 4, y + 17};
            Vector d = {x + 4, y + 17};
            vec_draw_line(a, b, BLACK);
            vec_draw_line(a, c, BLACK);
            vec_draw_line(c, d, BLACK);
            vec_draw_line(b, d, BLACK);
        }
        if (i==8) { //V
            Vector a = {x - 4, y};
            Vector b = {x + 4, y};
            Vector c = {x, y + 17};
            vec_draw_line(a, c, BLACK);
            vec_draw_line(b, c, BLACK);
        }
        if (i==9) { //E
            Vector a = {x - 4, y};
            Vector b = {x + 4, y};
            Vector c = {x - 4, y + 17};
            Vector d = {x + 4, y + 17};
            Vector e = {x - 4, y + 9};
            Vector f = {x + 4, y + 9};
            vec_draw_line(a, b, BLACK);
            vec_draw_line(a, c, BLACK);
            vec_draw_line(c, d, BLACK);
            vec_draw_line(e, f, BLACK);
        }
        if (i==10) { //R
            Vector a = {x - 4, y};
            Vector b = {x - 4, y + 17};
            Vector c = {x + 4, y + 4};
            Vector d = {x - 4, y + 9};
            Vector e = {x + 4, y + 17};
            vec_draw_line(a, b, BLACK);
            vec_draw_line(a, c, BLACK);
            vec_draw_line(c, d, BLACK);
            vec_draw_line(d, e, BLACK);
        }
    }
}

void draw_number(int position, int number) {
    int x = 10 + 13*position;
    int y = 10;
    Vector a = {x - 4, y};
    Vector b = {x + 4, y};
    Vector c = {x - 4, y + 17};
    Vector d = {x + 4, y + 17};
    Vector e = {x - 4, y + 9};
    Vector f = {x + 4, y + 9};
    Vector g = {x, y};
    Vector h = {x, y + 17};
    if (number==0) { //0
        vec_draw_line(a, b, WHITE);
        vec_draw_line(a, c, WHITE);
        vec_draw_line(b, d, WHITE);
        vec_draw_line(c, d, WHITE);
    }
    if (number==1) { //1
        vec_draw_line(g, h, WHITE);
    }
    if (number==2) { //2
        vec_draw_line(a, b, WHITE);
        vec_draw_line(b, f, WHITE);
        vec_draw_line(e, f, WHITE);
        vec_draw_line(c, e, WHITE);
        vec_draw_line(c, d, WHITE);
    }
    if (number==3) { //3
        vec_draw_line(a, b, WHITE);
        vec_draw_line(b, d, WHITE);
        vec_draw_line(c, d, WHITE);
        vec_draw_line(e, f, WHITE);
    }
    if (number==4) { //4
        vec_draw_line(a, e, WHITE);
        vec_draw_line(e, f, WHITE);
        vec_draw_line(b, d, WHITE);
    }
    if (number==5) { //5
        vec_draw_line(a, b, WHITE);
        vec_draw_line(a, e, WHITE);
        vec_draw_line(e, f, WHITE);
        vec_draw_line(d, f, WHITE);
        vec_draw_line(c, d, WHITE);
    }
    if (number==6) { //6
        vec_draw_line(a, b, WHITE);
        vec_draw_line(a, c, WHITE);
        vec_draw_line(e, f, WHITE);
        vec_draw_line(d, f, WHITE);
        vec_draw_line(c, d, WHITE);
    }
    if (number==7) { //7
        vec_draw_line(a, b, WHITE);
        vec_draw_line(b, d, WHITE);
    }
    if (number==8) { //8
        vec_draw_line(a, b, WHITE);
        vec_draw_line(a, c, WHITE);
        vec_draw_line(e, f, WHITE);
        vec_draw_line(d, b, WHITE);
        vec_draw_line(c, d, WHITE);
    }
    if (number==9) { //9
        vec_draw_line(a, b, WHITE);
        vec_draw_line(b, d, WHITE);
        vec_draw_line(a, e, WHITE);
        vec_draw_line(e, f, WHITE);
    }
}

void clear_score(int position) {
    int x = 10 + 13*position;
    int y = 10;
    Vector a = {x - 4, y};
    Vector b = {x + 4, y};
    Vector c = {x - 4, y + 17};
    Vector d = {x + 4, y + 17};
    Vector e = {x - 4, y + 9};
    Vector f = {x + 4, y + 9};
    Vector g = {x, y};
    Vector h = {x, y + 17};
    vec_draw_line(a, b, BLACK);
    vec_draw_line(a, c, BLACK);
    vec_draw_line(e, f, BLACK);
    vec_draw_line(d, b, BLACK);
    vec_draw_line(c, d, BLACK);
    vec_draw_line(g, h, BLACK);
}

void draw_score(Game* game) {
    int a = game->score;
    for (int i = 5; i >= 0; i--) {
        int b = a % 10;
        a = a / 10;
        clear_score(i);
        draw_number(i, b);
    }
}

void press_tab(Game* game) {
    int i = 2;
    int w = SCREEN_SIZE.x;
    int h = SCREEN_SIZE.y;
    for (i; i < 11; i++) {
        int x = (w / 2 - 36) + 2 + 7*(i-1);
        int y = (w / 2 - 10) + 3 + 300;
        if (i==2) { //P
            Vector a = {x - 2, y};
            Vector b = {x + 2, y};
            Vector c = {x - 2, y + 9};
            Vector d = {x + 2, y + 9};
            Vector e = {x - 2, y + 4};
            Vector f = {x + 2, y + 4};
            vec_draw_line(a, b, WHITE);
            vec_draw_line(a, c, WHITE);
            vec_draw_line(b, f, WHITE);
            vec_draw_line(e, f, WHITE);
        }
        if (i==3) {//R
            Vector a = {x - 2, y};
            Vector b = {x - 2, y + 9};
            Vector c = {x + 2, y + 2};
            Vector d = {x - 2, y + 4};
            Vector e = {x + 2, y + 9};
            vec_draw_line(a, b, WHITE);
            vec_draw_line(a, c, WHITE);
            vec_draw_line(c, d, WHITE);
            vec_draw_line(d, e, WHITE);
        }
        if (i==4) { //E
            Vector a = {x - 2, y};
            Vector b = {x + 2, y};
            Vector c = {x - 2, y + 9};
            Vector d = {x + 2, y + 9};
            Vector e = {x - 2, y + 4};
            Vector f = {x + 2, y + 4};
            vec_draw_line(a, b, WHITE);
            vec_draw_line(a, c, WHITE);
            vec_draw_line(c, d, WHITE);
            vec_draw_line(e, f, WHITE);
        }
        if (i==5) { //S
            Vector a = {x - 2, y};
            Vector b = {x + 2, y};
            Vector c = {x - 2, y + 9};
            Vector d = {x + 2, y + 9};
            Vector e = {x - 2, y + 4};
            Vector f = {x + 2, y + 4};
            vec_draw_line(a, b, WHITE);
            vec_draw_line(a, e, WHITE);
            vec_draw_line(e, f, WHITE);
            vec_draw_line(d, f, WHITE);
            vec_draw_line(c, d, WHITE);
        }
        if (i==6) { //S
            Vector a = {x - 2, y};
            Vector b = {x + 2, y};
            Vector c = {x - 2, y + 9};
            Vector d = {x + 2, y + 9};
            Vector e = {x - 2, y + 4};
            Vector f = {x + 2, y + 4};
            vec_draw_line(a, b, WHITE);
            vec_draw_line(a, e, WHITE);
            vec_draw_line(e, f, WHITE);
            vec_draw_line(d, f, WHITE);
            vec_draw_line(c, d, WHITE);
        }
        if (i==8) {//T
            Vector a = {x - 2, y};
            Vector b = {x + 2, y};
            Vector c = {x, y};
            Vector d = {x, y + 9};
            vec_draw_line(a, b, MAGENTA);
            vec_draw_line(c, d, MAGENTA);
        }
        if (i==9) { //A
            Vector a = {x, y};
            Vector b = {x - 2, y + 9};
            Vector c = {x + 2, y + 9};
            Vector d = {x - 1, y + 4};
            Vector e = {x + 1, y + 4};
            vec_draw_line(a, b, MAGENTA);
            vec_draw_line(a, c, MAGENTA);
            vec_draw_line(d, e, MAGENTA);
        }
        if (i==10) { //B
            Vector a = {x - 2, y};
            Vector b = {x + 2, y};
            Vector c = {x - 2, y + 9};
            Vector d = {x + 2, y + 9};
            Vector e = {x - 2, y + 4};
            Vector f = {x + 2, y + 4};
            vec_draw_line(a, b, MAGENTA);
            vec_draw_line(a, c, MAGENTA);
            vec_draw_line(e, f, MAGENTA);
            vec_draw_line(d, b, MAGENTA);
            vec_draw_line(c, d, MAGENTA);
        }
    }
}

void draw_lives(Game* game) {
    int i = 0;
    Ship ship = {
        .thrusting = false, 
        #ifdef CPULATOR
        .angle = 0
        #else
        .angle = M_PI
        #endif 
    };
    for (; i < nLIVES + game->bonus_lives; i++) {
        Vector temp = {
            .x = 6 + SHIP_WIDTH * SHIP_SCALE * i,
            .y = 7
        };

        ship.position = vec_sub(game->size, temp);

        // draw the remaining lives in MAGENTA, the used lives in GREY
        if (i < game->lives) {
            draw_ship(&ship, MAGENTA);
        } else if (i < nLIVES) {
            draw_ship(&ship, GREY);
        } else {
            draw_ship(&ship, BLACK);
        }
    }
}

void draw_high_score(Game* game) {

}


void init_game(Game* game) {
    game->size = SCREEN_SIZE;
    reset_ship(game);
    //reset_alien(game);
    for (int i = 0; i < nSHIP_VERTICES_THRUST; i++) {
        game->player.old_vertices[i] = game->player.vertices[i];
    }
    for (int i = 0; i < nALIEN_VERTICES_THRUST; i++) {
        game->alien.old_vertices[i] = game->alien.vertices[i];
    }
    reset_alien(game);
    game->asteroidHead = NULL; 
    game->bulletHead = NULL;
    game->score = 0;
    game->level = 1;
    game->lives = nLIVES;
    game->bonus_lives = 0;
    game->running = true;

    // set the asteroid model
    int i = 0;
    for (; i < nASTEROID_VERTICES; i++) {
        float rad = (float)rand()/RAND_MAX * 0.4 + 0.8;
        float angle = (float)i * 2 * M_PI / nASTEROID_VERTICES;
        asteroidModel[i].x = rad * cos(angle);
        asteroidModel[i].y = rad * sin(angle);
    }

    // add asteroids
    add_random_asteroids(game, nASTEROIDS);
}

void reset_game(Game* game) {
    reset_keys();
    reset_ship(game);
    reset_alien(game);

    delete_asteroid_list(game);
    delete_bullet_list(game);
    game->score = 0;
    game->level = 1;
    game->lives = nLIVES;
    game->bonus_lives = 0;
}

void reset_ship(Game* game) {
    game->player.position = vec_mul(game->size, 0.5);
    game->player.velocity = new_vector();
    game->player.angle = 
    #ifdef CPULATOR
    0;
    #else
    M_PI;
    #endif
    transform_model(game->player.vertices, playerModel, nSHIP_VERTICES_THRUST, 
                    game->player.position, game->player.angle, SHIP_SCALE);
    game->player.thrusting = false;
}

void reset_alien(Game* game) {
    Vector a = {-100, -100};
    game->alien.position = vec_sub(rand_vec(game), a);

    //game->alien.position = rand_vec(game);
    alienpositiontemp = game->alien.position;

    game->alien.velocity = new_vector();
    game->alien.angle = M_PI;
    game->alien.radius = 20;
    game->alien.radius_squared = 400;
    game->alien.thrusting = false;
    game->alien.alive = true;
    transform_model(game->alien.vertices, alienModel, nALIEN_VERTICES_THRUST, game->alien.position, M_PI, game->alien.radius);
}

void reset_keys() {
    int i = 0;
    for (; i < nKEYS; i++) {
        keys[i].is_down = false;
    }
}

void update_game(Game* game) {
    update_asteroids(game);
    update_bullets(game);
    update_ship(&game->player);
    update_alien(&game->alien, game);
    draw_score(game);
}

void draw_game(Game *game) {
    draw_asteroids(game->asteroidHead);
    draw_bullets(game->bulletHead);
    draw_ship(&game->player, SHIP_COLOR);
    if (alientimer <= 0) {
        draw_alien(&game->alien, PINK);
    }
    draw_lives(game);
    draw_score(game);
}

void delay(float seconds) {
    clock_t start = clock();
    while ((clock() - start)/CLOCKS_PER_SEC < seconds);
}

#define DELAY 0.1

void update_pressed_keys() {
    volatile int* PS2_ptr = (int*)PS2_BASE;
    int PS2_data, RVALID, RAVAIL;
    char byte1, byte2, byte3;

    PS2_data = *(PS2_ptr); 
    RVALID = (PS2_data & 0x8000) >> 15; // extract the RVALID field
    RAVAIL = (PS2_data & 0xFFFF0000) >> 16; // get the number of bytes available

    for(; RAVAIL > -1 && RVALID; RAVAIL--) {
        // update input bytes
        byte1 = byte2;      
        byte2 = byte3;
        byte3 = PS2_data & 0xFF;

        for (int j = 0; j < nKEYS; j++) {
            if (byte3 == keys[j].code) {

                if (byte2 == 0xF0) {
                    keys[j].is_down = false;
                    #ifdef PRINT_KEYS
                    printf("released %x\n", keys[j].code);
                    #endif
                }
                else {
                    keys[j].is_down = true;
                    #ifdef PRINT_KEYS
                    printf("pressed %x\n", keys[j].code);
                    #endif
                }
            }
        }

        // update loop parameters
        PS2_data = *(PS2_ptr); 
        RVALID = (PS2_data & 0x8000) >> 15; // extract the RVALID field
    }
}

void shoot_bullet(Game* game) {
    if (b_cooldown > 0) return;

    // shoot bullet from the tip of the ship
    Vector p_front = game->player.vertices[2];
/*     Vector incr = rotate(vec_mul(NORTH, 20), game->player.angle);
    Vector b_pos = vec_add(p_front, incr); */
    Vector b_pos = vec_add(p_front, vec_sub(game->player.vertices[2], game->player.vertices[0]));
    Bullet *b = new_bullet(b_pos, game->player.angle);
    // b->velocity = vec_add(game->player.velocity, b->velocity);
    insert_bullet(game, b);

    // player recoil from shooting bullet
    Vector recoil = vec_mul(b->velocity, -0.01);
    game->player.position = vec_add(game->player.position, recoil);

    b_cooldown = BULLET_COOLDOWN;

    //audio code added here
//    //volatile int * red_LED_ptr = (int *)LEDR_BASE;
//    volatile int * audio_ptr = (int *) AUDIO_BASE;
//
//    /* used for audio record/playback */
//    int fifospace;
//    int buffer_index = 0; int left_buffer[BUF_SIZE];
//    int right_buffer[BUF_SIZE];
//
//    *(red_LED_ptr) = 0x2;
//    fifospace = *(audio_ptr + 1); // read the audio port fifospace register
//    if ((fifospace & 0x00FF0000) > BUF_THRESHOLD) // check WSRC
//    {   // output data until the buffer is empty or the audio-out FIFO // is full
//        while ((fifospace & 0x00FF0000) && (buffer_index < BUF_SIZE)) {
//
//            *(audio_ptr + 2) = left_buffer[buffer_index];
//            *(audio_ptr + 3) = right_buffer[buffer_index];
//            ++buffer_index;
//            fifospace = *(audio_ptr + 1); // read the audio port fifospace register
//        }
//    }
//
//    *(audio_ptr + 2) = (-1265); // audio_ptr points to the AUDIO_BASE
//    *(audio_ptr + 3) = (707);
}
double alienangle;
int anglecounter = 0;
void shoot_alien_bullet(Game* game) {
    if (alienbulletcooldown > 0) return;

    // shoot bullet from the tip of the ship
    //Bullet *b = new_bullet(game->alien.vertices[0], game->alien.angle);
    anglecounter++;
    if (anglecounter > 15) {anglecounter = 0;}
    alienangle = anglecounter * M_PI / 8;
    Vector c;
    c.x = game->alien.position.x;
    c.y = game->alien.position.y;

    if ((alienangle > M_PI / 2) || (alienangle <= 3 * M_PI / 2)) {c.y -= 40;}
    if ((alienangle > 3 * M_PI / 2) || (alienangle <= M_PI / 2)) {c.y += 65;}
//    if ((anglecounter >= 2) || (anglecounter < 6 )) {c.x -= 20;}
//    if ((anglecounter >= 6) || (anglecounter < 10 )) {c.y += 20;}
//    if ((anglecounter >= 10) || (anglecounter < 14 )) {c.x += 20;}
//    if ((anglecounter >= 14) || (anglecounter < 2 )) {c.y -= 20;}

    Bullet *b = new_bullet(c, alienangle);
    //b->velocity = vec_sub(game->player.velocity, b->velocity);

    insert_bullet(game, b);

    // player recoil from shooting bullet
//    Vector recoil = vec_mul(b->velocity, -0.01);
//    game->alien.position = vec_add(a, recoil);

    alienbulletcooldown = BULLET_COOLDOWN;


}

void handle_pressed_keys(Game* game) {
    game->player.thrusting = false;

    for (int i = 0; i < nKEYS; i++) {
        if (keys[i].is_down) {
            switch (keys[i].code) {
                case KEY_LEFT:
                    rotate_ship_left(&game->player);
                    break;
                case KEY_RIGHT:
                    rotate_ship_right(&game->player);
                    break;
                case KEY_UP:
                    game->player.thrusting = true;
                    break;
                case KEY_DOWN:  // HYPERSPACE f
                    game->player.position = rand_vec(game);
                    game->player.velocity = new_vector();
                    break;
                case KEY_SPACE:
                    shoot_bullet(game);
                    break;
                case KEY_ESC:
                    game->running = false;
                    break;
                case KEY_Q:
                    shoot_alien_bullet(game);
                    break;
                default:
                    break;
            }
        }
    }

    b_cooldown -= dt;
    alienbulletcooldown -= dt;
}

void insert_asteroid(Game* game, Asteroid* a){
    if(game->asteroidHead) 
        game->asteroidHead->prev = a;
    a->next = game->asteroidHead;
    game->asteroidHead = a;
}

void add_random_asteroids(Game* game, int num_asteroids) {
    int i;
    for (i = 0; i < num_asteroids; i++) {
        float size = (float)rand()/RAND_MAX * (ASTEROID_MAX_RADIUS - ASTEROID_MIN_RADIUS) + ASTEROID_MIN_RADIUS;
        // results in a speed proportional to the asteroids' size
        Vector speed = vec_mul(NORTH, 
                                ASTEROID_MIN_SPEED + 
                                ASTEROID_MIN_RADIUS / size * 
                                (ASTEROID_MAX_SPEED - ASTEROID_MIN_SPEED));    
        Asteroid *a = new_asteroid(
            rand_vec(game),
            rotate(speed, (float)rand()/RAND_MAX * 2 * M_PI),
            size
        );
        insert_asteroid(game, a);
    }
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
    // remove it from the linked list
    if (game->asteroidHead == a)
        game->asteroidHead = a->next;
    if (a->prev)
        a->prev->next = a->next;
    if (a->next)
        a->next->prev = a->prev;

    free(a);
}

void update_asteroids(Game* game) {
    if (game->asteroidHead == NULL) {
        game->level++;
        if (game->level > 0 && game->level % 3 == 0) {
            game->lives++;
            game->bonus_lives++;
        }
        add_random_asteroids(game, game->level * nASTEROIDS);
        return;
    }

    Asteroid *a = game->asteroidHead;
    for (; a != NULL; a = a->next) {
        a->position = vec_add(a->position, vec_mul(a->velocity, dt));
        a->position = wrap(game->size, a->position);

        if (check_collision(game, a)) {

            if (a->radius/2 > ASTEROID_MIN_RADIUS) 
                split_asteroid(game, a);
            else {
                #ifdef CLEAR_FAST
                a->dead = true;
                #else
                delete_asteroid(game, a);
                #endif
            }
        }
        check_collision_alien(game);
//        {
////            #ifdef CLEAR_FAST
////            #else
//            reset_alien(game);
////            #endif
//        }

        // printf("asteroid position: ");
        // printf("%f, %f\n", a->position.x, a->position.y);
        // printf("asteroid velocity: ");
        // printf("%f, %f\n", a->velocity.x, a->velocity.y);
    }
}

bool in_triangle(Vector p, Vector a, Vector b, Vector c) {
    Vector ac, ab, ap;
    ac = vec_sub(c, a);
    ab = vec_sub(b, a);
    ap = vec_sub(p, a);

    int cdotc, cdotb, cdotp, bdotb, bdotp;
    cdotc = ac.x * ac.x + ac.y * ac.y; //dot_product(ac, ac);
    cdotb = ac.x * ab.x + ac.y * ab.y; //dot_product(ac, ab);
    cdotp = ac.x * ap.x + ac.y * ap.y; //dot_product(ac, ap);
    bdotb = ab.x * ab.x + ab.y * ab.y; //dot_product(ab, ab);
    bdotp = ab.x * ap.x + ac.y * ap.y; //dot_product(ab, ap);

    float inv;
    float u, v;
    inv = 1.0f/ (cdotc * bdotb - cdotb * cdotb);
    u = (bdotb * cdotp - cdotb * bdotp) * inv;
    v = (cdotc * bdotp - cdotb * cdotp) * inv;

    return (u >= 0) && (v >= 0) && (u + v <= 1);
}

inline bool point_in_ship(Game* game, int num_vertices, Vector p) {
    return in_triangle(p, game->player.vertices[1], game->player.vertices[2], game->player.vertices[3]);
}

bool check_collision(Game* game, Asteroid* a) {

    // check collision with each bullet
    Bullet* b = game->bulletHead;
    for (; b != NULL; b = b->next) {
        if (b->alive && point_in_asteroid(a, nASTEROID_VERTICES, b->position)) {
            // delete bullet
            #ifdef CLEAR_FAST
            b->alive = false;
            #else
            delete_bullet(game, b);
            #endif
            // update score
            game->score += ASTEROID_MIN_SCORE * ASTEROID_MAX_RADIUS/a->radius;
            return true;
        }
        //check if bullet collided with ship
        if (b->alive && point_in_ship(game, nSHIP_VERTICES_THRUST, b->position)) {
            // delete bullet
            #ifdef CLEAR_FAST
            b->alive = false;
            #else
            delete_bullet(game, b);
            #endif
            // update score
            game->score += ASTEROID_MIN_SCORE * ASTEROID_MAX_RADIUS/a->radius;
            game->lives--;
            reset_ship(game);
            return true;
        }
    }

    // check collision with ship
    int i = 0;
    for (; i < nSHIP_VERTICES; i++) {
        if (point_in_asteroid(a, nASTEROID_VERTICES, game->player.vertices[i])) {
            // update lives
            game->lives--;
            // reset ship
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
                                ASTEROID_MIN_SPEED + 
                                ASTEROID_MIN_RADIUS/size * 
                                (ASTEROID_MAX_SPEED - ASTEROID_MIN_SPEED));
                                
        Asteroid *a_new = new_asteroid(
                a->position,
                rotate(speed, (float)rand()/RAND_MAX * 2 * M_PI),
                size
        );
        insert_asteroid(game, a_new);
    }

    #ifdef CLEAR_FAST
    a->dead = true;
    #else
    delete_asteroid(game, a);
    #endif
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

void insert_bullet(Game* game, Bullet* b){
    if(game->bulletHead)
        game->bulletHead->prev = b;
    b->next = game->bulletHead;
    game->bulletHead = b;
}

void delete_bullet(Game* game, Bullet* b) {
    if (game->bulletHead == b)
        game->bulletHead = b->next;
    if (b->prev)
        b->prev->next = b->next;
    if (b->next)
        b->next->prev = b->prev;
    free(b);
}

void delete_bullet_list(Game* game) {
    Bullet *b = game->bulletHead;
    while (b != NULL) {
        Bullet *next = b->next;
        free(b);
        b = next;
    }
    game->bulletHead = NULL;
}

void update_bullets(Game* game) {
    Bullet *b = game->bulletHead;
    for (; b != NULL; b = b->next) {
        b->position = vec_add(b->position, vec_mul(b->velocity, dt));
        if (!point_on_screen(b->position)) {
            #ifdef CLEAR_FAST
            b->alive = false;
            #else
            delete_bullet(game, b);
            #endif
        }
    }
}

//================== A L I E N ==================//

void rotate_alien_left(Alien *alien) {
    alien->angle -= SHIP_ROTATION_P_SEC * dt;
}

void rotate_alien_right(Alien *alien) {
    alien->angle += SHIP_ROTATION_P_SEC * dt;
}

void bound_alien_speed(Alien *alien) {
    if (alien->velocity.x > SHIP_MAX_SPEED)
        alien->velocity.x = SHIP_MAX_SPEED;
    else if (alien->velocity.x < -SHIP_MAX_SPEED)
        alien->velocity.x = -SHIP_MAX_SPEED;
    if (alien->velocity.y > SHIP_MAX_SPEED)
        alien->velocity.y = SHIP_MAX_SPEED;
    else if (alien->velocity.y < -SHIP_MAX_SPEED)
        alien->velocity.y = -SHIP_MAX_SPEED;
}

void accelerate_alien(Alien* alien) {
    alien->velocity = vec_add(alien->velocity,
                             vec_mul(rotate(NORTH, alien->angle),
                                     ALIEN_ACCELERATION * dt));
    bound_alien_speed(alien);
}

void update_alien(Alien *alien, Game* game) {
    if (alientimer > 0) {alientimer -= dt; return;}
    alien->alive = true;

    alien->velocity = new_vector();
    Vector a = vec_sub(game->player.position, alien->position);
    alien->angle = - M_PI / 2 - 5 * atan(sqrt(magnitude_squared(a)));
    alien->angle = atan(a.y / a.x);
    //alien->velocity = rand_vec(game);
    alienangle = M_PI;
    alien->velocity.x = -30; alien->velocity.y = 0;
    alien->position = vec_add(alien->position, vec_mul(alien->velocity, dt));
    alienpositiontemp = game->alien.position;
    alien->position = wrap(
            SCREEN_SIZE,
            alien->position
    );
    if (alienshootcooldown <= 0) {shoot_alien_bullet(game); alienshootcooldown = 1;}
//    alien->position = wrap(
//            SCREEN_SIZE,
//            vec_add(alien->position, vec_mul(rand_vec(game), (dt/2 * pow(-1, rand() % 4))))
//    );

    //alien->position.y = (double) alien->position.y / 3 - 200;
    // add some friction to the ship when it is not accelerating
    if (alien->thrusting)
        accelerate_alien(alien);

    alien->velocity = vec_mul(alien->velocity, (pow(1 - SHIP_FRICTION, dt)));
    alienbulletcooldown -= dt;
    alienshootcooldown -= dt;
#ifdef PRINT_ALIEN_VELOCITY
    printf("alien velocity: %f %f\n", alien->velocity.x, alien->velocity.y );
#endif

}

void draw_alien(Alien *alien, short int color) {
    clear_alien(alien);

    #ifdef CPULATOR
    double alienAngle = 0;
    #else
    double alienAngle = M_PI;
    #endif

    transform_model(alien->vertices, alienModel, nALIEN_VERTICES_THRUST, alien->position, alienAngle, ALIEN_SCALE);
    draw_model(alien->vertices, nALIEN_VERTICES, color);

    if (!alien->thrusting) return;

    // draw thruster
    draw_model(alien->vertices + 4, nALIEN_VERTICES, RED);
    draw_model(alien->vertices + 8, nALIEN_VERTICES, YELLOW);
}

inline bool point_in_alien1(Alien *alien, int num_vertices, Vector p)
{
// testing how much this affects the performance
for (int i = -1; i <= 1; ++i) {
for (int j = -1; j <= 1; ++j) {
Vector q = {p.x + i * SCREEN_SIZE.x, p.y + j * SCREEN_SIZE.y};
if (alien->radius_squared >= magnitude_squared(vec_sub(p, alien->position)))
return true;
}
}
return false;
// might want to do more precise collision detection later
}

inline bool point_in_alien(Alien* alien, int num_vertices, Vector p) {
// testing how much this affects the performance
//    for (int i = -1; i <= 1; ++i) {
//        for (int j = -1; j <= 1; ++j) {
//            Vector q = {p.x + i * SCREEN_SIZE.x, p.y + j * SCREEN_SIZE.y};
//            if (100 >= magnitude_squared(vec_sub(q, game->alien.position)))
//            return true;
//        }
//    }
    if (alien->radius_squared >= magnitude_squared(vec_sub(p, alien->position))) {return true;}
    else {return false;}
// might want to do more precise collision detection later
}

bool check_collision_alien(Game* game) {

    // check collision with each bullet
    Bullet* b = game->bulletHead;
    for (; b != NULL; b = b->next) {
//        if (b->alive && point_in_alien2(&game->alien, nALIEN_VERTICES, b->position)) {
          if (point_in_alien(&game->alien, nALIEN_VERTICES, b->position)) {
            // delete bullet
#ifdef CLEAR_FAST
            b->alive = false;
#else
            delete_bullet(game, b);

#endif
            // update score
            //draw_alien(&game->alien, BLACK);
            game->score += ASTEROID_MIN_SCORE * ASTEROID_MAX_RADIUS/game->alien.radius;
            alientimer = 10;
            reset_alien(game);
            return true;
        }
    }

    // check collision with ship
    int i = 0;
    for (; i < nSHIP_VERTICES; i++) {
        if (point_in_alien1(&game->alien, nALIEN_VERTICES, game->player.vertices[i])) {
            // update lives
            game->lives--;
            // reset ship
            reset_ship(game);
            //draw_alien(&game->alien, BLACK);
            return true;
        }
    }

    return false;
}

//================== R E N D E R I N G   &   G R A P H I C S ==================//

void swap_buffers() {
    volatile int * pixel_ctrl_ptr = (int *)0xFF203020;
    *pixel_ctrl_ptr = 1;
}

void wait_for_vsync()
{
    volatile int * pixel_ctrl_ptr = (int *)0xFF203020;
    *pixel_ctrl_ptr = 1;

    while (*(pixel_ctrl_ptr + 3) & 1); // wait until S bit is 0
}
void vec_plot_pixel(Vector v, short int line_color)
{
    plot_pixel(v.x, v.y, line_color);
}

void plot_pixel(int x, int y, short int line_color)
{
    // if pixel not in bounds, wrap around
    while (x < 0) x += RESOLUTION_X;
    x = x % RESOLUTION_X;
    while (y < 0) y += RESOLUTION_Y;
    y = y % RESOLUTION_Y;    

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



///////////////////////////////EXPERIMENTAL///////////////////////////////////

void copy_olds(Game* game) {
    Asteroid *a = game->asteroidHead;
    for (; a != NULL; a = a->next) {
        int i = 0;
        for (; i < nASTEROID_VERTICES; i++) {
            a->old_vertices[i] = a->vertices[i];
        }
        // wait until asteroid is dead to delete it
        if (a->dead) delete_asteroid(game, a);
    }

    int i = 0;
    for (; i < nSHIP_VERTICES_THRUST; i++) {
        game->player.old_vertices[i] = game->player.vertices[i];
    }
    for (i = 0; i < nALIEN_VERTICES_THRUST; i++) {
        game->alien.old_vertices[i] = game->alien.vertices[i];
    }

    Bullet *b = game->bulletHead;
    for (; b != NULL; b = b->next) {
        b->old_position = b->position;
        if (!b->alive) delete_bullet(game, b);
    }

}

void clear_player(Ship* ship) {
    draw_model(ship->old_vertices, nSHIP_VERTICES_THRUST, BLACK);
}

void clear_alien(Alien* alien) {
    draw_model(alien->old_vertices, nALIEN_VERTICES_THRUST, BLACK);
}

void clear_bullets(Bullet* b) {
    for (; b != NULL; b = b->next) {
        draw_bullet(b->old_position, BLACK);
    }
}

void clear_asteroids(Asteroid *a) {
    while (a != NULL) {
        draw_model(a->old_vertices, nASTEROID_VERTICES, BLACK);
        a = a->next;
    }
}

void clear_screen_fast(Game * g) {
    clear_asteroids(g->asteroidHead);
    clear_player(&g->player);
    clear_alien(&g->alien);
    clear_bullets(g->bulletHead);
    copy_olds(g);
}

///////////////////////////////END EXPERIMENTAL///////////////////////////////////

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
    int radius = 2 * ASTEROID_MAX_RADIUS;
    Vector p = {0, 0};
    p.x = rand() % RESOLUTION_X;
    while ((p.x <= radius + game->player.position.x && p.x >= game->player.position.x - radius) || !point_on_screen(p))
        p.x = rand() % RESOLUTION_X;
    {
        p.x = rand() % RESOLUTION_X;
    }
    p.y = rand() % RESOLUTION_Y;
    while ((p.y <= radius + game->player.position.y && p.y >= game->player.position.y - radius) || !point_on_screen(p))
    {
        p.y = rand() % RESOLUTION_Y;
    }
    // printf("rand_vec: %f, %f\n", p.x, p.y);
    return p;
}