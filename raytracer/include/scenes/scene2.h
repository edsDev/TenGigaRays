#pragma once
#include "world.h"
#include "camera.h"

static world *create_world()
{
    auto w = new world;

    // ground
    w->add_sphere(1e5, vec3(0, -1e5, 0), w->create_lambertian(vec3(.8, .8, .8)));

    // balls
    w->add_sphere(0.5, vec3(-2, 0.5, -1), w->create_dielectric(0.3));
    w->add_aabb(vec3(-1.5, 0, -3), vec3(-0.5, 2, -2), w->create_lambertian(vec3(.3, .3, .6)));
    w->add_aabb(vec3(1, 0, -1.5), vec3(2, 1, -0.5), w->create_lambertian(vec3(.5, .2, .4)));

    // env
    w->use_parallel_light(vec3(3, 3, 3), vec3(5, -1, 0));

    return w;
}

static camera *create_camera()
{
    return new camera(vec3(0, 2, 3), vec3(0, 2, -1), vec3(0, 1, 0), 4, 3, 3);
}