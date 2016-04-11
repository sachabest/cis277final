#ifndef USER_H
#define USER_H

#include "camera.h"
#include "geometry/cube.h"

class User
{
public:
    User();
    User(Camera c);
    glm::vec3 center_position;
    float min_x;
    float min_y;
    float min_z;
    float max_x;
    float max_y;
    float max_z;

    float gravity = 9.81;
    void collision(Cube* c);
};

#endif // USER_H
