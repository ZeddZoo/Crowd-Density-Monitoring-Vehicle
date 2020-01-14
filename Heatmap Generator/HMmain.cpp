/*
    Main file for exporting JSON and saving heatmap data
    */
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string.h>

#include "importer.h"
#include "json-develop/single_include/nlohmann/json.hpp"
#include "personProcessor.h"
#include "pointAnalyzer.h"

int main(size_t height, size_t width, std::string source) {
  // Acquire the list of people
  std::vector<personFeature_t> peopleList = Importer::pointExtractor(source);

  // Compute and export the Heat Map
  Importer::getHeatMap(height, width, "heatMap.png", peopleList);

  // Analyze points
  // First, we get the sector counts
  std::vector<int> sectorCount = Analyzer::getSectorCount(width, height, peopleList);

  // Do the calculations
  std::vector<size_t> alertedSectors = Analyzer::getAlertSectors(sectorCount);
  std::vector<size_t> mostPopSect = Analyzer::getMostPopulatedSectors(sectorCount);
  int population = Analyzer::getPopulation(peopleList);

  // Put everything into a JSON
  nlohmann::json data;
  nlohmann::json spots;

  data["alertedSectors"] = alertedSectors;
  spots["one"] = mostPopSect[0];
  spots["two"] = mostPopSect[1];
  spots["three"] = mostPopSect[2];
  data["topSpots"] = spots;
  data["averageSpeed"] = 0; // We aren't going to use this for now
  data["population"] = population;

  // Save the file to the analysis JSON
  std::ofstream toSave("analysis.json");
  toSave << std::setw(4) << data << std::endl;
}