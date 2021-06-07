#ifndef SPRING_H
#define SPRING_H

#include <cstddef>
#include <utility>

class SpringMassGrid;
class PointMass;

class Spring
{
private:
	SpringMassGrid const  * const owner;
    const float target_len;

public:

    PointMass* const p0;
    PointMass* const p1;


public:
    Spring( SpringMassGrid const * const owner, 
            PointMass* const p0, 
            PointMass* const p1);

    void Update(float dt);
};

#endif