#include <scene/scene.h>
#include <scene/geometry/cube.h>
#include <scene/geometry/chunk.h>
#include <iostream>

#define MAX_TERRAIN_HEIGHT 3

static const int SCENE_DIM = 128;
static const int TERRAIN_DIM = 128;

// Dimensions must be a multiple of 16
Scene::Scene() : dimensions(SCENE_DIM, SCENE_DIM, SCENE_DIM), terrain(TERRAIN_DIM, TERRAIN_DIM), num_chunks(SCENE_DIM/16), origin(glm::vec3(0, 0, 0))
{
    this->octree = new OctNode(Point3(0, 0, 0), tree_length);
}

void Scene::shift(int dx, int dy, int dz) {
    origin.x += dx;
    origin.y += dy;
    origin.z += dz;
    terrain.shift(dx, dz);
    //    CreateScene();
    CreateNewChunks();
    findNearbyChunks();
}

void Scene::addVoxel(QSet<OctNode *> &set, Point3 &p) {
    OctNode *chunk = getContainingNode(p);
    Point3 localPoint = worldToChunk(p);
    set.insert(chunk);
    chunk->chunk->cells[localPoint.x][localPoint.y][localPoint.z] = true;
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

void Scene::voxelize(const QVector<LPair_t> &pairs) {
    glm::mat4 worldTransform;
    for (LPair_t pair : pairs) {
        glm::mat4 newTransform = worldTransform * pair.t;
        if (pair.draw) {
            bresenham(worldTransform[3], newTransform[3]);
        }
        worldTransform = newTransform;
    }
}

Chunk* Scene::getContainingChunk(Point3 p) const {
    Point3 p2(glm::floor(p.x/16), glm::floor(p.y/16), glm::floor(p.z/16));
    if (terrain.chunk_map.contains(p2)) {
        return terrain.chunk_map[p2];
    } else  {
        return nullptr;
    }
}

OctNode* Scene::getContainingNode(Point3 p)
{
    Point3 p2(glm::floor(p.x/16), glm::floor(p.y/16), glm::floor(p.z/16));
    return octree->getContainingNode(p2);
}

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
    return chunk->cells[p_chunk.x][p_chunk.y][p_chunk.z];
}

void Scene::CreateScene() {
    points.clear();
    for(int x = 0; x < dimensions.x; x++) {
        for(int z = 0; z < dimensions.z; z++) {
            float height = terrain.getBlock(x / (float) dimensions[0], z / (float) dimensions[2]);
            //            height *= MAX_TERRAIN_HEIGHT;
            for (int y = 0; y < height; y++) {
                points.append(Point3(origin.x + x, origin.y + y, origin.z + z));
            }
        }
    }
}

// Called when the scene is first rendered
void Scene::CreateChunkScene() {
    qDebug() << "Creating chunks";
    for (int x_chunk = 0; x_chunk < num_chunks; x_chunk++) {
        for (int z_chunk = 0; z_chunk < num_chunks; z_chunk++) {
            for (int y_chunk = 0; y_chunk < num_chunks; y_chunk++) {
                Chunk* chunk = new Chunk();
                for (int x = 0; x < 16; x++) {
                    for (int z = 0; z < 16; z++) {
                        float height = terrain.getBlock((x + x_chunk*16) / (float) dimensions[0],
                                (z + z_chunk*16) / (float) dimensions[2]);
                        for (int y = y_chunk*16; y <= height; y++) {
                            if (y >= (y_chunk+1)*16) {
                                break;
                            }
                            chunk->cells[x][y-y_chunk*16][z] = true;
                        }
                    }
                }
                chunk->create();
                Point3 p = Point3(x_chunk*16.0f, y_chunk*16.0f, z_chunk*16.0f);
                terrain.chunk_map.insert(p, chunk);
                chunk_points.append(p);
                OctNode* leaf = getContainingNode(p);
                leaf->chunk = chunk;
                qDebug() << "Set chunk at: ";
                qDebug() << QString::fromStdString(glm::to_string(p.toVec3()));
            }
        }
    }
}

// Called whenever the camera moves to a different chunk
void Scene::CreateNewChunks()
{
    for (int x_chunk = 0; x_chunk < num_chunks; x_chunk++) {
        for (int z_chunk = 0; z_chunk < num_chunks; z_chunk++) {
            Point3 p = Point3(x_chunk*16.0f + origin.x, 0, z_chunk*16.0f + origin.z);
            // Must generate a new chunk VBO
            if (!terrain.chunk_map.contains(p)) {
                for (int y_chunk = 0; y_chunk < num_chunks; y_chunk++) {
                    Chunk* chunk = new Chunk();
                    for (int x = 0; x < 16; x++) {
                        for (int z = 0; z < 16; z++) {
                            float height = terrain.getBlock((x + x_chunk*16.0f) / (float) dimensions[0],
                                    (z + z_chunk*16.0f) / (float) dimensions[2]);
                            if (height < 1) {
                                height = 1.0f;
                            }
                            for (int y = y_chunk*16; y < height; y++) {
                                if (y >= (y_chunk+1)*16) {
                                    break;
                                }
                                chunk->cells[x][y-y_chunk*16][z] = true;
                            }
                        }
                    }
                    chunk->create();
                    terrain.chunk_map.insert(Point3(p.x, y_chunk*16.0f, p.z), chunk);
                }
            }
        }
    }
}

// Called to populate myGL's set of chunks with the ones to be rendered
void Scene::findNearbyChunks()
{
    chunk_points.clear();
    //glm::vec3 eye = glm::vec3((origin.x + SCENE_DIM)/2, (origin.y + SCENE_DIM)/2, (origin.z + SCENE_DIM)/2);
    for (int x_chunk = 0; x_chunk < num_chunks; x_chunk++) {
        for (int z_chunk = 0; z_chunk < num_chunks; z_chunk++) {
            for (int y_chunk = 0; y_chunk < num_chunks; y_chunk++) {
                float x_coord = x_chunk*16.0f + origin.x;
                float y_coord = y_chunk*16.0f + origin.y;
                float z_coord = z_chunk*16.0f + origin.z;
                //if (glm::distance(eye, glm::vec3(x_coord, y_coord, z_coord)) <= 32*16) {
                Point3 p = Point3(x_coord, y_coord, z_coord);
                chunk_points.append(p);
                //}
            }
        }
    }
}
