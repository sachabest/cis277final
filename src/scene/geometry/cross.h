#ifndef CROSS_H
#define CROSS_H

#include <openGL/drawable.h>

class Cross : public Drawable
{
public:
    Cross();
    //DRAWING the center of the screen as a plus
    virtual GLenum drawMode();
    void create();
};

#endif // CROSS_H
