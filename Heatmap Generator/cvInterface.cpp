/*
    Implementation of the CV interface
    */
#include <iostream>
#include <stdio.h>
#include <opencv2/features2d.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/video/background_segm.hpp>
#include <vector>

#define TODISP image

// #include "opencv/modules/features2d/include/opencv2/features2d.hpp"
// #include "opencv/modules/highgui/include/opencv2/highgui.hpp"
// #include "opencv/modules/imgproc/include/opencv2/imgproc.hpp"

class cvInterface {
  public:
    // Returns a vector of all the people in the image as Point2f
    static std::vector<cv::Point2f> getPersonPoints(std::string source, cv::Mat tenSecDiff) {
      std::vector<cv::Point2f> persons;
      // Load in the image from the source
      // TODO@{BEN}: Make the loading in of the image make sense
      cv::Mat image = cv::imread(
                cv::samples::findFile("test7HD.jpg"), cv::IMREAD_COLOR);
      cv::Mat original = image.clone();
      cv::subtract(tenSecDiff, image, image);
      image.convertTo(image, -1, 1.25, -10);
      double alpha = 1.1;
      cv::Mat black = cv::Mat(image.rows, image.cols, image.type(), 0.0);
      double beta = (1.0 - alpha);
      cv::addWeighted(image, alpha, black, beta, 0.0, image);
      cv::cvtColor(image, image, cv::COLOR_BGR2GRAY);
      // Let's see if approximating heads as circles works

      std::vector<cv::Vec3f> circles;

      cv::GaussianBlur(image, image, cv::Size(5, 5), 1, 1);

      // Apply edge detection to eliminate uneeded details
      cv::Canny(image, image, 100, 200);
      // Filter out uneeded horizontal and vertical edges
      // cv::Mat horizontal = image.clone();
      // cv::Mat vertical = image.clone();

      // // Specify size on horizontal axis
      // int horizontal_size = horizontal.cols / 30;
      // // Create structure element for extracting horizontal lines through morphology operations
      // cv::Mat horizontalStructure = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(horizontal_size, 1));
      // erode(horizontal, horizontal, horizontalStructure, cv::Point(-1, -1));
      // dilate(horizontal, horizontal, horizontalStructure, cv::Point(-1, -1));

      // // Specify size on vertical axis
      // int vertical_size = vertical.rows / 30;
      // // Create structure element for extracting vertical lines through morphology operations
      // cv::Mat verticalStructure = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(1, vertical_size));
      // // Apply morphology operations
      // erode(vertical, vertical, verticalStructure, cv::Point(-1, -1));
      // dilate(vertical, vertical, verticalStructure, cv::Point(-1, -1));
      // image = image - vertical - horizontal;

      // cv::Mat element = cv::getStructuringElement(0, cv::Size(1, 1), cv::Point(0, 0));
      // cv::Mat lines;
      // // cv::morphologyEx(image, image, cv::MORPH_OPEN, element);
      // cv::morphologyEx(image, image, cv::MORPH_OPEN, element);
      // image -= lines;
      // // cv::subtract(element, lines, element);
      // cv::Mat imageStructure = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(3, 3));

      // cv::morphologyEx(image, image, cv::MORPH_CLOSE, imageStructure);
      // cv::erode(image, image, imageStructure, cv::Point(-1, -1));
      // cv::dilate(image, image, imageStructure, cv::Point(-1, -1));
      // cv::erode(image, image, imageStructure, cv::Point(-1, -1));


      cv::HoughCircles(image, circles, cv::HOUGH_GRADIENT, 1, image.rows/10, 250, 20, 25, 60);



      // Draw the circles detected and add their points to the person vector
      for( size_t i = 0; i < circles.size(); i++ )
      {
          cv::Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
          persons.push_back(cv::Point2f(cvRound(circles[i][0]), cvRound(circles[i][1])));
          int radius = cvRound(circles[i][2]);
          // circle center
          cv::circle(TODISP, center, 3, cv::Scalar(0,0,255), -1, 8, 0 );
          // circle outline
          cv::circle(TODISP, center, radius, cv::Scalar(255,255,0), 2, 8, 0 );
      }

      // / Show your results
      // Scale the result
      double sF = .5;
      cv::resize(TODISP, TODISP, cv::Size(TODISP.cols * sF, TODISP.rows * sF), 0, 0, cv::INTER_LINEAR);
      cv::namedWindow( "Hough Circle Transform Demo", cv::WINDOW_AUTOSIZE );
      cv::imshow( "Hough Circle Transform Demo", TODISP);

      cv::waitKey(0);
    }
};

int main() {
  cv::Mat tenSecDiff = cv::imread(
                cv::samples::findFile("test6HD.jpg"), cv::IMREAD_COLOR);
  cvInterface::getPersonPoints("hello", tenSecDiff);
}