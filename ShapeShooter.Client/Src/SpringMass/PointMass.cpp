#include "PointMass.h"

#include "SpringMassGrid.h"

#include <glm/glm.hpp>

template<typename T>
static T lerp(T v0, T v1, T t) 
{
    return v0 + t * (v1 - v0);
}

template<typename T>
static T min(T a, T b)
{
    return a < b ?  a : b;
}

template<typename T>
static T max(T a, T b)
{
    return a > b ? a : b;
}


glm::vec3 hsv2rgb(glm::vec3 c)
{
    glm::vec4 K = glm::vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    glm::vec3 p = glm::abs(glm::fract(glm::vec3(c.x + K.x, c.x + K.y, c.x + K.z)) * 6.0f - glm::vec3(K.w));
    return c.z * glm::mix(glm::vec3(K.x), glm::clamp(p - glm::vec3(K.x), 0.0f, 1.0f), c.y);
}

PointMass::PointMass(SpringMassGrid const * const owner, glm::vec3 pos, float inv_mass)
    : owner(owner), pos(pos), desired_pos(pos), inv_mass(inv_mass), accel(0), vel(0)
{
    color = glm::vec4(1,1,1,1);
}


void PointMass::Update(float dt)
{
    //there is an invisible spring that connects the initial
    //position to where the point currently is, these are the
    //calculations for said spring. Otherwise the grid effect
    //doesn't come back fast enough, also maintains the general shape
    //of the grid
    float stiffness_scale = 1.0f / 32.0f;
     
    auto spring_force = -owner->stiffness * stiffness_scale * (pos - desired_pos);
    auto damping_force = owner->damping * vel;
    auto force = spring_force - damping_force;

    accel += force * inv_mass;

    vel += accel * dt;
    pos += vel * dt;
    
    float rot = 0.5f + 0.5f * glm::abs(pos.z) / 25.0f;

    auto hsv = hsv2rgb({rot, 1.0f, 1.0f});
    color.r = hsv.r;
    color.g = hsv.g;
    color.b = hsv.b;
    color.a = 1.0f;

    //not really accurate but it works
    vel *= owner->damping * dt;
}
