#pragma once
#include "ray.h"
#include "hitable.h"

float rand_()
{
    return (-1 + (float(rand()) / (float)RAND_MAX) * 2);
}

vec3 random_in_unit_sphere()
{
    vec3 p;
    do
    {
        p = vec3(rand_(), rand_(), rand_());
    } while (dot(p, p) >= 1.0);
    return p;
}

vec3 reflect(const vec3& in, const vec3& normal)
{
    return in - 2 * dot(in, normal) * normal;
}

class material
{
public:
    material(){};
    ~material(){};
    virtual bool scatter(const ray& r, const hit_record& rec,
                         vec3& attenuation, ray& scattered) const = 0;
};

class lambertian : public material
{
public:
    lambertian(const vec3& a)
        : albedo(a) {}
    virtual bool scatter(const ray& r, const hit_record& rec,
                         vec3& attenuation, ray& scattered) const
    {
        auto normal = rec.normal;
        scattered   = ray(rec.p, normal + random_in_unit_sphere());
        attenuation = albedo;
        return true;
    }
    vec3 albedo;
};

class metal : public material
{
public:
    metal(const vec3& a, float f)
        : albedo(a)
    {
        if (f < 1)
            fuzz = f;
        else
            fuzz = 1;
    }
    virtual bool scatter(const ray& r, const hit_record& rec,
                         vec3& attenuation, ray& scattered) const
    {
        auto normal = rec.normal;
        scattered   = ray(rec.p, reflect(r.direction(), rec.normal) +
                                   fuzz * random_in_unit_sphere());
        attenuation = albedo;
        return (dot(scattered.direction(), rec.normal)) > 0;
    }
    float fuzz;
    vec3 albedo;
};
