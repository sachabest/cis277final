#include "chunk.h"
#include <la.h>

/**
 * @brief Chunk::Chunk Initializes all blocks to be EMPTY
 * @param height The height (y-value) of the chunk in worldspace
 */
Chunk::Chunk(int height) : height(height)
{
    for (int x = 0; x < 16; x++) {
        QList<QList<Texture>> Xs;
        for (int y = 0; y < 16; y++) {
            QList<Texture> Ys;
            for (int z = 0; z < 16; z++) {
                Ys.append(EMPTY);
            }
            Xs.append(Ys);
        }
        cells.append(Xs);
    }
}

Chunk::~Chunk()
{}

QVector<glm::vec3> Chunk::createChunkVertexPositions()
{
    QVector<glm::vec3> positions;
    for (int x = 0; x < cells.size(); x++) {
        for (int y = 0; y < cells.size(); y++) {
            for (int z = 0; z < cells.size(); z++) {
                if (cells[x][y][z] != EMPTY) {
                    // Front face
                    if (z == cells.size() - 1 || cells[x][y][z+1] == EMPTY) {
                        positions.append(glm::vec3(x+1, y+1, z+1));       // UR
                        positions.append(glm::vec3(x+1, y, z+1)); // LR
                        positions.append(glm::vec3(x, y, z+1)); // LL
                        positions.append(glm::vec3(x, y+1, z+1)); // UL
                    }
                    // Right face
                    if (x == cells.size() - 1 || cells[x+1][y][z] == EMPTY) {
                        positions.append(glm::vec3(x+1, y+1, z)); // UR
                        positions.append(glm::vec3(x+1, y, z)); // LR
                        positions.append(glm::vec3(x+1, y, z+1)); // LL
                        positions.append(glm::vec3(x+1, y+1, z+1));       // UL
                    }
                    // Left face
                    if (x == 0 || cells[x-1][y][z] == EMPTY) {
                        positions.append(glm::vec3(x, y+1, z+1)); // UR
                        positions.append(glm::vec3(x, y, z+1)); // LR
                        positions.append(glm::vec3(x, y, z));       // LL
                        positions.append(glm::vec3(x, y+1, z)); // UL
                    }
                    // Back face
                    if (z == 0 || cells[x][y][z-1] == EMPTY) {
                        positions.append(glm::vec3(x, y+1, z)); // UR
                        positions.append(glm::vec3(x, y, z));       // LR
                        positions.append(glm::vec3(x+1, y, z)); // LL
                        positions.append(glm::vec3(x+1, y+1, z)); // UL
                    }
                    // Top face
                    if (y == cells.size() - 1 || cells[x][y+1][z] == EMPTY) {
                        positions.append(glm::vec3(x+1, y+1, z)); // UR
                        positions.append(glm::vec3(x+1, y+1, z+1)); // LR
                        positions.append(glm::vec3(x, y+1, z+1)); // LL
                        positions.append(glm::vec3(x, y+1, z)); // UL
                    }
                    // Bottom face
                    if (y == 0 || cells[x][y-1][z] == EMPTY) {
                        positions.append(glm::vec3(x+1, y, z+1)); // UR
                        positions.append(glm::vec3(x+1, y, z)); // LR
                        positions.append(glm::vec3(x, y, z));       // LL
                        positions.append(glm::vec3(x, y, z+1)); // UL
                    }
                }
            }
        }
    }
    vertex_count = positions.size();
    return positions;
}

QVector<glm::vec3> Chunk::createChunkVertexNormals()
{
    QVector<glm::vec3> normals;
    for (int x = 0; x < cells.size(); x++) {
        for (int y = 0; y < cells.size(); y++) {
            for (int z = 0; z < cells.size(); z++) {
                if (cells[x][y][z] != EMPTY) {
                    // Front face
                    if (z == cells.size() - 1 || cells[x][y][z+1] == EMPTY) {
                        for (int i = 0; i < 4; i++) {
                            normals.append(glm::vec3(0, 0, 1));
                        }
                    }
                    // Right face
                    if (x == cells.size() - 1 || cells[x+1][y][z] == EMPTY) {
                        for (int i = 0; i < 4; i++) {
                            normals.append(glm::vec3(1, 0, 0));
                        }
                    }
                    // Left face
                    if (x == 0 || cells[x-1][y][z] == EMPTY) {
                        for (int i = 0; i < 4; i++) {
                            normals.append(glm::vec3(-1, 0, 0));
                        }
                    }
                    // Back face
                    if (z == 0 || cells[x][y][z-1] == EMPTY) {
                        for (int i = 0; i < 4; i++) {
                            normals.append(glm::vec3(0, 0, -1));
                        }
                    }
                    // Top face
                    if (y == cells.size() - 1 || cells[x][y+1][z] == EMPTY) {
                        for (int i = 0; i < 4; i++) {
                            normals.append(glm::vec3(0, 1, 0));
                        }
                    }
                    // Bottom face
                    if (y == 0 || cells[x][y-1][z] == EMPTY) {
                        for (int i = 0; i < 4; i++) {
                            normals.append(glm::vec3(0, -1, 0));
                        }
                    }
                }
            }
        }
    }
    return normals;
}

QVector<GLuint> Chunk::createChunkIndices()
{
    QVector<GLuint> indices;
    for (int i = 0; i < vertex_count/4; i++) {
        indices.append(i*4);
        indices.append(i*4+1);
        indices.append(i*4+2);
        indices.append(i*4);
        indices.append(i*4+2);
        indices.append(i*4+3);
    }
    index_count = indices.size();
    return indices;
}

void Chunk::create()
{
    QVector<glm::vec3> positions = createChunkVertexPositions();
    QVector<glm::vec3> normals = createChunkVertexNormals();
    QVector<GLuint> indices = createChunkIndices();
    QVector<glm::vec3> colors;

    const glm::vec3 color = glm::vec3(((float) rand()) / (float) RAND_MAX, ((float) rand()) / (float) RAND_MAX, ((float) rand()) / (float) RAND_MAX);

    for (int i = 0; i < vertex_count; i++) {
        colors.append(color);
    }

    count = index_count;

    bufIdx.create();
    bufIdx.bind();
    bufIdx.setUsagePattern(QOpenGLBuffer::StaticDraw);
    bufIdx.allocate(indices.data(), index_count * sizeof(GLuint));

    bufPos.create();
    bufPos.bind();
    bufPos.setUsagePattern(QOpenGLBuffer::StaticDraw);
    bufPos.allocate(positions.data(), vertex_count * sizeof(glm::vec3));

    bufNor.create();
    bufNor.bind();
    bufNor.setUsagePattern(QOpenGLBuffer::StaticDraw);
    bufNor.allocate(normals.data(), vertex_count * sizeof(glm::vec3));

    bufCol.create();
    bufCol.bind();
    bufCol.setUsagePattern(QOpenGLBuffer::StaticDraw);
    bufCol.allocate(colors.data(), vertex_count * sizeof(glm::vec3));
}
