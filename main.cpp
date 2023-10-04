#include "color.h"
#include "vec3.h"
#include "ray.h"

#include <cmath>
#include <iostream>

//function to determine if sphere is h
double hit_sphere(const point3& center, double radius, const ray& r) {
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
    //return -1 if the discriminant is greather than 0(indicates the ray does not intersect the sphere)
    if((discriminant < 0)){
        return -1.0;
    }
    //return the distance along the ray at which the intersection occurs
    else{
        return((-half_b - sqrt(discriminant)) / (a));
    }
}

//currently does not render a cube 
//needs to detect edges of cube using discriminant
bool hit_cube(const point3& min_corner, const point3& max_corner, const ray& r){
    auto a = (min_corner.x() - r.origin().x()) / (r.direction().x());
    auto b = (max_corner.x() - r.origin().x()) / (r.direction().x());
    auto c = (min_corner.y() - r.origin().y()) / (r.direction().y());
    auto d = (max_corner.y() - r.origin().y()) / (r.direction().y());
    auto e = (min_corner.z() - r.origin().z()) / (r.direction().z());
    auto f = (max_corner.z() - r.origin().z()) / (r.direction().z());

    double min = std::max(std::max(std::min(a, b), std::min(c, d)), std::min(e, f));
    double max = std::min(std::min(std::max(a, b), std::max(c, d)), std::max(e, f));

    return(max >= min && max >= 0);
}

//calculates the ray color as it interacts with the viewport
color ray_color(const ray& r){
    //calculates the unit vector of the rays direction
    vec3 unit_direction = unit_vector(r.direction());
    //calculates a using the y component(verticl extension of the vector) of the normalized ray direction
    //we add 1 to shift the range from [-1, 1] to [0, 2]
    //we multiply this by 0.5 to make the range [0, 1]
    //this remapping is done to create a gradient effect on the vertical axis
    auto a =0.5*(unit_direction.y() + 1.0);
    //check to see if the ray r hit the shape.

    auto t = hit_sphere(point3(0,0,-1), 0.5, r);
    if (t > 0.0) {
        vec3 N = unit_vector(r.at(t) - vec3(0,0,-1));
        return 0.5*color(N.x()+1, N.y()+1, N.z()+1);
    }

    //returns a vec3 object with rgb set to 0
    //            white portion              blue poriton
    return (1.0-a)*color(1.0, 1.0, 1.0) + a*color(0.5, 0.7, 1.0);
}

int main() {

    //Image size
    auto aspect_ratio = 16.0/9.0;
    int image_width = 400;

    //calculate image height based on aspect ratio and ensure that image height can never be less than 1
    int image_height = static_cast<int>(image_width / aspect_ratio);
    image_height = (image_height < 1) ? 1 : image_height;

    //Camera setup
    // represents distance from the camera to the image
    auto focal_length = 1.0;
    //determines the dimensions of the image plane
    auto viewport_height = 2.0;
    auto viewport_width = viewport_height * (static_cast<double>(image_width)/image_height);
    //origin point of camera position
    auto camera_center = point3(0, 0, 0);

    //calculate the vectors across the horizontal and down the vertical viewport edges.
    auto viewport_u = vec3(viewport_width, 0, 0);
    auto viewport_v = vec3(0, -viewport_height, 0);

    //calculate the horizontal and vertical delta vectors from pixel to pixel.
    auto pixel_delta_u = viewport_u / image_width;
    auto pixel_delta_v = viewport_v / image_height;

    //calculate the location of the upper left pixel on the image plane.

    //start from the center and adjust for the focal length and viewport dimensions
    //upper left is calculated by subtracing a vector from the point at the center then 
    //subtracting half the width and half the height from the center point
    auto viewport_upper_left = camera_center
                             - vec3(0, 0, focal_length) - viewport_u/2 - viewport_v/2;
    //pixel location is calculated by taking left most point of viewport and adding the position between two adjacent pixels
    //doing so shift the position from the upper-left corner of the viewport to the upper left corner of the first pixel in the image.
    auto pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);


    //render the image
    
    //ppm image header
    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    //for loop to iterate over img height
    for (int j = 0; j < image_height; ++j) {
        //message will be printed to error stream. \r is used to overwrite the previous line. std::flush ensures the message is immediately displayed
        std::clog << "\rScanlines remaining: " << (image_height -j) << ' ' << std::flush;
        //nested loop to iterate over img width
        for (int i = 0; i < image_width; ++i) {
            //calculates the position of the current pixel using the upper left pixel
            auto pixel_center = pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
            //calculates the direction of a ray from the camera center to the current pixel
            auto ray_direction = pixel_center - camera_center;
            //creates a ray object using the center point and the direction of the ray
            ray r(camera_center, ray_direction);
            //color of the pixel is calculated using ray_color
            color pixel_color = ray_color(r);
            //writes the color to std::out which is then formatted to a .ppm file
            write_color(std::cout, pixel_color);
        }
    }
    //error stream message to signify output is done
    std::clog << "\rDone.\n";
}