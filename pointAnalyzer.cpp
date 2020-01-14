/*
    Analyzer class contains methods for  feature analysis
    */

#include <vector>
#include <tuple>

#include "importer.h"

#define POPULATION_ALERT_THRESHOLD 20
#define SAMPLELING_FREQUENCY 60 // Hz

class Analyzer {
  public:
    // Keep track of the prevous positions as a class attribute
    static std::vector<personFeature_t> previousPosition;

    static std::vector<int> getSectorCount(size_t gridWidth, size_t gridHeight,
                                        std::vector<personFeature_t> persons) {
      // Initialize sector count vector
      std::vector<int> sectorCounts(gridWidth * gridHeight);

      // Count the number of people in each section
      for (personFeature_t person : persons) {
        // First, we determine which section the person is in
        int xPos = std::get<0>(person.position);
        int yPos = std::get<1>(person.position);
        size_t section = xPos * gridWidth + yPos;
        sectorCounts[section] += 1;
      }

      return sectorCounts;
    }

    // Get the alerted sectors, given a sector count
    static std::vector<size_t> getAlertSectors(std::vector<int> sectorCount) {
      // Initialize the alerted sectors vector
      std::vector<size_t> alertSectors;

      // Determine if any of the sections have alerts
      for (size_t sector = 0; sector < sectorCount.size(); sector++) {
        if (sectorCount[sector] > POPULATION_ALERT_THRESHOLD) {
          alertSectors.push_back(sector);
        }
      }
      return alertSectors;
    }

    // Get the most populated sectors, given sector count
    static std::vector<size_t> getMostPopulatedSectors(std::vector<int> sectorCount) {
      size_t first, second, third = 0;
      int firstVal, secondVal, thirdVal = 0;

      // I'm sorry
      for (int i = 0; i < sectorCount.size(); i++) {
        int thisVal = sectorCount[i];
        if (thisVal > firstVal) {
          thirdVal = secondVal;
          secondVal = firstVal;
          firstVal = thisVal;
          third = second;
          second = first;
          first = i;
        } else if (thisVal > secondVal) {
          thirdVal = secondVal;
          secondVal = thisVal;
          third = second;
          second = i;
        } else if (thisVal > thirdVal) {
          thirdVal = thisVal;
          third = i;
        }
      }
      std::vector<size_t> ranking = {first, second, third};
      return ranking;
    }

    // // Get the average speed of the people,
    // int getAverageSpeed(size_t gridWidth, size_t gridHeight,
    //                     int width, int length, std::vector<personFeature_t> persons) {
    //   std::vector<int> speeds;
    //   for (personFeature_t feature : persons) {
    //     // We'll just use linear approximation for movement, it's o.k.
    //     int prevX = std::get<0>(feature.position) / gridWidth * width;
    //   }
    // }

    // Get the number of people in the room
    static int getPopulation(std::vector<personFeature_t> persons) {
      return persons.size();
    }

};