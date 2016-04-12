#include "camera.h"

#include <la.h>
#include <iostream>


Camera::Camera():
    Camera(400, 400)
{
    look = glm::vec3(0,0,1);
    up = glm::vec3(0,1,0);
    right = glm::vec3(1,0,0);
}

Camera::Camera(unsigned int w, unsigned int h):
    Camera(w, h, glm::vec3(8,16,8), glm::vec3(64,2,64), glm::vec3(0,1,0))
{}

Camera::Camera(unsigned int w, unsigned int h, const glm::vec3 &e, const glm::vec3 &r, const glm::vec3 &worldUp):
    fovy(45),
    width(w),
    height(h),
    near_clip(0.1f),
    far_clip(1000),
    eye(e),
    ref(r),
    world_up(worldUp)
{
    RecomputeAttributes();
}

Camera::Camera(const Camera &c):
    fovy(c.fovy),
    width(c.width),
    height(c.height),
    near_clip(c.near_clip),
    far_clip(c.far_clip),
    aspect(c.aspect),
    eye(c.eye),
    ref(c.ref),
    look(c.look),
    up(c.up),
    right(c.right),
    world_up(c.world_up),
    V(c.V),
    H(c.H)
{}

void Camera::CopyAttributes(const Camera &c)
{
    fovy = c.fovy;
    near_clip = c.near_clip;
    far_clip = c.far_clip;
    eye = c.eye;
    ref = c.ref;
    look = c.look;
    up = c.up;
    right = c.right;
    width = c.width;
    height = c.height;
    aspect = c.aspect;
    V = c.V;
    H = c.H;
}

void Camera::RecomputeAttributes()
{
    look = glm::normalize(ref - eye);
    right = glm::normalize(glm::cross(look, world_up));
    up = glm::cross(right, look);

    float tan_fovy = tan(fovy/2 * DEG2RAD);
    float len = glm::length(ref - eye);
    aspect = width/height;
    V = up*len*tan_fovy;
    H = right*len*aspect*tan_fovy;
}

glm::mat4 Camera::getViewProj()
{
    return glm::perspective(fovy * DEG2RAD, width / (float)height, near_clip, far_clip) * glm::lookAt(eye, ref, up);
}

//glm::mat4 Camera::ViewMatrix()
//{
//    glm::vec4 r1 =
//    glm::mat4 O = ;
//}

//glm::mat4 Camera::PerspectiveProjectionMatrix()
//{
//    ;
//}

void Camera::RotateAboutUp(float deg)
{
    deg *= DEG2RAD;
    glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), deg, up);
    ref = ref - eye;
    ref = glm::vec3(rotation * glm::vec4(ref, 1));
    ref = ref + eye;
    RecomputeAttributes();
}
void Camera::RotateAboutRight(float deg)
{
    deg *= DEG2RAD;
    glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), deg, right);
    ref = ref - eye;
    ref = glm::vec3(rotation * glm::vec4(ref, 1));
    ref = ref + eye;
    RecomputeAttributes();
}

void Camera::TranslateAlongLook(float amt)
{
    glm::vec3 translation = look * amt;
    eye += translation;
    ref += translation;
}

void Camera::TranslateAlongRight(float amt)
{
    glm::vec3 translation = right * amt;
    eye += translation;
    ref += translation;
}
void Camera::TranslateAlongUp(float amt)
{
    glm::vec3 translation = up * amt;
    eye += translation;
    ref += translation;
}

//raycast for checking intersection
Ray Camera::raycast() {
    //assume center of screen is at (0,0,0); so x and y should both be 0
//    float ndc_x = 0.0;
//    float ndc_y = 0.0;

    //convert to 3d ray
//    float length = glm::length(ref-eye);
//    float alpha = fovy/2.0f;
//    glm::vec4 V = glm::normalize(up)*length*glm::tan(alpha);
//    glm::vec4 H = glm::normalize(right)*length*((float)width/height)*glm::tan(alpha);
//    glm::vec4 point = ref+ndc_x*H+ndc_y*V;
//    glm::vec3 ray_origin = glm::vec3(eye.x, eye.y, eye.z);
//    glm::vec4 ray_dir = point - eye;
//    glm::vec3 ray_direction = glm::normalize(glm::vec3(ray_dir.x, ray_dir.y, ray_dir.z));
//    Ray ray = Ray(ray_origin, ray_direction);
//    return ray;

    //casting from the center of the screen.......?
    glm::vec3 ray_origin = eye;
    glm::vec3 ray_direct = glm::normalize(ref-eye);
    Ray ray = Ray(ray_origin, ray_direct);
    return ray;

}





