#include "ray.h"

Ray::Ray()
{
    origin = glm::vec3(0,0,0);
    direction = glm::vec3(0,0,0);
}

Ray::Ray(glm::vec3 ori, glm::vec3 dir) : origin(ori), direction(dir) {

}

const Ray Ray::GetTransformedCopy(glm::mat4 T) {
    //return new Ray(T*origin, T*direction);
    glm::vec4 point = glm::vec4(origin, 1);
    glm::vec4 dir = glm::vec4(direction, 0);
    point = T*point;
    dir = T*dir;
    return Ray(glm::vec3(point), glm::vec3(dir));
}
