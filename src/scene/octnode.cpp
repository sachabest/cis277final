#include "octnode.h"

struct sort_pred {
    bool operator()(const std::pair<float,OctNode*> &left, const std::pair<float,OctNode*> &right) {
        return left.first < right.first;
    }
};

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

bool OctNode::nearlyEqual(float a, float b) {
    float epsilon = 0.000001;
    float abso = glm::abs(a-b);
    return (abso < epsilon);
}

Intersection OctNode::findIntersection(Ray ray, float minx, float maxx, float miny, float maxy, float minz, float maxz) {
    float t_near = -100000;
    float t_far = 100000;

    //glm::vec3 R0 = glm::vec3(glm::floor(ray.origin.x/16), glm::floor(ray.origin.y/16), glm::floor(ray.origin.z/16));
    glm::vec3 R0 = ray.origin;
    glm::vec3 Rd = ray.direction;
    //glm::vec3 invdir = glm::vec3(1.0f/Rd.x, 1.0f/Rd.y, 1.0f/Rd.z);

    /*
//    //negative direction
//    if (Rd.x < 0) {
//        Rd.x = -1 * Rd.x;
//    }
//    if (Rd.y < 0) {
//        Rd.y = -1 * Rd.y;
//    }
//    if (Rd.z < 0) {
//        Rd.z = -1 * Rd.z;
//    }*/

    //no intersection
    if ((Rd.x == 0 && (R0.x < minx || R0.x > maxx)) ||
           (Rd.y == 0 && (R0.y < miny || R0.y > maxy)) ||
            (Rd.z == 0 && (R0.z < minz || R0.z > maxz))) {
        //return Point3();
        this->intersect = Intersection(Point3(), INFINITY);
        return this->intersect;
    }

    float t_x0 = 0.0;
    float t_x1 = 0.0;
    float t_y0 = 0.0;
    float t_y1 = 0.0;
    float t_z0 = 0.0;
    float t_z1 = 0.0;

    //X Part
    if (!nearlyEqual(Rd.x, 0)) {
    //if (Rd.x >= 0) {
        t_x0 = (minx - R0.x) / Rd.x;
        t_x1 = (maxx - R0.x) / Rd.x;
    }
    else {
        t_x0 = R0.x;
        t_x1 = R0.x;
    }

//    if (invdir.x >= 0) {
//        t_x0 = (minx - R0.x) * invdir.x;
//        t_x1 = (maxx - R0.x) * invdir.x;
//    }
//    else {
//        t_x0 = (maxx - R0.x) * invdir.x;
//        t_x1 = (minx - R0.x) * invdir.x;
//    }

    if (t_x0 > t_x1) {
        float temp = t_x1;
        t_x1 = t_x0;
        t_x0 = temp;
    }

    if (t_x0 > t_near) {
        t_near = t_x0;
    }
    if (t_x1 < t_far) {
        t_far = t_x1;
    }

    //Y Part
    if (!nearlyEqual(Rd.y, 0)) {
    //if (nearlyEqual(Rd.y, Rd.y)) {
        t_y0 = (miny - R0.y) / Rd.y;
        t_y1 = (maxy - R0.y) / Rd.y;
    }
    else {
        t_y0 = R0.y;
        t_y1 = R0.y;
    }

//    if (invdir.y >= 0) {
//        t_y0 = (miny - R0.y) * invdir.y;
//        t_y1 = (maxy - R0.y) * invdir.y;
//    }
//    else {
//        t_y0 = (maxy - R0.y) * invdir.y;
//        t_y1 = (miny - R0.y) * invdir.y;
//    }

    if (t_y0 > t_y1) {
        float temp = t_y1;
        t_y1 = t_y0;
        t_y0 = temp;
    }

    if (t_y0 > t_near) {
        t_near = t_y0;
    }
    if (t_y1 < t_far) {
        t_far = t_y1;
    }

    //Z Part
    if (!nearlyEqual(Rd.z, 0)) {
        t_z0 = (minz - R0.z) / Rd.z;
        t_z1 = (maxz - R0.z) / Rd.z;
    }
    else {
        t_z0 = R0.z;
        t_z1 = R0.z;
    }

//    if (invdir.z >= 0) {
//        t_z0 = (minz - R0.z) * invdir.z;
//        t_z1 = (maxz - R0.z) * invdir.z;
//    }
//    else {
//        t_z0 = (maxz - R0.z) * invdir.z;
//        t_z1 = (minz - R0.z) * invdir.z;
//    }

    if (t_z0 > t_z1) {
        float temp = t_z1;
        t_z1 = t_z0;
        t_z0 = temp;
    }

    if (t_z0 > t_near) {
        t_near = t_z0;
    }
    if (t_z1 < t_far) {
        t_far = t_z1;
    }


    if (t_near < 0 && t_far < 0) {
        this->intersect = Intersection(Point3(), INFINITY);
        return this->intersect;
    }
    if (t_near <= t_far) {
//        if (t_near < 0) {
//            t_near = -1 * t_far;
//        }
        float t;
        if (t_near > 0) {
            t = t_near;
        }
        else {
            t = t_far;
        }
        glm::vec3 intersect_point = glm::vec3(R0.x + Rd.x*t,
                                              R0.y + Rd.y*t,
                                              R0.z + Rd.z*t);
        //return Point3(intersect_point.x, intersect_point.y, intersect_point.z);
        this->intersect = Intersection(Point3(intersect_point.x, intersect_point.y, intersect_point.z), t);
//        this->intersect = Intersection(Point3(glm::floor(intersect_point.x),
//                                              glm::floor(intersect_point.y),
//                                              glm::floor(intersect_point.z)), t);
        return this->intersect;
        /*
        //return Intersection(Point3(intersect_point.x, intersect_point.y, intersect_point.z), t_near);

//        std::cout << "cube x " << intersect_point.x << std::endl;
//        std::cout << "cube y " << intersect_point.y << std::endl;
//        std::cout << "cube z " << intersect_point.z << std::endl;
//        std::cout << "____________" << std::endl;


        //find t value
        //float t = t_near;
        //glm::vec4 point = model*glm::vec4(intersect_point, 1);
        //glm::vec4 world_normal = inverse_transpose*glm::vec4(normal, 0);

        //return Intersection(glm::vec3(point.x, point.y, point.z), glm::vec3(world_normal.x, world_normal.y, world_normal.z), t, this);

        //return Intersection(intersect_point, normal, t, this);*/
    }
   /* //nullptr = NO INTERSECTION
    //return Intersection();
    //return Intersection(Point3(), INFINITY);*/
    this->intersect = Intersection(Point3(), INFINITY);
    return this->intersect;
}

OctNode* OctNode::rayCastOct(Ray ray) {

    float minx = base.x * 16;
    float miny = base.y * 16;
    float minz = base.z * 16;
    float maxx = (base.x + length) * 16;
    float maxy = (base.y + length) * 16;
    float maxz = (base.z + length) * 16;

    Intersection isIntersect = findIntersection(ray, minx, maxx, miny, maxy, minz, maxz);
    qDebug() << "length cube level " << this->length;
    qDebug() << "base x " << base.x;
    qDebug() << "base y " << base.y;
    qDebug() << "base z " << base.z;
    qDebug() << "big cube intersect x " << isIntersect.point_val.x;
    qDebug() << "big cube intersect y " << isIntersect.point_val.y;
    qDebug() << "big cube intersect z " << isIntersect.point_val.z;
    qDebug() << "----------------------------------";

    //does it hit the big cube or not
    if (isIntersect.t != INFINITY) {
        //single block leaf node
        if (this->is_leaf && this-> length == 1) {
            return this;
        }
        else if (!this->is_leaf){
            std::vector<std::pair<float, OctNode*>> tvalues;

            //no intersection -> t will be infinity
            float t0 = findIntersection(ray,
                            children[0]->base.x * 16, (children[0]->base.x + children[0]->length) * 16,
                            children[0]->base.y * 16, (children[0]->base.y + children[0]->length) * 16,
                            children[0]->base.z * 16, (children[0]->base.z + children[0]->length) * 16).t;
            std::pair <float,OctNode*> pair0;
            pair0 = std::make_pair (t0, children[0]);
            tvalues.push_back(pair0);

            float t1 = findIntersection(ray,
                            children[1]->base.x * 16, (children[1]->base.x + children[1]->length) * 16,
                            children[1]->base.y * 16, (children[1]->base.y + children[1]->length) * 16,
                            children[1]->base.z * 16, (children[1]->base.z + children[1]->length) * 16).t;
            std::pair <float,OctNode*> pair1;
            pair1 = std::make_pair (t1, children[1]);
            tvalues.push_back(pair1);

            float t2 = findIntersection(ray,
                            children[2]->base.x * 16, (children[2]->base.x + children[2]->length) * 16,
                            children[2]->base.y * 16, (children[2]->base.y + children[2]->length) * 16,
                            children[2]->base.z * 16, (children[2]->base.z + children[2]->length) * 16).t;
            std::pair <float,OctNode*> pair2;
            pair2 = std::make_pair (t2, children[2]);
            tvalues.push_back(pair2);

            float t3 = findIntersection(ray,
                            children[3]->base.x * 16, (children[3]->base.x + children[3]->length) * 16,
                            children[3]->base.y * 16, (children[3]->base.y + children[3]->length) * 16,
                            children[3]->base.z * 16, (children[3]->base.z + children[3]->length) * 16).t;
            std::pair <float,OctNode*> pair3;
            pair3 = std::make_pair (t3, children[3]);
            tvalues.push_back(pair3);

            float t4 = findIntersection(ray,
                            children[4]->base.x * 16, (children[4]->base.x + children[4]->length) * 16,
                            children[4]->base.y * 16, (children[4]->base.y + children[4]->length) * 16,
                            children[4]->base.z * 16, (children[4]->base.z + children[4]->length) * 16).t;
            std::pair <float,OctNode*> pair4;
            pair4 = std::make_pair (t4, children[4]);
            tvalues.push_back(pair4);

            float t5 = findIntersection(ray,
                            children[5]->base.x * 16, (children[5]->base.x + children[5]->length) * 16,
                            children[5]->base.y * 16, (children[5]->base.y + children[5]->length) * 16,
                            children[5]->base.z * 16, (children[5]->base.z + children[5]->length) * 16).t;
            std::pair <float,OctNode*> pair5;
            pair5 = std::make_pair (t5, children[5]);
            tvalues.push_back(pair5);

            float t6 = findIntersection(ray,
                            children[6]->base.x * 16, (children[6]->base.x + children[6]->length) * 16,
                            children[6]->base.y * 16, (children[6]->base.y + children[6]->length) * 16,
                            children[6]->base.z * 16, (children[6]->base.z + children[6]->length) * 16).t;
            std::pair <float,OctNode*> pair6;
            pair6 = std::make_pair (t6, children[6]);
            tvalues.push_back(pair6);

            float t7 = findIntersection(ray,
                            children[7]->base.x * 16, (children[7]->base.x + children[7]->length) * 16,
                            children[7]->base.y * 16, (children[7]->base.y + children[7]->length) * 16,
                            children[7]->base.z * 16, (children[7]->base.z + children[7]->length) * 16).t;
            std::pair <float,OctNode*> pair7;
            pair7 = std::make_pair (t7, children[7]);
            tvalues.push_back(pair7);

            std::sort(tvalues.begin(), tvalues.end(), sort_pred());
            //QList<OctNode*> interOct;
            for (int i = 0; i < tvalues.size(); i++) {
                //interOct.append(tvalues.at(i).second->rayCastOct(ray));
                if (tvalues.at(i).second->rayCastOct(ray) != nullptr) {
                    return tvalues.at(i).second->rayCastOct(ray);
                }
            }
        }
        else{
            return nullptr;
        }
    }
    return nullptr;
}
