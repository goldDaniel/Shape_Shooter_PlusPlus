#ifndef SPRING_MASS_GRID_H
#define SPRING_MASS_GRID_H

#include <Core/Core.h>
#include <Graphics/ShapeRenderer.h>
#include <utility>

#include "PointMass.h"
#include "Spring.h"

class SpringMassGrid
{
public:
    float stiffness;
    float damping;
    float inverse_mass;

    float spacing;
private:
    std::vector<std::vector<PointMass>> points;
    std::vector<Spring> springs;

    glm::vec2 dimensions;

    std::size_t rows;
    std::size_t cols;
    

    float accumulator;

public:

    SpringMassGrid(glm::vec2 dimensions, float spacing = 0.1f, float stiffness = 10.25f, float damping = 3.25f, float inverse_mass = 1.0f / 0.025f);

    void Update(float dt);

    void ApplyRadialForce(glm::vec3 pos, float force, float radius);
    void ApplyRadialForce(glm::vec3 pos, float force, float radius, glm::vec4 color);

    void Render(ShapeRenderer* const sh);
};


#endif