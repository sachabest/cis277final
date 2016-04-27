#include <scene/scene.h>
#include <scene/geometry/cube.h>
#include <scene/geometry/chunk.h>
#include <iostream>

#define MAX_TERRAIN_HEIGHT 6    // Fix dis; make # of y_chunks generated dependent on Perlin noise height
static const int SCENE_DIM = 80;
static const int TERRAIN_DIM = 80;

// Dimensions must be a multiple of 16
Scene::Scene() : dimensions(SCENE_DIM, SCENE_DIM, SCENE_DIM), terrain(TERRAIN_DIM, TERRAIN_DIM), num_chunks(SCENE_DIM/16), origin(glm::vec3(0, 0, 0))
{
    /* The base coordinate centers the origin (0,0) on the x-z plane in the octree
       You can generate a maximum of 32 chunks in either direction, and 64 chunks upward */
    this->octree = new OctNode(Point3(-WORLD_DIM/2, 0, -WORLD_DIM/2), WORLD_DIM);
}

void Scene::shift(int dx, int dy, int dz) {
    origin.x += dx;
    origin.y += dy;
    origin.z += dz;
    terrain.shift(dx, dz);
    CreateNewChunks();
}

void Scene::addVoxel(QSet<OctNode *> &set, Point3 &p) {
    OctNode *chunk = getContainingNode(p);
    if (!chunk || !chunk->chunk)
        return;
    Point3 localPoint = worldToChunk(p);
    set.insert(chunk);
    chunk->chunk->cells[localPoint.x][localPoint.y][localPoint.z] = WOOD;
}

void Scene::bresenham(const glm::vec4 &p1, const glm::vec4 &p2) {
    QSet<OctNode *> modifiedNodes;
    Point3 p(p1.x, p1.y, p1.z);
    int dx = p2.x - p1.x;
    int dy = p2.y - p1.y;
    int dz = p2.z - p1.z;
    int xDir = (dx < 0) ? -1 : 1;
    int l = abs(dx);
    int yDir = (dy < 0) ? -1 : 1;
    int m = abs(dy);
    int zDir = (dz < 0) ? -1 : 1;
    int n = abs(dz);
    int error_1, error_2;
    int dx_err = l << 1;
    int dy_err = m << 1;
    int dz_err = n << 1;
    if ((l >= m) && (l >= n)) {
        error_1 = dy_err - l;
        error_2 = dz_err - l;
        for (int i = 0; i < l; i++) {
            addVoxel(modifiedNodes, p);
            // assign to point here
            if (error_1 > 0) {
                p.y += yDir;
                error_1 -= dx_err;
            }
            if (error_2 > 0) {
                p.z += zDir;
                error_2 -= dx_err;
            }
            error_1 += dy_err;
            error_2 += dz_err;
            p.x += xDir;
        }
    } else if ((m >= l) && (m >= n)) {
        error_1 = dy_err - m;
        error_2 = dz_err - m;
        for (int i = 0; i < m; i++) {
            addVoxel(modifiedNodes, p);
            if (error_1 > 0) {
                p.x += xDir;
                error_1 -= dy_err;
            }
            if (error_2 > 0) {
                p.z += zDir;
                error_2 -= dy_err;
            }
            error_1 += dy_err;
            error_2 += dz_err;
            p.y += yDir;
        }
    } else {
        error_1 = dy_err - n;
        error_2 = dz_err - n;
        for (int i = 0; i < n; i++) {
            addVoxel(modifiedNodes, p);
            if (error_1 > 0) {
                p.y += yDir;
                error_1 -= dz_err;
            }
            if (error_2 > 0) {
                p.x += xDir;
                error_2 -= dz_err;
            }
            error_1 += dy_err;
            error_2 += dx_err;
            p.z += zDir;
        }
    }
    // assign to point here
    addVoxel(modifiedNodes, p);
    for (OctNode *node : modifiedNodes) {
        node->chunk->create();
    }
}

void Scene::voxelize(const QVector<LPair_t> &pairs, const Point3 &pt) {
    glm::mat4 worldTransform = glm::translate(glm::mat4(), glm::vec3(pt.x, pt.y, pt.z));
    for (LPair_t pair : pairs) {
        glm::mat4 newTransform = worldTransform * pair.t;
        if (pair.draw) {
            bresenham(worldTransform[3], newTransform[3]);
        }
        worldTransform = newTransform;
    }
}

/**
 * @brief Scene::parseImage - reads the RGB values of each pixel and finds the corresponding height
 * @param image - image to create heightmap from
 * @param eye - the point the image should be centered on
 */
void Scene::parseImage(QImage image, glm::vec3 eye) {
    int min_x = int(eye.x - image.width()/2);
    int min_z = int(eye.z - image.height()/2);
    for (int z = 0; z < image.height(); z++) {
        QRgb *line = (QRgb *) image.scanLine(z);
        for (int x = 0; x < image.width(); x++) {
            // line[x] has an individual pixel
            heightmap.insert(Point(min_x + x, min_z + z), qGray(line[x])/10.0);
            for (int y = 0; y < (MAX_TERRAIN_HEIGHT + 1) * 16; y += 16) {
                getContainingNode(Point3(min_x + x, y, min_z + z))->setChunk(nullptr);
            }
        }
    }
    CreateNewChunks();
}

Chunk* Scene::getContainingChunk(Point3 p) const {
    return getContainingNode(p)->chunk;
}

// Returns the leaf node containing the point
// Encapsulates recursively building out the octree as well
OctNode* Scene::getContainingNode(Point3 p) const
{
    Point3 p2(glm::floor(p.x/16), glm::floor(p.y/16), glm::floor(p.z/16));
    OctNode* node = octree->getContainingNode(p2);
    if (node->length > 1) {
        node = node->buildTree(p2);
    }
    return node;
}

// Converts a point from world space to its position in local chunk space
Point3 Scene::worldToChunk(Point3 p)
{
    return Point3(glm::floor(p.x - (16*glm::floor(p.x/16))), glm::floor(p.y - 16*glm::floor(p.y/16)), glm::floor(p.z - 16*glm::floor(p.z/16)));
}

bool Scene::isFilled(Point3 p)
{
    Chunk* chunk = getContainingChunk(p);
    if (!chunk) {   // Chunk doesn't exist
        return false;
    }
    Point3 p_chunk = worldToChunk(p);
    if (p_chunk.x < chunk->cells.size()) {
        QList<QList<Texture>> ypart = chunk->cells[p_chunk.x];
        if (p_chunk.y < ypart.size()) {
            QList<Texture> zpart = ypart[p_chunk.y];
            if (p_chunk.z < zpart.size()) {
                if (zpart[p_chunk.z] == EMPTY) {
                    return false;
                }
                else {
                    return true;
                }
            }
        }
    }
    return false;
}

// Called whenever the camera moves to a different chunk
void Scene::CreateNewChunks()
{
    for (int x_chunk = 0; x_chunk < num_chunks; x_chunk++) {
        for (int z_chunk = 0; z_chunk < num_chunks; z_chunk++) {
            Point3 p = Point3(x_chunk*16.0f + origin.x, 0, z_chunk*16.0f + origin.z);
            // Must generate a new chunk VBO because octree is empty at that point
            if (!getContainingNode(p)->chunk) {
                for (int y_chunk = 0; y_chunk < MAX_TERRAIN_HEIGHT; y_chunk++) {
                    Point3 p_y = Point3(p.x, y_chunk*16.0f, p.z);
                    Chunk* chunk = new Chunk(p_y.y);
                    for (int x = 0; x < 16; x++) {
                        for (int z = 0; z < 16; z++) {
                            int x_coord = x + x_chunk*16.0f;
                            int z_coord = z + z_chunk*16.0f;
                            float height = terrain.getBlock(x_coord / (float) dimensions[0],
                                                            z_coord / (float) dimensions[2]);
                            if (heightmap.contains(Point(x_coord + origin.x, z_coord + origin.z))) {
                                height = heightmap[Point(x_coord + origin.x, z_coord + origin.z)];
                            }
                            if (height < 1) {
                                height = 1.0f;
                            }
                            for (int y = y_chunk*16; y < height; y++) {
                                if (y >= (y_chunk+1)*16) {
                                    break;
                                }

                                //STONE
                                if (y < 7) {
                                    chunk->cells[x][y-y_chunk*16][z] = STONE;
                                }

                                //LAVA
                                else if (y >= 7 && y < 9) {
                                    chunk->cells[x][y-y_chunk*16][z] = LAVA;
                                }

                                //WATER
                                else if (y >= 9 && y < 12) {
                                    chunk->cells[x][y-y_chunk*16][z] = WATER;
                                }

                                //WOOD
                                else if (y == 12) {
                                    chunk->cells[x][y-y_chunk*16][z] = WOOD;
                                }

                                //GRASS
                                else if (y >= 13 && y < 20) {
                                    chunk->cells[x][y-y_chunk*16][z] = GRASS;
                                }

                                //WATER
                                else if (y == 20) {
                                    chunk->cells[x][y-y_chunk*16][z] = WATER;
                                }

                                //GRASS; y > 20
                                else {
                                    chunk->cells[x][y-y_chunk*16][z] = GRASS;
                                }
                            }
                        }
                    }
                    chunk->create();
                    OctNode* leaf = getContainingNode(p_y);
                    leaf->setChunk(chunk);
                }
            }
        }
    }
}
