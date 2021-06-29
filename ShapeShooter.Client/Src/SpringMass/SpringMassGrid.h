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
    const float STIFFNESS;
    const float DAMPING;
    const float INVERSE_MASS;

private:
    std::vector<std::vector<PointMass>> points;
    std::vector<Spring> springs;

    glm::vec2 dimensions;

    std::size_t rows;
    std::size_t cols;
    
    float accumulator;

public:

    SpringMassGrid(glm::vec2 dimensions, float spacing);

    void Update(float dt);

    void ApplyRadialForce(glm::vec3 pos, float force, float radius, glm::vec4 color = glm::vec4(1,1,1,1));

    void Render(ShapeRenderer* const sh);
};


#endif