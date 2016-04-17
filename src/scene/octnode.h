#ifndef OCTNODE_H
#define OCTNODE_H

#include "geometry/chunk.h"
#include "point3.h"

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

    OctNode* getContainingNode(Point3 p);
    OctNode* buildTree(Point3 p);
    int getQuadrant(Point3 p);
};

#endif // OCTNODE_H
