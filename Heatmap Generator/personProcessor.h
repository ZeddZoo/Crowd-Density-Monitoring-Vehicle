/*
    Process personFeatures
    */
#include <tuple>

typedef struct {
  size_t personId;
  size_t ageSeconds;
  std::tuple<size_t, size_t> position;
} personFeature_t;

class personProcessor {
  public:
    static personFeature_t makePerson();
    static void setAgeSeconds(personFeature_t* person);
    static float getAgeSeconds(personFeature_t person);
    static void setId(personFeature_t* person);
    static size_t getId(personFeature_t person);
    static void setPosition(size_t x, size_t y, personFeature_t* person);
    static std::tuple<size_t, size_t> getPosition(personFeature_t person);
};