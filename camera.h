#ifndef CAMERA_H
#define CAMERA_H

#include "common.h"

#include "color.h"
#include "hittable.h"

#include <iostream>

class camera {
    
  public:

    double aspect_ratio = 1.0;  //ratio of image width over height
    int    image_width  = 100;  //rendered image width in pixel count
    int    samples_per_pixel = 10;   //count of random samples for each pixel

    void render(const hittable& scene) {
        //call to initialize the viewport
        initialize();

        //ppm image header
        std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

        //for loop to iterate over img height
        for (int j = 0; j < image_height; ++j) {

            //message will be printed to error stream. \r is used to overwrite the previous line. std::flush ensures the message is immediately displayed
            std::clog << "\rScanlines remaining: " << (image_height -j) << ' ' << std::flush;

            //nested loop to iterate over img width
            for (int i = 0; i < image_width; ++i) {

                color pixel_color(0,0,0);
                for (int sample = 0; sample < samples_per_pixel; ++sample) {
                    ray r = get_ray(i, j);
                    pixel_color += ray_color(r, scene);
                }
                write_color(std::cout, pixel_color, samples_per_pixel);
            }
        }
        //error stream message to signify output is done
        std::clog << "\rDone.\n";
    }

  private:

    int    image_height;   // Rendered image height
    point3 center;         // Camera center
    point3 pixel00_loc;    // Location of pixel 0, 0
    vec3   pixel_delta_u;  // Offset to pixel to the right
    vec3   pixel_delta_v;  // Offset to pixel below

    void initialize() {
        //calculate image height based on aspect ratio and ensure that image height can never be less than 1
        image_height = static_cast<int>(image_width / aspect_ratio);
        image_height = (image_height < 1) ? 1 : image_height;

        //origin point of camera position
        center = point3(0, 0, 0);

        //Camera setup
        // represents distance from the camera to the image
        auto focal_length = 1.0;
        //determines the dimensions of the image plane
        auto viewport_height = 2.0;
        auto viewport_width = viewport_height * (static_cast<double>(image_width)/image_height);

        //calculate the vectors across the horizontal and down the vertical viewport edges.
        auto viewport_u = vec3(viewport_width, 0, 0);
        auto viewport_v = vec3(0, -viewport_height, 0);

        //calculate the horizontal and vertical delta vectors from pixel to pixel.
        pixel_delta_u = viewport_u / image_width;
        pixel_delta_v = viewport_v / image_height;

        //calculate the location of the upper left pixel on the image plane.

        //start from the center and adjust for the focal length and viewport dimensions
        //upper left is calculated by subtracing a vector from the point at the center then 
        //subtracting half the width and half the height from the center point
        auto viewport_upper_left = center
                                 - vec3(0, 0, focal_length) - viewport_u/2 - viewport_v/2;
        //pixel location is calculated by taking left most point of viewport and adding the position between two adjacent pixels
        //doing so shift the position from the upper-left corner of the viewport to the upper left corner of the first pixel in the image.
        pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);
    }

    color ray_color(const ray& r, const hittable& scene){
        hit_record rec;

        if (scene.hit(r, interval(0, infinity), rec)) {
            return 0.5 * (rec.normal + color(1,1,1));
        }

        //calculates the unit vector of the rays direction
        vec3 unit_direction = unit_vector(r.direction());

        //calculates a using the y component(verticl extension of the vector) of the normalized ray direction
        //we add 1 to shift the range from [-1, 1] to [0, 2]
        //we multiply this by 0.5 to make the range [0, 1]
        //this remapping is done to create a gradient effect on the vertical axis
        auto a =0.5*(unit_direction.y() + 1.0);

        //returns a vec3 object with rgb set to 0
        //            white portion              blue poriton
        return (1.0-a)*color(1.0, 1.0, 1.0) + a*color(0.5, 0.7, 1.0);
    }
    
    ray get_ray(int i, int j) const {
        // Get a randomly sampled camera ray for the pixel at location i,j.

        auto pixel_center = pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
        auto pixel_sample = pixel_center + pixel_sample_square();

        auto ray_origin = center;
        auto ray_direction = pixel_sample - ray_origin;

        return ray(ray_origin, ray_direction);
    }

    vec3 pixel_sample_square() const {
        // Returns a random point in the square surrounding a pixel at the origin.
        auto px = -0.5 + random_double();
        auto py = -0.5 + random_double();
        return (px * pixel_delta_u) + (py * pixel_delta_v);
    }
};

#endif