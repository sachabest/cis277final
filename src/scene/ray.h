#ifndef RAY_H
#define RAY_H

#include <glm/glm.hpp>

class Ray
{
public:
    Ray();
    Ray(glm::vec3 ori, glm::vec3 dir);

    //origin is your eye
    glm::vec3 origin;
    glm::vec3 direction;
    const Ray GetTransformedCopy(glm::mat4 T);
};

#endif // RAY_H
