#ifndef __COLOR_H__
#define __COLOR_H__

#include <stdint.h>

typedef struct
{
    float red;
    float green;
    float blue;
    float alpha;    
} color4f;

typedef struct
{
    float hue;
    float saturation;
    float value;
    float alpha;
} color_hsv;

// ----------------------------------------------------------------------------
static inline float lerp_float(float a, float b, float t)
{
    return (1.f - t) * a + t * b;
}

// ----------------------------------------------------------------------------
static inline uint32_t RGBA_from_color4f(color4f color)
{
    uint32_t result = ((uint32_t)(color.alpha * 255.f)) << 24;
    result |= ((uint32_t)(color.blue * 255.f)) << 16;
    result |= ((uint32_t)(color.green * 255.f)) << 8;
    result |= ((uint32_t)(color.red * 255.f));
    return result;
}

// ----------------------------------------------------------------------------
static inline color4f color4f_from_RGBA(uint32_t color)
{
    return (color4f) 
    {
        .blue = (float)((color>>16)&0xff) / 255.f,
        .green = (float)((color>>8)&0xff) / 255.f,
        .red = (float)((color)&0xff) / 255.f,
        .alpha = (float)((color>>24)&0xff) / 255.f
    };
}

// ----------------------------------------------------------------------------
static inline color4f lerp_color4f(color4f a, color4f b, float t)
{
    return (color4f)
    {
        .blue = lerp_float(a.blue, b.blue, t),
        .green = lerp_float(a.green, b.green, t),
        .red = lerp_float(a.red, b.red, t),
        .alpha = lerp_float(a.alpha, b.alpha, t)
    };
}

// ----------------------------------------------------------------------------
static inline int get_blue_from_RGBA(uint32_t color)
{
    return (color>>16)&0xff;
}

// ----------------------------------------------------------------------------
static inline int get_red_from_RGBA(uint32_t color)
{
    return color&0xff;
}

// ----------------------------------------------------------------------------
static inline int get_green_from_RGBA(uint32_t color)
{
    return (color>>8)&0xff;
}

// ----------------------------------------------------------------------------
static inline int get_alpha_from_RGBA(uint32_t color)
{
    return (color>>24)&0xff;
}

// ----------------------------------------------------------------------------
static inline uint32_t lerp_RGBA(uint32_t a, uint32_t b, int alpha)
{
    int inverse_alpha = 256 - alpha;
    int blue = (get_blue_from_RGBA(a) * inverse_alpha + get_blue_from_RGBA(b) * alpha) >> 8;
    int green = (get_green_from_RGBA(a) * inverse_alpha + get_green_from_RGBA(b) * alpha) >> 8;
    int red = (get_red_from_RGBA(a) * inverse_alpha + get_red_from_RGBA(b) * alpha) >> 8;

    uint32_t result = 0xff000000;

    result |= (uint32_t)red;
    result |= (uint32_t)green << 8;
    result |= (uint32_t)blue << 16;

    return result;
}

// ----------------------------------------------------------------------------
color4f color4f_from_HSV(color_hsv hsv);

// ----------------------------------------------------------------------------
color_hsv hsv_from_color4f(color4f color);

// ----------------------------------------------------------------------------
color4f color_lerp(color4f a, color4f b, float t);


#endif // __COLOR_H__
