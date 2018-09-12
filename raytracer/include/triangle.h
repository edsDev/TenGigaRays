#pragma once
#include "hitable.h"
#include "material.h"

class triangle : public hitable
{
public:
    triangle(vec3 a, vec3 b, vec3 c, material *mat)
        : a_(a), b_(b), c_(c), material_(mat)
    {
    }

    bool hit(const ray &r, float t_min, float t_max, hit_record &rec) const override
    {
		// calculate t for ray to hit the triangle plane
		// calculate intersect point p
		// test if p is inside triangle
    }

private:
    vec3 a_;
    vec3 b_;
    vec3 c_;
    material *material_;
};
