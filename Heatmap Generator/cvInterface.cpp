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

const float inlier_threshold = 2.5f; // Distance threshold to identify inliers with homography check
const float nn_match_ratio = 0.8f;   // Nearest neighbor matching ratio

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

    static std::vector<cv::Vec2f> getMovements(std::string source, cv::Mat* prevFrame) {
                                              //  std::vector<cv::KeyPoint> prevKeyFrame) {
      std::vector<cv::Vec2f> movements;
      // Load in the new image
      cv::Mat image = cv::imread(
                cv::samples::findFile("mvt2.jpg"), cv::IMREAD_COLOR);
      cv::Mat original = image.clone();

      cv::cvtColor(image, image, cv::COLOR_BGR2GRAY);
      cv::Canny(image, image, 100, 200);
      cv::Mat temp = *prevFrame;

      cv::cvtColor(temp, temp, cv::COLOR_BGR2GRAY);
      cv::Canny(temp, temp, 100, 200);
      cv::Mat toSub = image.clone();
      cv::subtract(temp, image, image);
      cv::subtract(toSub, temp, temp);
      *prevFrame = temp;

      cv::Mat homography;
      // cv::FileStorage fs(cv::samples::findFile(std::parser.get<String>("@homography") ), cv::FileStorage::READ);
      // fs.getFirstTopLevelNode() >> homography;

      std::vector<cv::KeyPoint> kpts1, kpts2;
      cv::Mat desc1, desc2;

      cv::Ptr<cv::AKAZE> akazeTool = cv::AKAZE::create();
      akazeTool->detectAndCompute(image, cv::noArray(), kpts1, desc1);
      akazeTool->detectAndCompute(*prevFrame, cv::noArray(), kpts2, desc2);

      cv::BFMatcher matcher(cv::NORM_L1);
      std::vector<std::vector<cv::DMatch>> nn_matches;
      matcher.knnMatch(desc1, desc2, nn_matches, 2);

      std::vector<cv::KeyPoint> matched1, matched2;
      for (size_t i = 0; i < nn_matches.size(); ++i) {
        cv::DMatch first = nn_matches[i][0];
        float dist1 = nn_matches[i][0].distance;
        float dist2 = nn_matches[i][1].distance;
        if(dist1 < nn_match_ratio * dist2) {
            matched1.push_back(kpts1[first.queryIdx]);
            matched2.push_back(kpts2[first.trainIdx]);
        }
      }

      std::vector<cv::DMatch> goodMatches;
      std::vector<cv::KeyPoint> inliers1, inliers2;
      for (size_t i = 0; i < matched1.size(); i++) {
        cv::Mat col = cv::Mat::ones(3, 1, CV_64F);
        col.at<double>(0) = matched1[i].pt.x;
        col.at<double>(1) = matched1[i].pt.y;

        // col = homography * col;
        col /= col.at<double>(2);
        double dist = sqrt(pow(col.at<double>(0) - matched2[i].pt.x, 2) +
                          pow(col.at<double>(1) - matched2[i].pt.y, 2));

        if (1.5f < dist < inlier_threshold) {
          int new_i = static_cast<int>(inliers1.size());
          inliers1.push_back(matched1[i]);
          inliers2.push_back(matched2[i]);
          goodMatches.push_back(cv::DMatch(new_i, new_i, 0));
        }
      }
      cv::Mat res;
      cv::drawMatches(image, inliers1, *prevFrame, inliers2, goodMatches, res);
      imwrite("akaze_result.png", res);
      double inlier_ratio = inliers1.size() / (double) matched1.size();
      std::cout << "A-KAZE Matching Results" << std::endl;
      std::cout << "*******************************" << std::endl;
      std::cout << "# Keypoints 1:                        \t" << kpts1.size() << std::endl;
      std::cout << "# Keypoints 2:                        \t" << kpts2.size() << std::endl;
      std::cout << "# Matches:                            \t" << matched1.size() << std::endl;
      std::cout << "# Inliers:                            \t" << inliers1.size() << std::endl;
      std::cout << "# Inliers Ratio:                      \t" << inlier_ratio << std::endl;
      std::cout << std::endl;
      imshow("result", res);
      *prevFrame = original;
      cv::waitKey();
    }

};

int main() {
  cv::Mat tenSecDiff = cv::imread(
                cv::samples::findFile("mvt1.jpg"), cv::IMREAD_COLOR);
  // cvInterface::getPersonPoints("hello", tenSecDiff);
  cvInterface::getMovements("hello", &tenSecDiff);

}