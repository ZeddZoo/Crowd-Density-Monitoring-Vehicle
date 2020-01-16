/*
    Interface for using openCV
    */

#include "cvInterface.cpp"

class cvInterface {
  public:
    // Returns a vector of all the people in the image as Point2f
    static std::vector<cv::Point2f> getPersonPoints(std::string source, cv::Mat tenSecDiff);

    // Returns a vector of all the diff'd feature points
    // static std::vector<
};