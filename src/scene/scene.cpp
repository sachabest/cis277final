#include <scene/scene.h>
#include <scene/geometry/cube.h>
#include <scene/geometry/chunk.h>
#define MAX_TERRAIN_HEIGHT 3

Scene::Scene() : dimensions(96, 96, 96), terrain(96, 96), num_chunks(6), origin(glm::vec3(0))
{
}

void Scene::shift(int dx, int dy, int dz) {
    origin.x += dx;
    origin.y += dy;
    origin.z += dz;
    terrain.shift(dx, dz);
    //CreateScene();
    CreateNewChunks();
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
    for (int x_chunk = 0; x_chunk < num_chunks; x_chunk++) {
        for (int z_chunk = 0; z_chunk < num_chunks; z_chunk++) {
            Chunk* chunk = new Chunk();
            for (int x = 0; x < 16; x++) {
                for (int z = 0; z < 16; z++) {
                    float height = terrain.getBlock((x + 16*x_chunk) / (float) dimensions[0], (z + 16*z_chunk) / (float) dimensions[2]);
                    for (int y = 0; y < height; y++) {
                        chunk->cells[x][y][z] = true;
                    }
                }
            }
            chunk->create();
            Point3 p = Point3((x_chunk-(num_chunks/2))*16.0f, 0, (z_chunk-(num_chunks/2))*16.0f);
            terrain.chunk_map.insert(p, chunk);
        }
    }
}

void Scene::CreateNewChunks()
{
    for (int x_chunk = 0; x_chunk < num_chunks; x_chunk++) {
        for (int z_chunk = 0; z_chunk < num_chunks; z_chunk++) {
            Point3 p = Point3((x_chunk-(num_chunks/2))*16.0f + origin.x, 0, (z_chunk-(num_chunks/2))*16.0f + origin.z);
            // Must generate a new chunk VBO
            if (!terrain.chunk_map.contains(p)) {
                Chunk* chunk = new Chunk();
                for (int x = 0; x < 16; x++) {
                    for (int z = 0; z < 16; z++) {
                        float height = terrain.getBlock((x + 16*x_chunk + origin.x) / (float) dimensions[0], (z + 16*z_chunk + origin.z) / (float) dimensions[2]);
                        for (int y = 0; y < height; y++) {
                            chunk->cells[x][y][z] = true;
                        }
                    }
                }
                chunk->create();
                terrain.chunk_map.insert(p, chunk);
            }
        }
    }
}
