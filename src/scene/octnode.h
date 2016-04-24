#ifndef OCTNODE_H
#define OCTNODE_H

#include "geometry/chunk.h"
#include "point3.h"
#include "ray.h"
#include "intersection.h"

class OctNode
{
public:
    OctNode(Point3 base, int length);
    ~OctNode();
    bool is_leaf;
    int length;     // in chunks
    Point3 base;    // the smallest x, y, and z coordinates of the node
    QList<OctNode*> children;
    Chunk* chunk;   // null unless is_leaf == true
    Intersection intersect;

    OctNode* getContainingNode(Point3 p);
    OctNode* buildTree(Point3 p);
    int getQuadrant(Point3 p);
    OctNode* rayCastOct(Ray ray);
    Intersection findIntersection(Ray ray, float minx, float maxx, float miny, float maxy, float minz, float maxz);
    bool nearlyEqual(float a, float b);
};

#endif // OCTNODE_H
