#include <memory>
#include "world.h"
#include "camera.h"

static world *create_world()
{
    auto w = new world;

    // box
    w->add_aabb(vec3(-1, 3.9, -1), vec3(1, 6, 1), w->create_lightsrc(vec3(6., 6., 6.)));    // light
    w->add_aabb(vec3(-6, 4, -6), vec3(6, 6, 6), w->create_lambertian(vec3(.8, .8, .8)));    // top
    w->add_aabb(vec3(-6, -1, -6), vec3(6, 0, 6), w->create_lambertian(vec3(.8, .8, .8)));   // bottom
    w->add_aabb(vec3(-6, -1, -6), vec3(6, 10, -5), w->create_lambertian(vec3(.2, .8, .2))); // front
    w->add_aabb(vec3(-6, -1, 5), vec3(6, 10, 6), w->create_lambertian(vec3(.8, .8, .8)));   // back
    w->add_aabb(vec3(-4, -1, -6), vec3(-3, 10, 6), w->create_lambertian(vec3(.8, .2, .2))); // left
    w->add_aabb(vec3(3, -1, -6), vec3(4, 10, 6), w->create_lambertian(vec3(.2, .2, .8)));   // right

    // balls
    w->add_sphere(0.5, vec3(-2, 0.5, -1), w->create_dielectric(0.3));
    w->add_aabb(vec3(-1.5, 0, -3), vec3(-0.5, 2, -2), w->create_lambertian(vec3(.3, .3, .6)));
    w->add_aabb(vec3(1, 0, -1.5), vec3(2, 1, -0.5), w->create_lambertian(vec3(.5, .2, .4)));

    // env
    w->use_parallel_light(vec3(0, 0, 0), vec3(0, -1, 0));

    return w;
}

static camera *create_camera()
{
    return new camera(vec3(0, 2, 3), vec3(0, 2, -1), vec3(0, 1, 0), 4, 3, 3);
}