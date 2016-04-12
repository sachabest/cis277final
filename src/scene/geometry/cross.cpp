#include "cross.h"

Cross::Cross()
{

}

//DRAWING the plus in the center of the screen
GLenum Cross::drawMode() {
   return GL_LINES;
}

void Cross::create() {
    std::vector<GLuint> idx;
    std::vector<glm::vec3> pos;
    std::vector<glm::vec4> col;

    //four indices 0-1; 2-3
    idx.push_back(0);
    idx.push_back(1);
    idx.push_back(2);
    idx.push_back(3);

    //vertical line 0-1; colored white
    pos.push_back(glm::vec3(0,-0.5,0));
    pos.push_back(glm::vec3(0,0.5,0));
    col.push_back(glm::vec4(1,1,1,1));
    col.push_back(glm::vec4(1,1,1,1));

    //horizontal line 2-3
    pos.push_back(glm::vec3(-0.5,0,0));
    pos.push_back(glm::vec3(0.5,0,0));
    col.push_back(glm::vec4(1,1,1,1));
    col.push_back(glm::vec4(1,1,1,1));

    count = idx.size();

    bufIdx.create();
    bufIdx.bind();
    bufIdx.setUsagePattern(QOpenGLBuffer::StaticDraw);
    bufIdx.allocate(idx.data(), idx.size() * sizeof(GLuint));


    bufPos.create();
    bufPos.bind();
    bufPos.setUsagePattern(QOpenGLBuffer::StaticDraw);
    bufPos.allocate(pos.data(), pos.size() * sizeof(glm::vec4));

    bufCol.create();
    bufCol.bind();
    bufCol.setUsagePattern(QOpenGLBuffer::StaticDraw);
    bufCol.allocate(col.data(), col.size() * sizeof(glm::vec4));
}
