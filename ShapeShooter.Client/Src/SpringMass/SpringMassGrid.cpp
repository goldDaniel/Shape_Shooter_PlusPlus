#include "SpringMassGrid.h"
#include <future>

SpringMassGrid::SpringMassGrid(glm::vec2 dimensions, float spacing)
    : dimensions(dimensions)
{
    this->rows = static_cast<int>(dimensions.x / spacing);
    this->cols = static_cast<int>(dimensions.y / spacing);

    for (std::size_t i = 0; i < rows + 1; i++)
    {
        points.emplace_back(std::vector<PointMass>());
        for (std::size_t j = 0; j < cols + 1; j++)
        {
            float inv_mass = INVERSE_MASS;
            if (i == 0 || i == rows || j == 0 || j == cols)
            {
                inv_mass = 0;
            }

            glm::vec3 pos = 
            {
                i * dimensions.x / rows - dimensions.x / 2.0f,
                j * dimensions.y / cols - dimensions.y / 2.0f,
                0,
            };

            points[i].emplace_back(PointMass(this, pos, inv_mass));
        }
    }

    for (std::size_t i = 0; i < points.size() - 1; i++)
    {
        for (std::size_t j = 0; j < points[i].size() - 1; j++)
        {
            springs.push_back(Spring(this, &points[i][j], &points[i+1][j]));
            springs.push_back(Spring(this, &points[i][j], &points[i][j+1]));
        }
    }
}

void SpringMassGrid::ApplyRadialForce(glm::vec3 position, float force, float radius, glm::vec4 color)
{
    for (auto& pointArr : points)
    {
        for (auto& p : pointArr)
        {
            if (p.pos.x < position.x + radius && p.pos.x > position.x - radius &&
                p.pos.y < position.y + radius && p.pos.y > position.y - radius)
            { 
                float dist = glm::distance(position, p.pos);
                if (dist < radius)
                {
                    glm::vec3 directional_force = p.pos - position;

                    directional_force = glm::normalize(directional_force);
                    directional_force *= force * (1.0f - dist / radius);

                    p.color = color;

                    p.accel += directional_force * p.inv_mass;
                }
            }
        }
    }
}

void SpringMassGrid::Update(float dt)
{
    for (auto& s : springs)
    {
        s.Update(dt);
    }

    std::vector<std::future<void>> futures;
    for (auto& arr : points)
    {
        futures.emplace_back(std::async(std::launch::async, [&arr, dt] 
        { 
            for (auto& p : arr)
            {
                p.Update(dt);
            }
        }));   
    }

    for (auto& f : futures)
    {
        f.wait();
    }
}

void SpringMassGrid::Render(ShapeRenderer* const sh)
{
    for (const auto& s : springs)
    {
        float a0 = glm::abs(s.p0->pos.z);
        float a1 = glm::abs(s.p1->pos.z);
        float a = glm::max(a0,a1);

        sh->Line(
            s.p0->pos,
            s.p1->pos,
            s.p0->color,
            s.p1->color
        );
    }
}