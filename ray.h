#ifndef RAY_H
#define RAY_H

#include "vec3.h"

class ray{
    public:
        //constructor
        ray(){}
        //parameterized constructor, takes origin and direction(point3 and vec3, respectively)
        //this allows us to create a ray by specifying origin and direction during instantiation
        ray(const point3& origin, const vec3& direction) : orig(origin), dir(direction) {}

        //returns the origin point of the 3D line
        point3 origin() const { return orig; }
        //returs the direction vector of the 3D line
        vec3 direction() const { return dir; }

        //computes a point with the function P(t)=A+t(b) where A is the ray origin and b is the ray direction
        point3 at(double t) const {
            return orig + t*dir;
        }

        private:
            point3 orig;
            vec3 dir;
};

#endif