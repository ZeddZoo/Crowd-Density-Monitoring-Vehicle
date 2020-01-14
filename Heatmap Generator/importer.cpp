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
#include "personProcessor.h"

#define NPOINTS = 1000;

class Importer {
  public:
    // Maybe take in a JSON or something
    static std::vector<personFeature_t> pointExtractor(std::string source) {
      std::vector<personFeature_t> persons;
      // IMPORT STUFF USING OpenCV or something
      std::vector<SOMETHING> rawPersons = GETSOMETHING(SOMETHINGELSE);
      for (SOMETHING rawDude : rawPersons) {
        size_t x = ???;
        size_t y = ???;
        // Create the person
        personFeature_t person;
        personProcessor::setAgeSeconds(&person);
        personProcessor::setId(&person);
        personProcessor::setPosition(x, y, &person);
        persons.push_back(person);
      }
      return persons;
    }

    static int getHeatMap(size_t height, size_t width, std::string file,
                        std::vector<personFeature_t> personList) {
      // Initialize the heatmap object
      heatmap_t* hm = heatmap_new(width, height);

      std::vector<personFeature_t>::iterator point;
      // Add each feature point to the point cloud
      for (point = personList.begin(); point < personList.end(); point++) {
        personFeature_t thisPoint = *point;
        heatmap_add_point(hm, std::get<0>(thisPoint.position),
                              std::get<1>(thisPoint.position));
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
};
