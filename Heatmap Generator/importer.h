/*
    Importer takes a list of points and outputs a heatmap
    */

#include "importer.cpp"
#include "personProcessor.h"

class Importer {
  public:
    // Acquire the points for the import methodd
    static std::vector<personFeature_t> pointExtractor(std::string source);

    // Export the importet points as a heatmap
    static int getHeatMap(size_t height, size_t width, std::string file,
                        std::vector<personFeature_t> personList);
};