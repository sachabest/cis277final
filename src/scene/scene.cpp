#include <scene/scene.h>
#include <scene/geometry/cube.h>
#define MAX_TERRAIN_HEIGHT 3

Scene::Scene() : dimensions(64, 64, 64), terrain(64, 64)
{
}

void Scene::CreateTestScene() {
    points.clear();
    for(int x = 0; x < dimensions.x; x++) {
        for(int z = 0; z < dimensions.z; z++) {
            float height = terrain.getBlock(x / (float) dimensions[0], z / (float) dimensions[2]);
            height *= MAX_TERRAIN_HEIGHT;
            for (int y = 0; y < height; y++) {
                points.append(Point3(x, y, z));
            }
        }
    }

//    objects[0][dimensions.y/2+2][0] = true;
//    objects[dimensions.x-1][dimensions.y/2+2][0] = true;
//    objects[0][dimensions.y/2+2][dimensions.z-1] = true;
//    objects[dimensions.x-1][dimensions.y/2+2][dimensions.z-1] = true;
}
