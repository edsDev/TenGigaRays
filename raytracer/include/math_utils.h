#pragma once
#include <random>
#include <cmath>

class vec3
{
public:
    vec3()
        : vec3(0, 0, 0){};
    vec3(float e0, float e1, float e2)
    {
        e[0] = e0;
        e[1] = e1;
        e[2] = e2;
    }
    float r() const { return e[0]; }
    float g() const { return e[1]; }
    float b() const { return e[2]; }
    float x() const { return e[0]; }
    float y() const { return e[1]; }
    float z() const { return e[2]; }

    const vec3 &operator+() const { return *this; }
    vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); }
    float operator[](int i) const { return e[i]; }
    float &operator[](int i) { return e[i]; };

#define OVERLOAD_VEC3_ASSIGN_OP(OP)      \
    vec3 &operator OP(const vec3 &other) \
    {                                    \
        e[0] OP other.e[0];              \
        e[1] OP other.e[1];              \
        e[2] OP other.e[2];              \
        return *this;                    \
    }                                    \
    vec3 &operator OP(float x)           \
    {                                    \
        e[0] OP x;                       \
        e[1] OP x;                       \
        e[2] OP x;                       \
        return *this;                    \
    }

    OVERLOAD_VEC3_ASSIGN_OP(=)
    OVERLOAD_VEC3_ASSIGN_OP(+=)
    OVERLOAD_VEC3_ASSIGN_OP(-=)
    OVERLOAD_VEC3_ASSIGN_OP(*=)
    OVERLOAD_VEC3_ASSIGN_OP(/=)

#undef OVERLOAD_VEC3_ASSIGN_OP

    bool operator==(const vec3 &other) const
    {
        return e[0] == other.e[0] && e[1] == other.e[1] && e[2] == other.e[2];
    }
    bool operator!=(const vec3 &other) const
    {
        return !operator==(other);
    }

    float length() const
    {
        return sqrtf(e[0] * e[0] + e[1] * e[1] + e[2] * e[2]);
    }
    vec3 unit_vector() const
    {
        auto result = *this;
        //result *= 1.f / length();
        result /= length();
        return result;
    }

    float dot(const vec3 &other) const
    {
        return e[0] * other.e[0] + e[1] * other.e[1] + e[2] * other.e[2];
    }
    vec3 cross(const vec3 &other) const
    {
        return vec3(e[1] * other.e[2] - e[2] * other.e[1],
                    e[2] * other.e[0] - e[0] * other.e[2],
                    e[0] * other.e[1] - e[1] * other.e[0]);
    }

private:
    float e[3];
};

#define CONCAT_TOKEN(LHS, RHS) LHS##RHS
#define OVERLOAD_VEC3_CALC_OP(OP)                             \
    inline vec3 operator OP(const vec3 &lhs, const vec3 &rhs) \
    {                                                         \
        auto result = lhs;                                    \
        result CONCAT_TOKEN(OP, =) rhs;                       \
        return result;                                        \
    }                                                         \
    inline vec3 operator OP(float lhs, const vec3 &rhs)       \
    {                                                         \
        auto result = rhs;                                    \
        result CONCAT_TOKEN(OP, =) lhs;                       \
        return result;                                        \
    }                                                         \
    inline vec3 operator OP(const vec3 &lhs, float rhs)       \
    {                                                         \
        auto result = lhs;                                    \
        result CONCAT_TOKEN(OP, =) rhs;                       \
        return result;                                        \
    }

OVERLOAD_VEC3_CALC_OP(+);
OVERLOAD_VEC3_CALC_OP(-);
OVERLOAD_VEC3_CALC_OP(*);
OVERLOAD_VEC3_CALC_OP(/);

#undef CONCAT_TOKEN
#undef OVERLOAD_VEC3_CALC_OP

inline float dot(const vec3 &lhs, const vec3 &rhs)
{
    return lhs.dot(rhs);
}
inline vec3 cross(const vec3 &lhs, const vec3 &rhs)
{
    return lhs.cross(rhs);
}
inline float distance_manhattan(const vec3 &lhs, const vec3 &rhs)
{
    auto diff = lhs - rhs;
    return abs(diff[0]) + abs(diff[1]) + abs(diff[2]);
}
inline float distance_euclidean(const vec3 &lhs, const vec3 &rhs)
{
    return (lhs - rhs).length();
}

// Random Generation
//

inline float rand_real(float min, float max)
{
    static thread_local auto rnd = std::mt19937();

    auto dist = std::uniform_real_distribution<float>(min, max);
    return dist(rnd);
}

// uniform in [0, 1)
inline float rand_canonical()
{
    return rand_real(0.f, 1.f);
}

// uniform in [-1, 1)
inline float rand_mirror()
{
    return rand_real(-1.f, 1.f);
}

inline vec3 random_in_unit_sphere()
{
    vec3 p;
    do {
        p = vec3(rand_mirror(), rand_mirror(), rand_mirror());
    } while (dot(p, p) >= 1.0);
    return p;
}