#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include "image.h"
#define TWOPI 6.2831853

void l1_normalize(image im)
{
    // TODO
    float total = 0;
    for (int x = 0; x < im.w; x++){
        for (int y = 0; y < im.h; y++){
            for (int c = 0; c < im.c; c++){
                total += get_pixel(im, x, y, c);
            }
        }
    }

    for (int x = 0; x < im.w; x++){
        for (int y = 0; y < im.h; y++){
            for (int c = 0; c < im.c; c++){
                set_pixel(im, x, y, c, (get_pixel(im, x, y, c) / total));
            }
        }
    }
}

image make_box_filter(int w)
{
    // TODO
    image res = make_image(w, w, 1);
    for (int x = 0; x < w; x++){
        for (int y = 0; y < w; y++){
            set_pixel(res, x, y, 0, 1.0 / (w*w));
        }
    }
    return res;
}

image convolve_image(image im, image filter, int preserve)
{
    // TODO
    assert(filter.c == im.c || filter.c == 1);
    image res = make_image(im.w, im.h, im.c);
    
    //if (preserve == 1 || 1 == 1){
        for (int x = 0; x < im.w; x++){
            for (int y = 0; y < im.h; y++){
                for (int c = 0; c < im.c; c++){
                    float resValue = 0;
                    for (int fx = 0; fx < filter.w; fx++){
                        for (int fy = 0; fy < filter.h; fy++){
                            //for (int fc = 0; fc < filter.c; fc++){
                                //if (x-filter.w/2+fx < im.w && x-filter.w/2+fx >= 0 && y-filter.h/2+fy < im.h && y-filter.h/2+fy >= 0){
                                    resValue += get_pixel(filter, fx, fy, c) * 
                                                get_pixel(im, x-filter.w/2+fx, y-filter.h/2+fy, c);
                                //}
                                
                            //}
                        }
                    }
                    set_pixel(res,x,y,c,resValue);
                }
            }
        }
        // if preserve is set to 1, produce an image with the same number of channels as the input.
        if (preserve == 1){
            return res;
        }
        
    //}
    
    // else, sum between channels
    image ress = make_image(im.w, im.h, 1);
    for (int x = 0; x < res.w; x++){
        for (int y = 0; y < res.h; y++){
            float v = 0;
            for (int c = 0; c < res.c; c++){
                v += get_pixel(res, x, y, c);
            }
            set_pixel(ress, x, y, 1, v);
        }
    }
    return ress;
    
}

image make_highpass_filter()
{
    // TODO
    image filter = make_image(3,3,1);
    float values[9] = {0, -1, 0, 
                      -1, 4, -1, 
                      0, -1, 0};
    memcpy(filter.data, values, 9 * sizeof(float));
    return filter;
}

image make_sharpen_filter()
{
    // TODO
    image filter = make_image(3,3,1);
    float values[9] = {0, -1, 0, 
                      -1, 5, -1, 
                      0, -1, 0};
    memcpy(filter.data, values, 9 * sizeof(float));
    return filter;
}

image make_emboss_filter()
{
    // TODO
    image filter = make_image(3,3,1);
    float values[9] = {-2, -1, 0, 
                      -1, 1, 1, 
                      0, 1, 2};
    memcpy(filter.data, values, 9 * sizeof(float));
    return filter;
}

// Question 2.2.1: Which of these filters should we use preserve when we run our convolution and which ones should we not? Why?
// Answer: sharpen filter and emboss filter should, and highpass not.

// Question 2.2.2: Do we have to do any post-processing for the above filters? Which ones and why?
// Answer: Yes, do clamp_image() for all filters.

image make_gaussian_filter(float sigma)
{
    // TODO
    int size = (int)ceil(6*sigma)%2 == 1 ? (int)ceil(6*sigma) : (int)ceil(6*sigma)+1;
    image filter = make_image(size, size, 1);
    // a = 1/(2*pi*sigma^2)
    float a = 1/(2*3.14159265358979323846*sigma*sigma);
    float b = -(2*sigma*sigma);
    for (int x = -(size/2); x <= size/2; x++){
        for (int y = -(size/2); y <= size/2; y++){
            float v = a * exp((x*x + y*y) / b);
            set_pixel(filter, x+(size/2), y+(size/2), 1, v);
        }
    }
    l1_normalize(filter);
    return filter;
}

image add_image(image a, image b)
{
    // TODO
    assert(a.c == b.c && a.h == b.h && a.w == b.w);

    image res = make_image(a.w, a.h, a.c);
    for (int x = 0; x < res.w; x++){
        for (int y = 0; y < res.h; y++){
            for (int c = 0; c < res.c; c++){
                float av = get_pixel(a, x, y ,c);
                float bv = get_pixel(b, x, y, c);
                set_pixel(res, x, y, c, av+bv);
            }
        }
    }
    return res;
}

image sub_image(image a, image b)
{
    // TODO
    assert(a.c == b.c && a.h == b.h && a.w == b.w);
     
    image res = make_image(a.w, a.h, a.c);
    for (int x = 0; x < res.w; x++){
        for (int y = 0; y < res.h; y++){
            for (int c = 0; c < res.c; c++){
                float av = get_pixel(a, x, y ,c);
                float bv = get_pixel(b, x, y, c);
                set_pixel(res, x, y, c, av-bv);
            }
        }
    }
    return res;
}

image make_gx_filter()
{
    // TODO
    image filter = make_image(3,3,1);
    float values[9] = {-1, 0, 1, 
                      -2, 0, 2, 
                      -1, 0, 1};
    memcpy(filter.data, values, 9 * sizeof(float));
    return filter;
}

image make_gy_filter()
{
    // TODO
    image filter = make_image(3,3,1);
    float values[9] = {-1, -2, -1, 
                      0, 0, 0, 
                      1, 2, 1};
    memcpy(filter.data, values, 9 * sizeof(float));
    return filter;
}

void feature_normalize(image im)
{
    // TODO
    float min = get_pixel(im, 0, 0, 0);
    float max = get_pixel(im, 0, 0, 0);;
    for (int x = 0; x < im.w; x++){
        for (int y = 0; y < im.h; y++){
            for (int c = 0; c < im.c; c++){
                min = min <= get_pixel(im, x, y, c)? min : get_pixel(im, x, y, c);
                max = max >= get_pixel(im, x, y, c)? max : get_pixel(im, x, y, c);
            }
        }
    }

    int zero = min == max? 1:0;
    
    for (int x = 0; x < im.w; x++){
        for (int y = 0; y < im.h; y++){
            for (int c = 0; c < im.c; c++){
                if (zero == 1){
                    set_pixel(im, x, y, c, 0);
                } else {
                    float v = get_pixel(im, x, y, c);
                    set_pixel(im, x, y, c, (v-min)/(max-min));
                }
                
            }
        }
    }
}

image *sobel_image(image im)
{
    // TODO
    image Gx = convolve_image(im, make_gx_filter(), 0);
    image Gy = convolve_image(im, make_gy_filter(), 0);
    image gradient = make_image(Gx.w, Gx.h, 1);
    image direction = make_image(im.w, im.h, 1);

    for (int x = 0; x < Gx.w; x++){
        for (int y = 0; y < Gx.h; y++){
            float gxValue = get_pixel(Gx, x, y, 1);
            float gyValue = get_pixel(Gy, x, y, 1);
            set_pixel(gradient, x, y, 0, sqrt(gxValue*gxValue + gyValue*gyValue));
            set_pixel(direction, x, y, 0, atan2(gyValue, gxValue));
        }
    }
    
    image *res = calloc(2, sizeof(image));
    res[0] = gradient;
    res[1] = direction;
    return res;
}

image colorize_sobel(image im)
{
    // TODO
    image *sobel = sobel_image(im);
    image res = make_image(im.w, im.h, 3);
    for (int x = 0; x < im.w; x++){
        for (int y = 0; y < im.h; y++){
            //for (int c = 0; c < 3; c++){
                set_pixel(res, x, y, 0, get_pixel(sobel[1], x, y, 0));
                set_pixel(res, x, y, 1, get_pixel(sobel[0], x, y, 0));
                set_pixel(res, x, y, 2, get_pixel(sobel[0], x, y, 0));
            //}
        }
    }
    return res;
}
