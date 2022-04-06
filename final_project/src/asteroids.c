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

/* Constants for animation */

/* not sure these are needed anymore */
// #define NUM_ASTEROIDS 8 // max number of asteroids on the screen at once
// #define NUM_BULLETS 8 // max number of bullets on the screen at once
// #define FALSE 0
// #define TRUE 1



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

#define SHIP_ROTATION_SPEED M_PI/16 // fine tune later
#define nSHIP_VERTICES 4

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

//================== A S T E R O I D ==================//

// starting radius of asteroid in pixels
#define ASTEROID_SIZE 4 


//================== G A M E ==================//
 
Vector CENTER = {RESOLUTION_X/2, RESOLUTION_Y/2};

 typedef struct Game {

 } Game;


//================== R E N D E R I N G   &   G R A P H I C S ==================//

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



/******************************************************************************
 *                          P R I V A T E   D A T A                           *
 *****************************************************************************/

//================== G L O B A L S ==================//

volatile int pixel_buffer_start; // global variable

//================== M O D E L S ==================//
/* consider storing the models inside the game object */
Vector playerModel[] = 
{
    {0, -6},
    {4, 4},
    {0, 2},
    {-4, 4}
};


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

    volatile int* ps2_data = (int*)PS2_BASE;
    int key_pressed = KEY_NONE;

    Ship player = {
        .position = CENTER,
        .velocity = {0, 0},
        .angle = 0
    };

    while (1)
    {   
        clear_screen();
        
        transform_model(player.vertices, playerModel, nSHIP_VERTICES, player.position, player.angle, 2);
        draw_model(player.vertices, nSHIP_VERTICES, WHITE);

        /* Poll for input */
        key_pressed = (*ps2_data) & 0xff;  // get the last byte
        // printf("%d\n", key_pressed);
        key_pressed = KEY_RIGHT;

        if (key_pressed == KEY_RIGHT)
        {
            rotate_ship_right(&player);
        }
        else if (key_pressed == KEY_LEFT)
        {
            rotate_ship_left(&player);
        }

        /* Erase any boxes and lines that were drawn in the last iteration */
        // code for updating the locations of boxes (not shown)
        // update_locations();

        // code for drawing the boxes and lines (not shown)

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


//================== R E N D E R I N G   &   G R A P H I C S ==================//

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