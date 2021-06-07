#ifndef POINT_MASS_H
#define POINT_MASS_H

#include <glm/glm.hpp>
class SpringMassGrid;

class PointMass
{
private:
    SpringMassGrid const * const owner;

public:
    const glm::vec3 desired_pos;
    const float inv_mass;

    glm::vec3 pos;
    glm::vec3 vel;
    glm::vec3 accel;

    glm::vec4 color;

public:
    PointMass(SpringMassGrid const * const owner, glm::vec3 pos, float inv_mass);

    void Update(float dt);
};


#endif