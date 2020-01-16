/*
 * Main file for exporting JSON and saving heatmap data
 */

#include <arpa/inet.h>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string.h>
#include <sys/socket.h>

#include "importer.h"
#include "json-develop/single_include/nlohmann/json.hpp"
#include "personProcessor.h"
#include "pointAnalyzer.h"

static int PORT = 8000, WIDTH = 1280, HEIGHT = 720,
           IMAGE_SIZE = sizeof(int) * WIDTH * HEIGHT;

int main(int argc, const char *argv[]) {
  if (argc != 2) {
    std::cout << "Error: expected IP address argument" << std::endl;
    return EXIT_FAILURE;
  }

  // create socket
  int sock = 0;
  if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    std::cout << "Error: Failed to create socket" << std::endl;
    return EXIT_FAILURE;
  }

  // create address
  sockaddr_in server_address;
  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(PORT);
  if (inet_pton(AF_INET, argv[1], &server_address.sin_addr) <= 0) {
    std::cout << "Error: Invalid Address: " << argv[1] << std::endl;
    return EXIT_FAILURE;
  }

  // connect socket
  if (connect(sock, (sockaddr *)&server_address, sizeof(server_address)) < 0) {
    std::cout << "Error: Connection failed" << std::endl;
    return EXIT_FAILURE;
  }

  // continously read from stream
  int buffer[IMAGE_SIZE];
  while (read(sock, buffer, IMAGE_SIZE) == IMAGE_SIZE) {
    cv::Mat image(cv::Size(WIDTH, HEIGHT), cv::CV_32S, buffer);

    std::vector<personFeature_t> peopleList = Importer::pointExtractor(image);

    Compute and export the Heat Map Importer::getHeatMap(
        height, width, "heatMap.png", peopleList);

    // Analyze points
    // First, we get the sector counts
    std::vector<int> sectorCount =
        Analyzer::getSectorCount(width, height, peopleList);

    // Do the calculations
    std::vector<size_t> alertedSectors = Analyzer::getAlertSectors(sectorCount);
    std::vector<size_t> mostPopSect =
        Analyzer::getMostPopulatedSectors(sectorCount);
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
}