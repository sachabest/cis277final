#include "chunk.h"
#include <la.h>

// Takes in a 16x16x16 list of bools indicating whether the cell is occupied
Chunk::Chunk(QList<QList<QList<bool>>> cells) : cells(cells)
{

}

Chunk::Chunk()
{
    for (int x = 0; x < 16; x++) {
        QList<QList<bool>> Xs;
        for (int y = 0; y < 16; y++) {
            QList<bool> Ys;
            for (int z = 0; z < 16; z++) {
                Ys.append(y < z);
            }
            Xs.append(Ys);
        }
        cells.append(Xs);
    }
}

Chunk::~Chunk()
{

}

QVector<glm::vec3> Chunk::createChunkVertexPositions()
{
    QVector<glm::vec3> positions;
    for (int x = 0; x < cells.size(); x++) {
        for (int y = 0; y < cells.size(); y++) {
            for (int z = 0; z < cells.size(); z++) {
                if (cells[x][y][z]) {
                    // Top face
                    if (y == cells.size() - 1 || !cells[x][++y][z]) {
                        positions.append(glm::vec3(x+1, y+1, z)); // UR
                        positions.append(glm::vec3(x+1, y+1, z+1)); // LR
                        positions.append(glm::vec3(x, y+1, z+1)); // LL
                        positions.append(glm::vec3(x, y+1, z)); // UL
                    }
                    // Bottom face
                    if (y == 0 || !cells[x][--y][z]) {
                        positions.append(glm::vec3(x+1, y, z+1)); // UR
                        positions.append(glm::vec3(x+1, y, z)); // LR
                        positions.append(glm::vec3(x, y, z));       // LL
                        positions.append(glm::vec3(x, y, z+1)); // UL
                    }
                    // Left face
                    if (x == 0 || !cells[--x][y][z]) {
                        positions.append(glm::vec3(x, y+1, z+1)); // UR
                        positions.append(glm::vec3(x, y, z+1)); // LR
                        positions.append(glm::vec3(x, y, z));       // LL
                        positions.append(glm::vec3(x, y+1, z)); // UL
                    }
                    // Right face
                    if (x == cells.size() - 1 || !cells[++x][y][z]) {
                        positions.append(glm::vec3(x+1, y+1, z)); // UR
                        positions.append(glm::vec3(x+1, y, z)); // LR
                        positions.append(glm::vec3(x+1, y, z+1)); // LL
                        positions.append(glm::vec3(x+1, y+1, z+1));       // UL
                    }
                    // Front face
                    if (z == 0 || !cells[x][y][--z]) {
                        positions.append(glm::vec3(x+1, y+1, z+1));       // UR
                        positions.append(glm::vec3(x+1, y, z+1)); // LR
                        positions.append(glm::vec3(x, y, z+1)); // LL
                        positions.append(glm::vec3(x, y+1, z+1)); // UL
                    }
                    // Back face
                    if (z == cells.size() - 1 || !cells[x][y][++z]) {
                        positions.append(glm::vec3(x, y+1, z)); // UR
                        positions.append(glm::vec3(x, y, z));       // LR
                        positions.append(glm::vec3(x+1, y, z)); // LL
                        positions.append(glm::vec3(x+1, y+1, z)); // UL
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
                if (cells[x][y][z]) {
                    // Top face
                    if (z == cells.size() - 1 || !cells[x][++y][z]) {
                        for (int i = 0; i < 4; i++) {
                            normals.append(glm::vec3(0, 1, 0));
                        }
                    }
                    // Bottom face
                    if (z == 0 || !cells[x][--y][z]) {
                        for (int i = 0; i < 4; i++) {
                            normals.append(glm::vec3(0, -1, 0));
                        }
                    }
                    // Left face
                    if (x == 0 || !cells[--x][y][z]) {
                        for (int i = 0; i < 4; i++) {
                            normals.append(glm::vec3(-1, 0, 0));
                        }
                    }
                    // Right face
                    if (x == cells.size() - 1 || !cells[++x][y][z]) {
                        for (int i = 0; i < 4; i++) {
                            normals.append(glm::vec3(1, 0, 0));
                        }
                    }
                    // Front face
                    if (y == 0 || !cells[x][y][--z]) {
                        for (int i = 0; i < 4; i++) {
                            normals.append(glm::vec3(0, 0, 1));
                        }
                    }
                    // Back face
                    if (y == cells.size() - 1 || !cells[x][y][++z]) {
                        for (int i = 0; i < 4; i++) {
                            normals.append(glm::vec3(0, 0, -1));
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
    for(int i = 0; i < vertex_count/4; i++){
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

    for(int i = 0; i < vertex_count; i++){
        colors.append(glm::vec3(0.2f, 1.0f, 0.6f));
    }

    GLuint std_indices[index_count];
    glm::vec3 std_positions[vertex_count];
    glm::vec3 std_normals[vertex_count];
    glm::vec3 std_colors[vertex_count];

    for (int i = 0; i < positions.size(); i++) {
        std_positions[i] = positions[i];
        std_normals[i] = normals[i];
        std_colors[i] = colors[i];
    }

    for (int i = 0; i < indices.size(); i++) {
        std_indices[i] = indices[i];
    }

    count = index_count;

    bufIdx.create();
    bufIdx.bind();
    bufIdx.setUsagePattern(QOpenGLBuffer::StaticDraw);
    bufIdx.allocate(std_indices, index_count * sizeof(GLuint));

    bufPos.create();
    bufPos.bind();
    bufPos.setUsagePattern(QOpenGLBuffer::StaticDraw);
    bufPos.allocate(std_positions, vertex_count * sizeof(glm::vec3));

    bufNor.create();
    bufNor.bind();
    bufNor.setUsagePattern(QOpenGLBuffer::StaticDraw);
    bufNor.allocate(std_normals, vertex_count * sizeof(glm::vec3));

    bufCol.create();
    bufCol.bind();
    bufCol.setUsagePattern(QOpenGLBuffer::StaticDraw);
    bufCol.allocate(std_colors, vertex_count * sizeof(glm::vec3));
}
