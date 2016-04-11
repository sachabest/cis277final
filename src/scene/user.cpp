#include "user.h"

User::User()
{
    center_position = glm::vec3(0,0,0);
}

User::User(Camera c) {
    //center of the user's position is the eye piece
    center_position = c.eye;

    //CALCULATE the boundary of the user's boxes
    //since the user is two cubes stacked on top of one another, with the camera centered at the top cube
    //and we know camera is at the center of the screen
    //bounds are calculated below
    float min_x = c.eye.x - 0.5;
    float max_x = c.eye.x + 0.5;
    float min_y = c.eye.y - 0.5;
    float max_y = c.eye.y + 1.5;
    float min_z = c.eye.z - 0.5;
    float max_z = c.eye.z + 0.5;
}

void User::collision(Cube *c) {
    //how to put two things in the same space
    //let's do everything in camera space! because user is in camera space
    //so multiply cube's position by camera inverse
    //once they are in the same space
    //compare the x y and z values

    //CHECK: is minz in the front of back
    //TEMP treating it at the front = minz

    //xmin_cube >= xmax_user || xmax_cube <= xmin_user || zmax_cube <= zmin_user || zmin_cube >= zmax_user || ymax_cube <= maxy_user
    //return FALSE -> no collision -> DONT CHANGE ANYTHING KEEP GOING

    //if miny_cube > maxy_user
    //RETURN FALSE
    //FREE FALLING -> HOW DO YOU FREE FALL

    //else: RETURN TRUE
    //BASICALLY: shift user's boxes to be aligned with the world -> adjust your camera position -> MIGHT BE BETTER TO CHECK THIS

    //IN THE X DIRECTION
    //if xmin_cube < xmas_user
    //find diff = xmas_user - xmin_cube
    //shift (left) xmin_user = xmin_user - diff
    //xmax_user = xmas_user - diff
    //everything else is the same; redraw this
    //if xmas_cube > xmin_user
    //diff = xmas_cube - xmin_user
    //shift (right) xmin_user = xmin_user + diff
    //xmas_user = xmas_user + diff

    //IN THE Z DIRECTION
    //if zmax_cube > zmin_user
    //shift user (back/away/+z direction)
    //diff = zmax_cube - zmin_user
    //zmin_user = zmin_user + diff; zmax_user = zmax_user + diff
    //if zmin_cube < zmax_user
    //shift user (front/forward/-z direction)
    //diff = zmax-user - zmin_cube
    //zmin_user = zmin_user - diff
    //zmax_user = zmax_user - diff

    //IN THE Y DIRECTIOn (WELL THIS GETS TRICKY WITH FREE FALLING)
    //intercepts with bottom -> move up
    //if ymin_cube < ymax_user
    //diff = ymax_user - ymin_cube
    //move user up in the -y direction
    //ymin_user = ymin_user - diff
    //ymax_user = ymax_user - diff

    //IN THE OTHER DIRECTION -> can potentially be free falling

}
