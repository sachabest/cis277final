#include <scene/scene.h>
#include <scene/geometry/cube.h>
#include <scene/geometry/chunk.h>
#define MAX_TERRAIN_HEIGHT 3

static const int SCENE_DIM = 64;
static const int TERRAIN_DIM = 64;

// Dimensions must be a multiple of 16
Scene::Scene() : dimensions(SCENE_DIM, SCENE_DIM, SCENE_DIM), terrain(TERRAIN_DIM, TERRAIN_DIM), num_chunks(SCENE_DIM/16), origin(glm::vec3(SCENE_DIM/2, 0, SCENE_DIM/2))
{
}

void Scene::shift(int dx, int dy, int dz) {
    origin.x += dx;
    origin.y += dy;
    origin.z += dz;
    terrain.shift(dx, dz);
    CreateScene();
    CreateNewChunks();
    findNearbyChunks();
}

QList<Point3> Scene::voxelize(const QVector<LPair_t> &pairs) {
    glm::mat4 worldTransform;
    for (LPair_t pair : pairs) {
        worldTransform *= pair.t;
        if (pair.draw) {
            //glm::vec3 point = glm::vec4(worldTransform[3]);
            // carolina do something with this point plz
        }
    }
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

void Scene::CreateChunkScene() {
    qDebug() << "Creating chunks";
    for (int x_chunk = 0; x_chunk < num_chunks; x_chunk++) {
        for (int z_chunk = 0; z_chunk < num_chunks; z_chunk++) {
            for (int y_chunk = 0; y_chunk < num_chunks; y_chunk++) {
                Chunk* chunk = new Chunk();
                for (int x = 0; x < 16; x++) {
                    for (int z = 0; z < 16; z++) {
                        float height = terrain.getBlock((x + x_chunk*16) / (float) dimensions[0], (z + z_chunk*16) / (float) dimensions[2]);
                        for (int y = y_chunk*16; y <= height; y++) {
                            if (height >= (y_chunk+1)*16) {
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
            }
        }
    }
}

void Scene::CreateNewChunks()
{
    for (int x_chunk = 0; x_chunk < num_chunks; x_chunk++) {
        for (int z_chunk = 0; z_chunk < num_chunks; z_chunk++) {
            Point3 p = Point3((x_chunk-num_chunks/2)*16.0f + origin.x, 0, (z_chunk-num_chunks/2)*16.0f + origin.z);
            // Must generate a new chunk VBO
            if (!terrain.chunk_map.contains(p)) {
                for (int y_chunk = 0; y_chunk < num_chunks; y_chunk++) {
//                    qDebug() << QString::fromStdString(glm::to_string(p.toVec3()));
//                    qDebug() << "======";
                    Chunk* chunk = new Chunk();
                    for (int x = 0; x < 16; x++) {
                        for (int z = 0; z < 16; z++) {
                            float height = terrain.getBlock((x + (x_chunk-num_chunks/2)*16.0f + origin.x) / (float) dimensions[0], (z + (z_chunk-num_chunks/2)*16.0f + origin.z) / (float) dimensions[2]);
                            if (height < 1) {
                                height = 1.0f;
                            }
//                            qDebug() << height;
                            for (int y = y_chunk*16; y < height; y++) {
                                if (height >= (y_chunk+1)*16) {
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

void Scene::findNearbyChunks()
{
    chunk_points.clear();
    for (int x_chunk = 0; x_chunk < num_chunks; x_chunk++) {
        for (int z_chunk = 0; z_chunk < num_chunks; z_chunk++) {
            for (int y_chunk = 0; y_chunk < num_chunks; y_chunk++) {
                Point3 p = Point3((x_chunk-num_chunks/2)*16.0f + origin.x, y_chunk*16.0f + origin.y, (z_chunk-num_chunks/2)*16.0f + origin.z);
                chunk_points.append(p);
            }
        }
    }
}
