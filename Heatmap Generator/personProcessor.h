/*
    Process personFeatures
    */

#include "personProcessor.cpp"

// typedef struct {
//   size_t personId;
//   size_t ageSeconds;
//   std::tuple<size_t, size_t> position;
// } personFeature_t;

class personProcessor {
  static personFeature_t makePerson();
  static float getAgeSeconds(personFeature_t person);
  static size_t getId(personFeature_t person);
  static void setPosition(personFeature_t* person);
  static std::tuple<size_t, size_t> getPosition(personFeature_t person);
};