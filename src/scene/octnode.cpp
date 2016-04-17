#include "octnode.h"

// Include parent pointer?
/*
Octree subdivides in this order:

    2---6
   /|  /|
  3---7 |
  | 0-|-4
  |/  |/
  1---5

  */
// +z is toward you!
// base point = "bottom left" of octnode
OctNode::OctNode(Point3 base, int length) : base(base), length(length), is_leaf(true), chunk(nullptr)
{
//    qDebug() << "Creating new node at: ";
//    qDebug() << QString::fromStdString(glm::to_string(base.toVec3()));

    //    if (!is_leaf) {
    //        children.append(new OctNode(Point3(base.x, base.y, base.z), length/2));
    //        children.append(new OctNode(Point3(base.x, base.y, base.z + (length/2)), length/2));
    //        children.append(new OctNode(Point3(base.x, base.y + (length/2), base.z), length/2));
    //        children.append(new OctNode(Point3(base.x, base.y + (length/2), base.z + (length/2)), length/2));
    //        children.append(new OctNode(Point3(base.x + (length/2), base.y, base.z), length/2));
    //        children.append(new OctNode(Point3(base.x + (length/2), base.y, base.z + (length/2)), length/2));
    //        children.append(new OctNode(Point3(base.x + (length/2), base.y + (length/2), base.z), length/2));
    //        children.append(new OctNode(Point3(base.x + (length/2), base.y + (length/2), base.z + (length/2)), length/2));
    //    }
}

OctNode::~OctNode()
{
    for (OctNode* child : children) {
        delete child;
    }
    delete this;
}

// Returns the quadrant (0-7) containing the point
int OctNode::getQuadrant(Point3 p)
{
    if (this->length == 1) {
        return -1;
    }
    int x_mid = base.x + (length/2);
    int y_mid = base.y + (length/2);
    int z_mid = base.z + (length/2);
    if (p.x < x_mid && p.y < y_mid && p.z < z_mid) {
        return 0;
    } else if (p.x < x_mid && p.y < y_mid && p.z >= z_mid) {
        return 1;
    } else if (p.x < x_mid && p.y >= y_mid && p.z < z_mid) {
        return 2;
    } else if (p.x < x_mid && p.y >= y_mid && p.z >= z_mid) {
        return 3;
    } else if (p.x >= x_mid && p.y < y_mid && p.z < z_mid) {
        return 4;
    } else if (p.x >= x_mid && p.y < y_mid && p.z >= z_mid) {
        return 5;
    } else if (p.x >= x_mid && p.y >= y_mid && p.z < z_mid) {
        return 6;
    } else if (p.x >= x_mid && p.y >= y_mid && p.z >= z_mid) {
        return 7;
    } else {
        qDebug() << "Called getQuadrant on an ineligible node";
        return -1;
    }
}

// Returns the leaf node OF SIZE ONE containing the point
OctNode* OctNode::buildTree(Point3 p)
{
    if (length > 1) {
        if (is_leaf) {  // Recursively build the tree
        is_leaf = false;
        children.append(new OctNode(Point3(base.x, base.y, base.z), length/2));
        children.append(new OctNode(Point3(base.x, base.y, base.z + (length/2)), length/2));
        children.append(new OctNode(Point3(base.x, base.y + (length/2), base.z), length/2));
        children.append(new OctNode(Point3(base.x, base.y + (length/2), base.z + (length/2)), length/2));
        children.append(new OctNode(Point3(base.x + (length/2), base.y, base.z), length/2));
        children.append(new OctNode(Point3(base.x + (length/2), base.y, base.z + (length/2)), length/2));
        children.append(new OctNode(Point3(base.x + (length/2), base.y + (length/2), base.z), length/2));
        children.append(new OctNode(Point3(base.x + (length/2), base.y + (length/2), base.z + (length/2)), length/2));
        }
        // Even if this isn't a leaf, we still want to find the child to recurse on
        return children[this->getQuadrant(p)]->buildTree(p);
    } else {
        return this;
    }
}

// p must be an exact base coordinate (bottom left corner of some octnode)
OctNode* OctNode::getContainingNode(Point3 p)
{
    if (this->is_leaf) {
        return this;
    }
    int x_mid = base.x + (length/2);
    int y_mid = base.y + (length/2);
    int z_mid = base.z + (length/2);
    if (p.x < x_mid && p.y < y_mid && p.z < z_mid) {
        return children[0]->getContainingNode(p);
    } else if (p.x < x_mid && p.y < y_mid && p.z >= z_mid) {
        return children[1]->getContainingNode(p);
    } else if (p.x < x_mid && p.y >= y_mid && p.z < z_mid) {
        return children[2]->getContainingNode(p);
    } else if (p.x < x_mid && p.y >= y_mid && p.z >= z_mid) {
        return children[3]->getContainingNode(p);
    } else if (p.x >= x_mid && p.y < y_mid && p.z < z_mid) {
        return children[4]->getContainingNode(p);
    } else if (p.x >= x_mid && p.y < y_mid && p.z >= z_mid) {
        return children[5]->getContainingNode(p);
    } else if (p.x >= x_mid && p.y >= y_mid && p.z < z_mid) {
        return children[6]->getContainingNode(p);
    } else if (p.x >= x_mid && p.y >= y_mid && p.z >= z_mid) {
        return children[7]->getContainingNode(p);
    } else {
        qDebug() << "Something went wrong";
        return nullptr;
    }
}
