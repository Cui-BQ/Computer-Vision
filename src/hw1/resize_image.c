#include <math.h>
#include "image.h"

float nn_interpolate(image im, float x, float y, int c)
{
    // TODO Fill in
    return get_pixel(im, round(x), round(y), c);
}

image nn_resize(image im, int w, int h)
{
    // TODO Fill in (also fix that first line)
    float srcW = im.w;
    float srcH = im.h;
    image dst = make_image(w,h,3);

    for (int c = 0; c < 3; c++){
        for (int dstX = 0; dstX < w; dstX++){
            float srcX = dstX * (srcW / w) + 0.5*(srcW / w - 1);
            for (int dstY = 0; dstY < h; dstY++){
                float srcY = dstY * (srcH / h) + 0.5*(srcH / h - 1);
                float dstValue = nn_interpolate(im, srcX, srcY, c);
                set_pixel(dst, dstX, dstY, c, dstValue);
            }
        }
    }
    return dst;
}

float bilinear_interpolate(image im, float x, float y, int c)
{
    // TODO
    int i = floor(x);
    float u = x - i;
    int j = floor(y);
    float v = y - j;

    float ij = get_pixel(im, i, j, c);
    float i1j = get_pixel(im, i+1, j, c);
    float ij1 = get_pixel(im, i, j+1, c);
    float i1j1 = get_pixel(im, i+1, j+1, c);

    return (1-u)*(1-v)*ij + (1-u)*v*ij1 + u*(1-v)*i1j + u*v*i1j1;
}

image bilinear_resize(image im, int w, int h)
{
    // TODO
    float srcW = im.w;
    float srcH = im.h;
    image dst = make_image(w,h,3);

    for (int c = 0; c < 3; c++){
        for (int dstX = 0; dstX < w; dstX++){
            float srcX = dstX * (srcW / w) + 0.5*(srcW / w - 1);
            for (int dstY = 0; dstY < h; dstY++){
                float srcY = dstY * (srcH / h) + 0.5*(srcH / h - 1);
                float dstValue = bilinear_interpolate(im, srcX, srcY, c);
                set_pixel(dst, dstX, dstY, c, dstValue);
            }
        }
    }
    return dst;
}

