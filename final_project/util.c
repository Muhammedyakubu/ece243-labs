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

void draw_line_points(Box a, Box b, short int color)
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