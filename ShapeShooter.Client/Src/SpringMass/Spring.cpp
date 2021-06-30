#include "Spring.h"
#include "SpringMassGrid.h"

#include <glm/glm.hpp>

Spring::Spring( SpringMassGrid const  * const owner, 
                PointMass* const p0,
                PointMass* const p1) 
    :   owner(owner), 
	    p0(p0), 
	    p1(p1),
	    target_len(glm::distance(p0->pos, p1->pos))
{
}

void Spring::Update(float dt)
{
    auto diff = p1->pos - p0->pos;
    auto dst2 = glm::dot(diff, diff);

    if (dst2 > target_len * target_len)
    {
        auto dv = (p1->vel - p0->vel) * dt * owner->damping;
       
        auto force = (p0->pos - p1->pos) * owner->stiffness;
        force -= dv;

        p1->accel += force * p1->inv_mass;
        p0->accel -= force * p0->inv_mass;
    }
}