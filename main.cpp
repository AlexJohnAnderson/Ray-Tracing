#include "color.h"
#include "vec3.h"

#include <iostream>

int main() {

    //Image size
    int image_width = 256;
    int image_height = 256;

    //Render the image
    
    //ppm image header
    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    //for loop to iterate over img height
    for (int j = 0; j < image_height; ++j) {
        //message will be printed to error stream. \r is used to overwrite the previous line. std::flush ensures the message is immediately displayed
        std::clog << "\rScanlines remaining: " << (image_height -j) << ' ' << std::flush;
        //nested look to iterate over img width
        for (int i = 0; i < image_width; ++i) {
            //sets pixel_color to rgb components using vec3 object
            auto pixel_color = color(double(i)/(image_width-1), double(j)/(image_height-1), 0);
            //writes the rgb vectors to std::out
            write_color(std::cout, pixel_color);
        }
    }
    //error stream message to signify output is done
    std::clog << "\rDone.\n";
}