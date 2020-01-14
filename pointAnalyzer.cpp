/*
    Analyzer class contains methods for  feature analysis
    */

#include <vector>
#include <tuple>

#include "importer.h"

class Analyzer {

  // Keep track of the prevous positions as a class attribute
  static std::vector<personFeature_t> previousPosition;

  // Get the alerted sectors
  std::vector<size_t> getAlertSectors(size_t gridWidth, size_t gridHeight,
                                      std::vector<personFeature_t> persons);

  // Get the most populated sectors
  std::vector<size_t> getMostPopulatedSectors(size_t gridWidth, size_t gridHeight,
                                      std::vector<personFeature_t> persons);

  // Get the average speed of the people
  int getAverageSpeed(size_t gridWidth, size_t gridHeight,
                      int width, int length, std::vector<personFeature_t> persons);

  // Get the number of people in the room
  int getPopulation(std::vector<personFeature_t> persons);

};