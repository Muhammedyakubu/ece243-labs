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

/* Constants for animation */
#define ASTEROID_SIZE 4 // side length of asteroid in pixels
#define NUM_ASTEROIDS 8 // max number of asteroids on the screen at once

#define FALSE 0
#define TRUE 1

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

////////////////// S T R U C T S //////////////////


typedef struct point
{
    int x, y;
} Point;

////////////////// F U N C T I O N    D E C L A R E S //////////////////

void wait_for_vsync();
void swap(int * a, int * b);
void plot_pixel(int x, int y, short int line_color);
void draw_line(int x0, int y0, int x1, int y1, short int color);
void clear_screen();
int get_rand_dir();

////////////////// G L O B A L S //////////////////

volatile int pixel_buffer_start; // global variable



int main(void)
{
    ////////////////// S E T U P //////////////////
    
    volatile int * pixel_ctrl_ptr = (int *)0xFF203020;
    // declare other variables(not shown)
    // initialize location and direction of rectangles(not shown)
    init();

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

    ////////////////// M A I N   L O O P //////////////////

    while (1)
    {
        /* Erase any boxes and lines that were drawn in the last iteration */
        // code for updating the locations of boxes (not shown)
        update_locations();

        // code for drawing the boxes and lines (not shown)

        wait_for_vsync(); // swap front and back buffers on VGA vertical sync
        pixel_buffer_start = *(pixel_ctrl_ptr + 1); // new back buffer
    }
    
    return 0;
}


void wait_for_vsync()
{
    volatile int * pixel_ctrl_ptr = (int *)0xFF203020;
    *pixel_ctrl_ptr = 1;

    while (*(pixel_ctrl_ptr + 3) & 1); // wait until S bit is 0
}

void swap(int * a, int * b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

void plot_pixel(int x, int y, short int line_color)
{
    // check that pixel is in bounds
    if (x < 0 || x >= RESOLUTION_X || y < 0 || y >= RESOLUTION_Y) 
        return;
    else 
        *(short int *)(pixel_buffer_start + (y << 10) + (x << 1)) = line_color;
}

int get_rand_dir()
{
    int num = rand() % 3 - 1;
    while (num == 0)
    {
        num = rand() % 3 - 1;
    }
    return num;
}

void draw_line_points(Point a, Point b, short int color)
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
    
    for (int x = x0; x < x1; ++x)
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
    for (int y = 0; y < RESOLUTION_Y; y++)
    {
        int x0 = 0, x1 = RESOLUTION_X ;
        draw_line(x0, y, x1, y, WHITE);
    }
}