#include "segment.h"

static const int SEG_IDX = 4;
static const int SEG_VERT = 3;
static const glm::vec4 firstColor = glm::vec4(1.0, 0, 0, 1);
static const glm::vec4 secondColor = glm::vec4(1.0, 1.0, 0, 1);
const glm::vec4 LineSeg::WHITE = glm::vec4(1, 1, 1, 1);
const glm::vec4 LineSeg::BLACK = glm::vec4(0, 0, 0, 1);
const glm::vec4 LineSeg::RED = glm::vec4(1, 0, 0, 1);
const glm::vec4 LineSeg::GREEN = glm::vec4(0, 1, 0, 1);
const glm::vec4 LineSeg::BLUE = glm::vec4(0, 0, 1, 1);

LineSeg::LineSeg() : Drawable()
{}

char *LineSeg::name() {
    return "Segment";
}


GLenum LineSeg::drawMode() {
    return GL_LINES;
}

void LineSeg::create() {}
void LineSeg::createSeg(glm::vec4 pos1, glm::vec4 pos2) {
    createSeg(pos1, pos2, firstColor, secondColor);
}

void LineSeg::createSeg(glm::vec4 pos1, glm::vec4 pos2, glm::vec4 col1, glm::vec4 col2) {
    GLuint sph_idx[SEG_IDX];
    glm::vec4 sph_vert_pos[SEG_VERT];
    glm::vec4 sph_vert_col[SEG_VERT];

    sph_vert_pos[0] = pos1;
    sph_vert_pos[1] = (pos1 + pos2) / 2.0f;
    sph_vert_pos[2] = pos2;

    sph_vert_col[0] = col1;
    sph_vert_col[1] = col2;
    sph_vert_col[2] = col2;

    sph_idx[0] = 0;
    sph_idx[1] = 2;
    sph_idx[2] = 1;
    sph_idx[3] = 2;

    count = SEG_IDX;

    bufIdx.create();
    bufIdx.bind();
    bufIdx.setUsagePattern(QOpenGLBuffer::StaticDraw);
    bufIdx.allocate(sph_idx, SEG_IDX * sizeof(GLuint));

    bufPos.create();
    bufPos.bind();
    bufPos.setUsagePattern(QOpenGLBuffer::StaticDraw);
    bufPos.allocate(sph_vert_pos, SEG_VERT * sizeof(glm::vec4));

    bufCol.create();
    bufCol.bind();
    bufCol.setUsagePattern(QOpenGLBuffer::StaticDraw);
    bufCol.allocate(sph_vert_col, SEG_VERT * sizeof(glm::vec4));
}
