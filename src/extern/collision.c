#include "collision.h"
#include <stdlib.h>
#include <assert.h>

//-----------------------------------------------------------------------------
#define AABB_LEFT   (1<<1)
#define AABB_RIGHT  (1<<2)
#define AABB_BOTTOM (1<<3)
#define AABB_TOP    (1<<4)

//-----------------------------------------------------------------------------
bool intersection_segment_line(segment seg, segment line, vec2* intersection_point)
{
    vec2 dirA = vec2_sub(seg.p1, seg.p0);
    vec2 dirB = vec2_sub(line.p1, line.p0);

    // make sure that lines aren't parallel
    if ( (dirA.y/dirA.x) != (dirB.y/dirB.x))
    {
        float denominator = dirA.x * dirB.y - dirA.y * dirB.x;
        if (denominator != 0.f)
        {
            vec2 delta = vec2_sub(seg.p0, line.p0);
            float r = (delta.y * dirB.x - delta.x * dirB.y) / denominator;

            // should be in the segment bounds
            if (r >= 0.f && r <= 1.f)
            {
                if (intersection_point != NULL)
                    *intersection_point = vec2_add(vec2_scale(dirA, r), seg.p0);
                
                return true;
            }
        }
    }	
    return false;
}

//-----------------------------------------------------------------------------
bool intersection_segment_segment(segment seg0, segment seg1, vec2* intersection_point)
{
    vec2 dirA = vec2_sub(seg0.p1, seg0.p0);
    vec2 dirB = vec2_sub(seg1.p1, seg1.p0);

    // make sure that lines aren't parallel
    if ( (dirA.y/dirA.x) != (dirB.y/dirB.x))
    {
        float denominator = dirA.x * dirB.y - dirA.y * dirB.x;
        if (denominator != 0.f)
        {
            vec2 delta = vec2_sub(seg0.p0, seg1.p0);
            float r = (delta.y * dirB.x - delta.x * dirB.y) / denominator;
            float s = (delta.y * dirA.x - delta.x * dirA.y) / denominator;

            // should be in the segment bounds
            if (r >= 0.f && r <= 1.f && s >= 0.f && s <= 1.f)
            {
                if (intersection_point != NULL)
                    *intersection_point = vec2_add(vec2_scale(dirA, r), seg0.p0);
                
                return true;
            }
        }
    }	
    return false;
}

//-----------------------------------------------------------------------------
bool intersection_aabb_circle(aabb box, vec2 center, float radius)
{
    vec2 nearest_point = vec2_clamp(center, box.min, box.max);
    float sq_distance = vec2_sq_distance(nearest_point, center);
    return sq_distance < radius*radius;
}

//-----------------------------------------------------------------------------
bool intersection_aabb_circle_info(aabb box, vec2 center, float radius, vec2* nearest_point, float* penetration)
{
    *nearest_point = vec2_clamp(center, box.min, box.max);
    *penetration = radius - vec2_distance(*nearest_point, center);
    return (*penetration > 0.f);
}

//-----------------------------------------------------------------------------
bool intersection_circle_circle(vec2 center0, float radius0, vec2 center1, float radius1)
{
    vec2 d = vec2_sub(center1, center0);
    float sq_distance = vec2_sq_length(d);
    float rA = radius0, rB = radius1;
    float radius = rA + rB;
    return (sq_distance < radius * radius);
}

//-----------------------------------------------------------------------------
bool test_point_triangle(vec2 p0, vec2 p1, vec2 p2, vec2 point)
{
    // from http://www.blackpawn.com/texts/pointinpoly/default.html

    vec2 edge0 = vec2_sub(p2, p0);
    vec2 edge1 = vec2_sub(p1, p0);
    vec2 edge2 = vec2_sub(point, p0);

    // compute dot products
    float dot00 = vec2_dot(edge0, edge0);
    float dot01 = vec2_dot(edge0, edge1);
    float dot02 = vec2_dot(edge0, edge2);
    float dot11 = vec2_dot(edge1, edge1);
    float dot12 = vec2_dot(edge1, edge2);

    // compute barycentric coordinates
    float invDenom = 1.f / (dot00 * dot11 - dot01 * dot01);
    float u = (dot11 * dot02 - dot01 * dot12) * invDenom;
    float v = (dot00 * dot12 - dot01 * dot02) * invDenom;

    // check if point is in triangle
    return (u > 0.f) && (v > 0.f) && (u + v < 1.f);
}

//-----------------------------------------------------------------------------
static inline int compute_outcode(aabb box, vec2 point)
{
    assert(box.min.x < box.max.x && box.min.y < box.max.y);
    
    int result = 0;    
    
    if (point.x < box.min.x)
        result |= AABB_LEFT;
    else if (point.x > box.max.x)
        result |= AABB_RIGHT;
    
    if (point.y < box.min.y)
        result |= AABB_TOP;
    else if (point.y > box.max.y)
        result |= AABB_BOTTOM;
    
    return result;
}

//-----------------------------------------------------------------------------
// based on https://en.wikipedia.org/wiki/Cohen%E2%80%93Sutherland_algorithm
bool intersect_aabb_segment(aabb box, segment seg)
{
    int outcode0 = compute_outcode(box, seg.p0);
    int outcode1 = compute_outcode(box, seg.p1);
    
    // one point in the aabb
    if (outcode0 == 0 || outcode1 == 0)
        return true;
    
    // both points share an outside zone
    if (outcode0&outcode1)
        return false;
        
    int sides = outcode0^outcode1;    
    
    if ((sides&AABB_LEFT) && intersection_segment_segment(seg, (segment){{box.min.x, box.min.y}, {box.min.x, box.max.y}}, NULL))
        return true;
         
    if ((sides&AABB_RIGHT) && intersection_segment_segment(seg, (segment){{box.max.x, box.min.y}, {box.max.x, box.max.y}}, NULL))
        return true;
     
    if ((sides&AABB_TOP) && intersection_segment_segment(seg, (segment){{box.min.x, box.min.y}, {box.max.x, box.min.y}}, NULL))
        return true;
     
    if ((sides&AABB_BOTTOM) && intersection_segment_segment(seg, (segment){{box.min.x, box.max.y}, {box.max.x, box.max.y}}, NULL))
        return true;
    
    return false;    
}


