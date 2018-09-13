#pragma once
#include "hitable.h"
#include "hitable_list.h"
#include "material.h"
#include "aabb.h"
#include <memory>
#include <deque>
#include <algorithm>

class world
{
public:
    world() = default;

    vec3 color(const ray &r, size_t depth)
    {
        vec3 k(1, 1, 1);
        ray next_ray = r;
        for (int depth = 0; depth < 10; ++depth) {
            hit_record rec;
            if (collection_.hit(next_ray, 0.001f, 999999.f, rec)) {
                vec3 light;
                if (rec.mat_ptr->emit(r, rec, light)) {
                    // light emitted
                    return k * light;
                }

                vec3 attenuation;
                ray scattered;
                if (rec.mat_ptr->scatter(next_ray, rec, attenuation, scattered)) {
					// light scattered
                    k *= attenuation;
                    next_ray = scattered;
                } else {
                    // light absorbed
                    return vec3();
                }
            } else {
				// no object hit
                return k*calc_env_light(next_ray);
			}
        }

		// drop current ray and generate a shadow ray on origin
		vec3 p = next_ray.origin();
        vec3 dir = distant_light_ ? -env_light_param_ : env_light_param_ - p;
        ray shadow(p, dir);

		hit_record rec;
        if (collection_.hit(shadow, 0.001f, 999999.f, rec)) {
            // exposed to global light source
			return k * calc_env_light(shadow);
        } else {
            // in shadow
            return vec3();
        }
    }

	vec3 calc_env_light(const ray& r)
	{
        if (distant_light_) {
            return env_light_color_ * std::max(0.f, dot(r.unit_direction(), -env_light_param_));
        } else {
            auto d = distance_euclidean(env_light_param_, r.origin());
            return env_light_color_ / (4.f * 3.141592 * d * d);
        }
	}

    // Factories
    //

    material *create_lightsrc(vec3 light)
    {
        return create_material<lightsrc>(light);
    }
    material *create_lambertian(vec3 color)
    {
        return create_material<lambertian>(color);
    }
    material *create_metal(const vec3 &color, float f)
    {
        return create_material<metal>(color, f);
    }
    material *create_dielectric(float f)
    {
        return create_material<dielectric>(f);
    }

    void add_sphere(float radius, vec3 center, material *mat)
    {
        add_hitable<sphere>(center, radius, mat);
    }
    void add_aabb(vec3 min, vec3 max, material *mat)
    {
        add_hitable<aabb>(min, max, mat);
    }

    void use_parallel_light(vec3 color, vec3 direction)
    {
        distant_light_   = true;
        env_light_color_ = color;
        env_light_param_ = direction.unit_vector();
    }
    void use_point_light(vec3 color, vec3 position)
    {
        distant_light_   = false;
        env_light_color_ = color;
        env_light_param_ = position;
    }

private:
    template <typename TMat, typename... TArgs>
    material *create_material(TArgs &&... args)
    {
        materials_.push_back(std::make_unique<TMat>(std::forward<TArgs>(args)...));
        return materials_.back().get();
    }
    template <typename TObj, typename... TArgs>
    void add_hitable(TArgs &&... args)
    {
        objects_.push_back(std::make_unique<TObj>(std::forward<TArgs>(args)...));
        collection_.add_item(objects_.back().get());
    }

    bool distant_light_ = true;
    vec3 env_light_color_;
    vec3 env_light_param_;
    hitable_list collection_;

    std::deque<std::unique_ptr<hitable>> objects_;
    std::deque<std::unique_ptr<material>> materials_;
};