#ifndef SEGMENT_H
#define SEGMENT_H

#include "opengl/drawable.h"
#include <QOpenGLContext>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include "la.h"

class LineSeg : public Drawable {
public:
    LineSeg();
    virtual char *name();
    virtual GLenum drawMode();
    void createSeg(glm::vec4 pos1, glm::vec4 pos2);
    void createSeg(glm::vec4 pos1, glm::vec4 pos2, glm::vec4 col1, glm::vec4 col2);
    virtual void create();
    static const glm::vec4 WHITE;
    static const glm::vec4 BLACK;
    static const glm::vec4 RED;
    static const glm::vec4 GREEN;
    static const glm::vec4 BLUE;

};

#endif // SEGMENT_H
