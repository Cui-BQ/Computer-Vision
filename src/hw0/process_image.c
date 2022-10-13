#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "image.h"

float get_pixel(image im, int x, int y, int c)
{
    // TODO Fill this in
    if (x >= im.w) x = im.w - 1;
    if (x < 0) x = 0;
    if (y >= im.h) y = im.h - 1;
    if (y < 0) y = 0;
    if (c >= im.c) c = im.c - 1;
    if (c < 0) c = 0;
    int index = c * (im.w*im.h) + (y * im.w) + x;
    return im.data[index];
}

void set_pixel(image im, int x, int y, int c, float v)
{
    // TODO Fill this in
    if (x >= im.w) x = im.w - 1;
    if (x < 0) x = 0;
    if (y >= im.h) y = im.h - 1;
    if (y < 0) y = 0;
    if (c >= im.c) c = im.c - 1;
    if (c < 0) c = 0;
    int index = c * (im.w*im.h) + (y * im.w) + x;
    im.data[index] = v;
}

image copy_image(image im)
{
    image copy = make_image(im.w, im.h, im.c);
    // TODO Fill this in
    memcpy(copy.data, im.data, im.w * im.h * im.c * sizeof(float));
    return copy;
}

image rgb_to_grayscale(image im)
{
    assert(im.c == 3);
    image gray = make_image(im.w, im.h, 1);
    // TODO Fill this in
    for (int x = 0; x < im.w; x++){
        for (int y = 0; y < im.h; y++){
            float k = 0.299 * get_pixel(im, x, y, 0);
            k += 0.587 * get_pixel(im, x, y, 1);
            k += 0.114 * get_pixel(im, x, y, 2);
            set_pixel(gray, x, y, 0, k);
        }
    }
    return gray;
}

void shift_image(image im, int c, float v)
{
    // TODO Fill this in
    for (int x = 0; x < im.w; x++){
        for (int y = 0; y < im.h; y++){
            float current_pixel = get_pixel(im, x, y, c);
            set_pixel(im, x, y, c, current_pixel+v);
        }
    }
}


float clamp_pixel_values (float f){
    if (f > 1) f = 1;
    if (f < 0) f = 0;
    return f;
}

void clamp_image(image im)
{
    // TODO Fill this in
    for (int x = 0; x < im.w; x++){
        for (int y = 0; y < im.h; y++){
            set_pixel (im, x, y, 0, clamp_pixel_values(get_pixel(im, x, y, 0)));
            set_pixel (im, x, y, 1, clamp_pixel_values(get_pixel(im, x, y, 1)));
            set_pixel (im, x, y, 2, clamp_pixel_values(get_pixel(im, x, y, 2)));
        }
    }
}


// These might be handy
float three_way_max(float a, float b, float c)
{
    return (a > b) ? ( (a > c) ? a : c) : ( (b > c) ? b : c) ;
}

float three_way_min(float a, float b, float c)
{
    return (a < b) ? ( (a < c) ? a : c) : ( (b < c) ? b : c) ;
}

void rgb_to_hsv(image im)
{
    // TODO Fill this in
    for (int x = 0; x < im.w; x++){
        for (int y = 0; y < im.h; y++){
            float R = get_pixel(im, x, y, 0);
            float G = get_pixel(im, x, y, 1);
            float B = get_pixel(im, x, y, 2);
            float V = three_way_max(R, G, B);
            float m = three_way_min(R, G, B);
            float C = V - m;
            float S = 0;
            if (R != 0 || G != 0 || B != 0) S = C / V;
            float Hp = 0;
            if (V == R) Hp = (G-B)/C;
            if (V == G) Hp = (B-R)/C + 2;
            if (V == B) Hp = (R-G)/C + 4;
            float H = Hp/6;
            if (Hp < 0) H = Hp/6 + 1;
            if (C == 0) H = 0;
            set_pixel(im, x, y, 0, H);
            set_pixel(im, x, y, 1, S);
            set_pixel(im, x, y, 2, V);
        }
    }
}

void hsv_to_rgb(image im)
{
    // TODO Fill this in
    for (int x = 0; x < im.w; x++){
        for (int y = 0; y < im.h; y++){
            float H = get_pixel(im, x, y, 0);
            float S = get_pixel(im, x, y, 1);
            float V = get_pixel(im, x, y, 2);
            float Hp = 6*H;
            if (Hp > 5) Hp = 6*(H - 1);
            float C = S * V;
            float m = V - C;
            float R = 0;
            float B = 0;
            float G = 0;

            if (Hp <= 1) {
                R = V;
                B = m;
                G = B + Hp * C;
                if (Hp < 0){
                    G = m;
                    B = G - Hp * C;
                }
            } else if (Hp <= 3) {
                Hp -= 2;
                G = V;
                R = m;
                B = R + Hp * C;
                if (Hp < 0) {
                    B = m;
                    R = B - Hp * C;
                } 
            } else {
                Hp -= 4;
                B = V;
                G = m;
                R = G + Hp * C;
                if (Hp < 0) {
                    R = m;
                    G = R - Hp * C;
                }
            }
      
            set_pixel(im, x, y, 0, R);
            set_pixel(im, x, y, 1, G);
            set_pixel(im, x, y, 2, B);
        }
        
        
    }
}
