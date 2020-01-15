/*
    Implementation of the CV interface
    */
#include <opencv2/features2d.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <vector>

// #include "opencv/modules/features2d/include/opencv2/features2d.hpp"
// #include "opencv/modules/highgui/include/opencv2/highgui.hpp"
// #include "opencv/modules/imgproc/include/opencv2/imgproc.hpp"

class cvInterface {
  public:
    // Returns a vector of all the people in the image as Point2f
    static std::vector<cv::Point2f> getPersonPoints(std::string source) {
      std::vector<cv::Point2f> persons;
      // Load in the image from the source
      // TODO@{BEN}: Make the loading in of the image make sense
      cv::Mat image = cv::imread(
                cv::samples::findFile("test.jpg"), cv::IMREAD_GRAYSCALE);
      // Let's see if approximating heads as circles works
      std::vector<cv::Vec3f> circles;
      cv::HoughCircles(image, circles, cv::HOUGH_PROBABILISTIC, 1, image.rows/8, 200, 100, 0, 0);

      cv::GaussianBlur(image, image, cv::Size(9, 9), 2, 2 );

      /// Draw the circles detected and add their points to the person vector
      for( size_t i = 0; i < circles.size(); i++ )
      {
          cv::Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
          persons.push_back(cv::Point2f(cvRound(circles[i][0]), cvRound(circles[i][1])));
          int radius = cvRound(circles[i][2]);
          // circle center
          cv::circle( image, center, 3, cv::Scalar(0,255,0), -1, 8, 0 );
          // circle outline
          cv::circle( image, center, radius, cv::Scalar(0,0,255), 3, 8, 0 );
      }

      /// Show your results
      cv::namedWindow( "Hough Circle Transform Demo", cv::WINDOW_AUTOSIZE );
      cv::imshow( "Hough Circle Transform Demo", image );

      cv::waitKey(0);
    }
};

int main() {
  cvInterface::getPersonPoints("hello");
}