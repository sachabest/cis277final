#pragma once
#include <QList>
#include "camera.h"
#include "terrain/terrain.h"
#include "point3.h"
#include <iostream>
#include "transform.h"

class Scene {
public:
  Scene();

  void CreateScene();
  void shift(int dx, int dy, int dz);

  QList<Point3> points;
  glm::ivec3 dimensions;
  glm::vec3 origin;
  Terrain terrain;


};
