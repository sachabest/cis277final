#ifndef CHUNK_H
#define CHUNK_H

#include <openGL/drawable.h>
#include <scene/texture.h>

class Chunk : public Drawable
{
public:
    Chunk(QList<QList<QList<Texture>>> cells);
    Chunk(int height);
    ~Chunk();
    void create();
    QList<QList<QList<Texture>>> cells;
    int height;

private:
    int index_count;
    int vertex_count;
    QVector<glm::vec3> createChunkVertexPositions();
    QVector<glm::vec3> createChunkVertexNormals();
    QVector<GLuint> createChunkIndices();
};

#endif // CHUNK_H
