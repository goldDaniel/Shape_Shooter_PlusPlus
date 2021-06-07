#include "PointMass.h"

#include "SpringMassGrid.h"

#include <glm/glm.hpp>

static float lerp(float v0, float v1, float t) 
{
    return v0 + t * (v1 - v0);
}

PointMass::PointMass(SpringMassGrid const * const owner, glm::vec3 pos, float inv_mass)
    : owner(owner), pos(pos), desired_pos(pos), inv_mass(inv_mass), accel(0), vel(0)
{
    color = glm::vec4(0, 0, 0, 0);
}


void PointMass::Update(float dt)
{
    //there is an invisible spring that connects the initial
    //position to where the point currently is, these are the
    //calculations for said spring. Otherwise the grid effect
    //doesn't come back fast enough, also maintains the general shape
    //of the grid
    float stiffness_scale = 1.0f / 16.0f;
     
    auto spring_force = -owner->STIFFNESS * stiffness_scale * (pos - desired_pos);
    auto damping_force = owner->DAMPING * vel;
    auto force = spring_force - damping_force;

    accel += force * inv_mass;

    vel += accel * dt;
    pos += vel * dt;

    pos.z = glm::abs(pos.z);
    color.a = pos.z;

    color.r = lerp(color.r, 0, dt);
    color.g = lerp(color.g, 0, dt);
    color.b = lerp(color.b, 0, dt);
    color.a = lerp(color.b, 0, dt);

    //not really accurate but it works
    vel *= owner->DAMPING * dt;
}
