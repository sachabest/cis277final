#include "cube.h"
#include <la.h>
#include <iostream>

static const int CUB_IDX_COUNT = 36;
static const int CUB_VERT_COUNT = 24;

glm::vec4 GetCubeNormal(const glm::vec4& P)
{
    int idx = 0;
    float val = -1;
    for(int i = 0; i < 3; i++){
        if(glm::abs(P[i]) > val){
            idx = i;
            val = glm::abs(P[i]);
        }
    }
    glm::vec4 N(0,0,0,0);
    N[idx] = glm::sign(P[idx]);
    return N;
}

//These are functions that are only defined in this cpp file. They're used for organizational purposes
//when filling the arrays used to hold the vertex and index data.
void createCubeVertexPositions(glm::vec3 (&cub_vert_pos)[CUB_VERT_COUNT])
{
    int idx = 0;
    //Front face
    //UR
    cub_vert_pos[idx++] = glm::vec3(0.5f, 0.5f, 0.5f);
    //LR
    cub_vert_pos[idx++] = glm::vec3(0.5f, -0.5f, 0.5f);
    //LL
    cub_vert_pos[idx++] = glm::vec3(-0.5f, -0.5f, 0.5f);
    //UL
    cub_vert_pos[idx++] = glm::vec3(-0.5f, 0.5f, 0.5f);

    //Right face
    //UR
    cub_vert_pos[idx++] = glm::vec3(0.5f, 0.5f, -0.5f);
    //LR
    cub_vert_pos[idx++] = glm::vec3(0.5f, -0.5f, -0.5f);
    //LL
    cub_vert_pos[idx++] = glm::vec3(0.5f, -0.5f, 0.5f);
    //UL
    cub_vert_pos[idx++] = glm::vec3(0.5f, 0.5f, 0.5f);

    //Left face
    //UR
    cub_vert_pos[idx++] = glm::vec3(-0.5f, 0.5f, 0.5f);
    //LR
    cub_vert_pos[idx++] = glm::vec3(-0.5f, -0.5f, 0.5f);
    //LL
    cub_vert_pos[idx++] = glm::vec3(-0.5f, -0.5f, -0.5f);
    //UL
    cub_vert_pos[idx++] = glm::vec3(-0.5f, 0.5f, -0.5f);

    //Back face
    //UR
    cub_vert_pos[idx++] = glm::vec3(-0.5f, 0.5f, -0.5f);
    //LR
    cub_vert_pos[idx++] = glm::vec3(-0.5f, -0.5f, -0.5f);
    //LL
    cub_vert_pos[idx++] = glm::vec3(0.5f, -0.5f, -0.5f);
    //UL
    cub_vert_pos[idx++] = glm::vec3(0.5f, 0.5f, -0.5f);

    //Top face
    //UR
    cub_vert_pos[idx++] = glm::vec3(0.5f, 0.5f, -0.5f);
    //LR
    cub_vert_pos[idx++] = glm::vec3(0.5f, 0.5f, 0.5f);
    //LL
    cub_vert_pos[idx++] = glm::vec3(-0.5f, 0.5f, 0.5f);
    //UL
    cub_vert_pos[idx++] = glm::vec3(-0.5f, 0.5f, -0.5f);

    //Bottom face
    //UR
    cub_vert_pos[idx++] = glm::vec3(0.5f, -0.5f, 0.5f);
    //LR
    cub_vert_pos[idx++] = glm::vec3(0.5f, -0.5f, -0.5f);
    //LL
    cub_vert_pos[idx++] = glm::vec3(-0.5f, -0.5f, -0.5f);
    //UL
    cub_vert_pos[idx++] = glm::vec3(-0.5f, -0.5f, 0.5f);
}


void createCubeVertexNormals(glm::vec3 (&cub_vert_nor)[CUB_VERT_COUNT])
{
    int idx = 0;
    //Front
    for(int i = 0; i < 4; i++){
        cub_vert_nor[idx++] = glm::vec3(0,0,1);
    }
    //Right
    for(int i = 0; i < 4; i++){
        cub_vert_nor[idx++] = glm::vec3(1,0,0);
    }
    //Left
    for(int i = 0; i < 4; i++){
        cub_vert_nor[idx++] = glm::vec3(-1,0,0);
    }
    //Back
    for(int i = 0; i < 4; i++){
        cub_vert_nor[idx++] = glm::vec3(0,0,-1);
    }
    //Top
    for(int i = 0; i < 4; i++){
        cub_vert_nor[idx++] = glm::vec3(0,1,0);
    }
    //Bottom
    for(int i = 0; i < 4; i++){
        cub_vert_nor[idx++] = glm::vec3(0,-1,0);
    }
}

void createCubeIndices(GLuint (&cub_idx)[CUB_IDX_COUNT])
{
    int idx = 0;
    for(int i = 0; i < 6; i++){
        cub_idx[idx++] = i*4;
        cub_idx[idx++] = i*4+1;
        cub_idx[idx++] = i*4+2;
        cub_idx[idx++] = i*4;
        cub_idx[idx++] = i*4+2;
        cub_idx[idx++] = i*4+3;
    }
}

void Cube::create()
{
    GLuint cub_idx[CUB_IDX_COUNT];
    glm::vec3 cub_vert_pos[CUB_VERT_COUNT];
    glm::vec3 cub_vert_nor[CUB_VERT_COUNT];
    glm::vec3 cub_vert_col[CUB_VERT_COUNT];

    createCubeVertexPositions(cub_vert_pos);
    createCubeVertexNormals(cub_vert_nor);
    createCubeIndices(cub_idx);

    for(int i = 0; i < CUB_VERT_COUNT; i++){
        cub_vert_col[i] = glm::vec3(0.2f, 1.0f, 0.6f);
    }

    count = CUB_IDX_COUNT;

    bufIdx.create();
    bufIdx.bind();
    bufIdx.setUsagePattern(QOpenGLBuffer::StaticDraw);
    bufIdx.allocate(cub_idx, CUB_IDX_COUNT * sizeof(GLuint));

    bufPos.create();
    bufPos.bind();
    bufPos.setUsagePattern(QOpenGLBuffer::StaticDraw);
    bufPos.allocate(cub_vert_pos,CUB_VERT_COUNT * sizeof(glm::vec3));

    bufNor.create();
    bufNor.bind();
    bufNor.setUsagePattern(QOpenGLBuffer::StaticDraw);
    bufNor.allocate(cub_vert_nor, CUB_VERT_COUNT * sizeof(glm::vec3));

    bufCol.create();
    bufCol.bind();
    bufCol.setUsagePattern(QOpenGLBuffer::StaticDraw);
    bufCol.allocate(cub_vert_col, CUB_VERT_COUNT * sizeof(glm::vec3));

}

bool Cube::nearlyEqual(float a, float b) {
    float epsilon = 0.000001;
    float abso = glm::abs(a-b);
    return (abso < epsilon);
}

//testing for intersection
bool Cube::intersect(Ray r, Transform cube_to_world) {
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
            return false;
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
            return true;
        }
        //NO INTERSECTION
        return false;
}




