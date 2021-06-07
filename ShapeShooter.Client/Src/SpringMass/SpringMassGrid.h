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
    const float STIFFNESS = 3.25f;
    const float DAMPING = 2.25f;
    const float INVERSE_MASS = 1.0f / 0.025f;

private:
    std::vector<std::vector<PointMass>> points;
    std::vector<Spring> springs;

    glm::vec2 dimensions;

    std::size_t rows;
    std::size_t cols;
    
public:

    SpringMassGrid(glm::vec2 dimensions, float spacing);

    void Update(float dt);

    void ApplyRadialForce(glm::vec3 pos, float force, float radius, glm::vec4 color = glm::vec4(1,1,1,1));

    void Render(ShapeRenderer* const sh);

    PointMass& GetPoint(const std::pair<std::size_t, std::size_t>& idx)
    {
        return points[idx.first][idx.second];
    }
};


#endif