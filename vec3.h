#ifndef VEC3_H
#define VEC3_H

#include <cmath>
#include <iostream>

using std::sqrt;

//3D vector class
class vec3{
    public: 

        double e[3];

        //constructor
        vec3() : e{0,0,0} {}
        //parameterized constructor
        vec3(double e0, double e1, double e2) : e{e0, e1, e2} {}

        //x, y, z coordinates
        double x() const { return e[0]; }
        double y() const { return e[1]; }
        double z() const { return e[2]; }


        //overloading - operator to turn the vector poiting in the opposite direction
        vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); }
        //overloading [] to return the index passed in(0, 1, or 2) of the vector. Can be called on const intances(read only) of vec3
        double operator[](int i) const { return e[i]; }
        //overloading [] to set the value at an index
        double& operator[](int i) { return e[i]; }

        //operator overload will allow for += to be used on vetors
        vec3& operator+=(const vec3 &V){
            e[0] += V.e[0];
            e[1] += V.e[1];
            e[2] += V.e[2];
            return *this;
        }

        //operator overload will allow for *= to be used on vetors
        vec3& operator*=(double t){
            e[0] *= t;
            e[1] *= t;
            e[2] *= t;
            return *this;
        }

        //this operator overload will allow for /= to be used on vetors
        vec3& operator/=(double t) {
        return *this *= 1/t;
    }

    //returns the length(magnitude) of a vector using Euclidean Distance
    double length() const {
        return sqrt(length_squared());
    }
    
    //finds the squared length of a vector
    double length_squared() const {
        return e[0]*e[0] + e[1]*e[1] + e[2]*e[2];
    }
};

// point3 is just an alias for vec3, but useful for geometric clarity in the code.
using point3 = vec3;


// Vector Utility Functions

//printing vec3 objects
inline std::ostream& operator<<(std::ostream &out, const vec3 &v) {
    return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

//addition of vec3 objects
inline vec3 operator+(const vec3 &u, const vec3 &v) {
    return vec3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}

//subtraction of vec3 objects
inline vec3 operator-(const vec3 &u, const vec3 &v) {
    return vec3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}

//component wise multiplication of vec3 objects
inline vec3 operator*(const vec3 &u, const vec3 &v) {
    return vec3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}

//scalar multiplication of vec3 objects
inline vec3 operator*(double t, const vec3 &v) {
    return vec3(t*v.e[0], t*v.e[1], t*v.e[2]);
}

//scalar multiplication of vec3 objects but reverses the order
inline vec3 operator*(const vec3 &v, double t) {
    return t * v;
}

//division of a vec3 object by a scalar
inline vec3 operator/(vec3 v, double t) {
    return (1/t) * v;
}

//dot product calcuation between two vec3 objects
inline double dot(const vec3 &u, const vec3 &v) {
    return u.e[0] * v.e[0]
         + u.e[1] * v.e[1]
         + u.e[2] * v.e[2];
}

//cross multiplication between two vec3 objects
inline vec3 cross(const vec3 &u, const vec3 &v) {
    return vec3(u.e[1] * v.e[2] - u.e[2] * v.e[1],
                u.e[2] * v.e[0] - u.e[0] * v.e[2],
                u.e[0] * v.e[1] - u.e[1] * v.e[0]);
}

//unit vector of a ved3 object
inline vec3 unit_vector(vec3 v) {
    return v / v.length();
}

#endif