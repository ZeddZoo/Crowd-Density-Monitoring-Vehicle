/*
    Uses https://github.com/lucasb-eyer/libheatmap to create and image
    containing the heatmap of the image, given:
      1) widht and height
      2) Destination file name
      3) A list of feature points
    TODO: Figure out how to interface this with the the other things
    Maybe also take in a pointer to store the image file????
*/

#include <vector>
#include <iostream>
#include <assert.h>

#include "lodepng-master/lodepng.h"
#include "libheatmap-master/heatmap.h"

#define NPOINTS = 1000;

int main (size_t height, size_t width, std::string file,
                    std::vector<std::tuple<size_t, size_t>> pointList) {

  // Initialize the heatmap object
  heatmap_t* hm = heatmap_new(width, height);

  std::vector<std::tuple<size_t, size_t>>::iterator point;
  // Add each feature point to the point cloud
  for (point = pointList.begin(); point < pointList.end(); point++) {
    std::tuple<size_t, size_t> thisPoint = *point;
    heatmap_add_point(hm, std::get<0>(thisPoint),
                          std::get<1>(thisPoint));
  }

  // Create image out of the heatmap
  std::vector<unsigned char> image(width * height * 4);
  heatmap_render_default_to(hm, &image[0]);

  heatmap_free(hm);

  // Export the image
  std::cout << "Exporting to distination file: " << file;
  if (unsigned error = lodepng::encode(file, image, width, height)) {
    std::cerr << "encoder error" << error
              << ": " << lodepng_error_text(error) << std::endl;
    return 1;
  }
  return 0;
}