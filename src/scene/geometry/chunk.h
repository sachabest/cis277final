#ifndef CHUNK_H
#define CHUNK_H

#include <openGL/drawable.h>
#include <scene/texture.h>
#include <iostream>
#include <QOpenGLTexture>


class Chunk : public Drawable
{

public:
    Chunk(QList<QList<QList<Texture>>> cells);
    Chunk(int height);
    QOpenGLTexture* texture;
    Chunk(QOpenGLTexture*);
    ~Chunk();
    void create();
    QList<QList<QList<Texture>>> cells;
    int height;


    //make a qimage -> do it in mygl and pass texture here; default to true


private:
    int index_count;
    int vertex_count;
    QVector<glm::vec2> uvs;
    QVector<glm::vec3> createChunkVertexPositions();
    QVector<glm::vec3> createChunkVertexNormals();
    QVector<GLuint> createChunkIndices();
};

#endif // CHUNK_H
