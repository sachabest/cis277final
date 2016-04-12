#include "point3.h"
#include "math.h"

bool fequal(float f1, float f2, float epsilon = 0.0001) {
    return fabs(f1 - f2) < epsilon;
}

Point3::Point3(float x, float y, float z) : x(x), y(y), z(z) {}

bool Point3::operator<(const Point3 &p) const {
    // x major
    if (fequal(this->x, p.x)) {
        if (fequal(this->y, p.y)) {
            return (fequal(this->z, p.z));
        } else {
            return this->y < p.y;
        }
    } else {
        return this->x < p.x;
    }
}

bool Point3::operator==(const Point3 &p) const {
    if (fequal(this->x, p.x) && fequal(this->y, p.y) && fequal(this->z, p.z)) {
        return true;
    }
    return false;
}


bool Point3::nearlyEqual(float a, float b) {
    float epsilon = 0.000001;
    float abso = glm::abs(a-b);
    return (abso < epsilon);
}

//testing for intersection
//return normals
glm::vec3 Point3::intersect(Ray r, Transform cube_to_world) {
    //cube_to_world is the transformation that takes from a cube's local space to world space
    glm::mat4 model = cube_to_world.T();
    glm::mat4 inverse = cube_to_world.invT();
    glm::mat4 inverse_transpose = cube_to_world.invTransT();

    //convert ray to the cube's local space
    Ray copyRay = r.GetTransformedCopy(inverse);

    //point and direction for the copy ray
    glm::vec3 R0 = copyRay.origin;
    glm::vec3 Rd = copyRay.direction;

    float t_near = -100000;
    float t_far = 100000;

    float x_min = -0.5;
        float x_max = 0.5;
        float y_min = -0.5;
        float y_max = 0.5;
        float z_min = -0.5;
        float z_max = 0.5;


        if ((Rd.x == 0 && (R0.x < x_min || R0.x > x_max)) ||
               (Rd.y == 0 && (R0.y < y_min || R0.y > y_max)) ||
                (Rd.z == 0 && (R0.z < z_min || R0.z > z_max))) {
            //return Intersection();
            //BAD NORMAL
            return glm::vec3(0,0,0);
        }

        float t_x0 = 0.0;
        float t_x1 = 0.0;
        float t_y0 = 0.0;
        float t_y1 = 0.0;
        float t_z0 = 0.0;
        float t_z1 = 0.0;

        //X Part
        if (!nearlyEqual(Rd.x, 0)) {
            t_x0 = (x_min - R0.x) / Rd.x;
            t_x1 = (x_max - R0.x) / Rd.x;
        }
        else {
            t_x0 = R0.x;
            t_x1 = R0.x;
        }

        if (t_x0 > t_x1) {
            float temp = t_x1;
            t_x1 = t_x0;
            t_x0 = temp;
        }

        if (t_x0 > t_near) {
            t_near = t_x0;
        }
        if (t_x1 < t_far) {
            t_far = t_x1;
        }

        //Y Part
        if (!nearlyEqual(Rd.y, 0)) {
            t_y0 = (y_min - R0.y) / Rd.y;
            t_y1 = (y_max - R0.y) / Rd.y;
        }
        else {
            t_y0 = R0.y;
            t_y1 = R0.y;
        }

        if (t_y0 > t_y1) {
            float temp = t_y1;
            t_y1 = t_y0;
            t_y0 = temp;
        }

        if (t_y0 > t_near) {
            t_near = t_y0;
        }
        if (t_y1 < t_far) {
            t_far = t_y1;
        }

        //Z Part
        if (!nearlyEqual(Rd.z, 0)) {
            t_z0 = (z_min - R0.z) / Rd.z;
            t_z1 = (z_max - R0.z) / Rd.z;
        }
        else {
            t_z0 = R0.z;
            t_z1 = R0.z;
        }

        if (t_z0 > t_z1) {
            float temp = t_z1;
            t_z1 = t_z0;
            t_z0 = temp;
        }

        if (t_z0 > t_near) {
            t_near = t_z0;
        }
        if (t_z1 < t_far) {
            t_far = t_z1;
        }


        if (t_near <= t_far) {
            glm::vec3 intersect_point = glm::vec3(R0.x + Rd.x*t_near,
                                                          R0.y + Rd.y*t_near,
                                                          R0.z + Rd.z*t_near);


            glm::vec3 normal;
            if (nearlyEqual(intersect_point.x, -0.5)) {
                normal = glm::vec3(-1,0,0);
            }
            else if (nearlyEqual(intersect_point.x, 0.5)) {
                normal = glm::vec3(1,0,0);
            }
            else if (nearlyEqual(intersect_point.y, -0.5)) {
                normal = glm::vec3(0,-1,0);
            }
            else if (nearlyEqual(intersect_point.y, 0.5)) {
                normal = glm::vec3(0,1,0);
            }
            else if (nearlyEqual(intersect_point.z, -0.5)) {
                normal = glm::vec3(0,0,-1);
            }
            else if (nearlyEqual(intersect_point.z, 0.5)) {
                normal = glm::vec3(0,0,1);
            }

            return normal;
        }
        //NO INTERSECTION
        return glm::vec3(0,0,0);
}
