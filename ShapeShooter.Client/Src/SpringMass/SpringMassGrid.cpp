#include "SpringMassGrid.h"
#include <future>

SpringMassGrid::SpringMassGrid(glm::vec2 dimensions, float spacing, float stiffness, float damping, float inverse_mass)
    : dimensions(dimensions),
      accumulator(0),
      spacing(spacing),
      stiffness(stiffness),
      damping(damping),
      inverse_mass(inverse_mass)
{
    rows = static_cast<std::size_t>(dimensions.x / spacing);
    cols = static_cast<std::size_t>(dimensions.y / spacing);

    for (std::size_t i = 0; i < rows + 1; i++)
    {
        points.emplace_back(std::vector<PointMass>());
        for (std::size_t j = 0; j < cols + 1; j++)
        {
            float inv_mass = (i == 0 || i == rows || j == 0 || j == cols) ? 0 : inverse_mass;

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

void SpringMassGrid::ApplyRadialForce(glm::vec3 position, float force, float radius)
{
    for (auto& pointArr : points)
    {
        for (auto& p : pointArr)
        {
            float dist = glm::distance(position, p.pos);
            if (dist < radius)
            {
                glm::vec3 directional_force = p.pos - position;

                directional_force = glm::normalize(directional_force);
                directional_force *= force * (1.0f - dist / radius);

                p.accel += directional_force * p.inv_mass;
            }
        }
    }
}

void SpringMassGrid::ApplyRadialForce(glm::vec3 position, float force, float radius, glm::vec4 color)
{
    for (auto& pointArr : points)
    {
        for (auto& p : pointArr)
        {
            float dist = glm::distance(position, p.pos);
            if (dist < radius)
            {
                glm::vec3 directional_force = p.pos - position;

                directional_force = glm::normalize(directional_force);
                directional_force *= force * (1.0f - dist / radius);

                p.accel += directional_force * p.inv_mass;
                p.color = color;
            }
        }
    }
}

void SpringMassGrid::Update(float dt)
{
    float step = 1.0f / 60.0f;
    accumulator += dt;
    while (accumulator >= step)
    {
        for (auto& s : springs)
        {
            s.Update(step);
        }

        std::vector<std::future<void>> futures;
        for (auto& arr : points)
        {
            futures.emplace_back(std::async(std::launch::async, [&arr, step]
            {
                for (auto& p : arr)
                {
                    p.Update(step);
                }
            }));
        }

        for (auto& f : futures)
        {
            f.wait();
        }

        accumulator -= step;
    }
}

void SpringMassGrid::Render(ShapeRenderer* const sh)
{
    sh->Line({ -dimensions.x / 2, -dimensions.y / 2, 0 },
             { -dimensions.x / 2,  dimensions.y / 2, 0 });

    sh->Line({ dimensions.x / 2, -dimensions.y / 2, 0 },
             { dimensions.x / 2,  dimensions.y / 2, 0 });

    sh->Line({ -dimensions.x / 2, -dimensions.y / 2, 0 },
             {  dimensions.x / 2,  -dimensions.y / 2, 0 });

    sh->Line({ -dimensions.x / 2, dimensions.y / 2, 0 },
             { dimensions.x / 2,  dimensions.y / 2, 0 });

    for (const auto& s : springs)
    {
        float a0 = glm::abs(s.p0->pos.z);
        float a1 = glm::abs(s.p1->pos.z);
        float a = glm::min(a0, a1);



        sh->Line( 
            s.p0->pos, 
            s.p1->pos,
            { s.p0->color.r, s.p0->color.g, s.p0->color.b, a }, 
            { s.p1->color.r, s.p1->color.g, s.p1->color.b, a }
        );
    }
}