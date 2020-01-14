/*
    Analyzer class contains methods for  feature analysis

    SECTORS

    */

#include "pointAnalyzer.cpp"

class Analyzer {
  public:
    // Keep track of the prevous positions as a class attribute
    static std::vector<personFeature_t> previousPosition;

    // Get the sector count list
    static std::vector<int> getSectorCount(size_t gridWidth, size_t gridHeight,
                                        std::vector<personFeature_t> persons);

    // Get the alerted sectors
    static std::vector<size_t> getAlertSectors(std::vector<int> sectorCount);

    // Get the most populated sectors
    static std::vector<size_t> getMostPopulatedSectors(std::vector<int> sectorCount);

    // // Get the average speed of the people [ DON"T USE THIS LMAO, WAY TOO SLOW]
    // int getAverageSpeed(size_t gridWidth, size_t gridHeight,
    //                     int width, int length, std::vector<personFeature_t> persons);

    // Get the number of people in the room
    static int getPopulation(std::vector<personFeature_t> persons);

};