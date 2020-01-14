/*
    Processes the people features
    */

#include <tuple>
#include <time.h>

typedef struct {
  size_t personId;
  time_t birthTime;
  std::tuple<size_t, size_t> position;
} personFeature_t;


class personProcessor {
  public:

    static size_t peopleCount;
    // TODO MAKE A PERSON
    static personFeature_t makePerson();

    // Set the birth time to be the current time
    static void setAgeSeconds(personFeature_t* person) {
      if (person != nullptr) {
        person->birthTime = time(NULL);
      }
    }

    // Get the age of the feature in seconds
    static float getAgeSeconds(personFeature_t person) {
      time_t timer = time(NULL);
      return difftime(timer, person.birthTime);
    }

    // Set the ID to be the next value
    static void setId(personFeature_t* person) {
      person->personId = personProcessor::peopleCount;
      personProcessor::peopleCount++;
    }

    // Get the ID of the person
    static size_t getId(personFeature_t person) {
      return person.personId;
    }

    // Set the position of the person
    static void setPosition(size_t x, size_t y, personFeature_t* person) {
      if (person != nullptr) {
        std::tuple<size_t, size_t> newPosition(x, y);
        person->position = newPosition;
      }
    }

    static std::tuple<size_t, size_t> getPosition(personFeature_t person) {
      return person.position;
    }
};