#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "vec3.h"

class sphere : public hittable {
  public:
    //parameterized constructor 
    sphere(point3 _center, double _radius) : center(_center), radius(_radius) {}

    //r is a reference to a ray to be tested for intersection
    //ray_tmin is the min valid value for ray object intersections
    //ray_tmax is the max valid value for ray object intersections 
    //rec is a reference to a hit_record object where info about intersection is stored
    bool hit(const ray& r, double ray_tmin, double ray_tmax, hit_record& rec) const override {
        //calculates a vector from the origin of the ray to the center of the sphere
        vec3 oc = r.origin() - center;
        //a, b and c will be used for calculating the discriminant of the sphere
        //calculates the magnitude of r.
        //a is a measure of how "fast" the ray is moving
        auto a = r.direction().length_squared();
        //calculates the dot product between the oc vector and the ray passed in
        //b is a measure of where the ray intersection occurs
        auto half_b = dot(oc, r.direction());
        //calculates the magnitude of oc and subtracts radius squared
        //c is a measure of wheather the ray's origin is inside or outside
        auto c = oc.length_squared() - radius*radius;

        //formula to see if ray intersects a sphere
        auto discriminant = half_b*half_b - a*c;
        //ray does not intersect the sphere
        if (discriminant < 0) return false;
        auto sqrtd = sqrt(discriminant);

        // Find the nearest root that lies in the acceptable range.
        auto root = (-half_b - sqrtd) / a;
        //check if the root is in the valid range of values
        if (root <= ray_tmin || ray_tmax <= root) {
            //set root equal to the 
            root = (-half_b + sqrtd) / a;
            //chceck if root is in the evalid range of values
            if (root <= ray_tmin || ray_tmax <= root)
                return false;
        }
        //setters for the hit record, rec
        //set t to root(value that the intersection of the sphere and ray occurs)
        rec.t = root;
        //calculates and sets the intersection point based on the ray, r and param rec.t. 
        //rec.p represents the actual 3D point in space where the ray intersects the sphere
        rec.p = r.at(rec.t);
        //calculates vector that will be used for shading and lighting.
        //rec.p-center calculates a vector from the center to the point of intersection.(direction)
        //dividing by the radius normalizes the vector which is useful for shading
        rec.normal = (rec.p - center) / radius;

        return true;
    }

  private:
    point3 center;
    double radius;
};

#endif