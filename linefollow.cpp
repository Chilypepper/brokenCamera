#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <ctime>
#include "geometry_msgs/Vector3.h"
#include "std_msgs/Float32.h"
#include <math.h>
#include "imu_3dm_gx4/FilterOutput.h"
#include "sensor_msgs/Joy.h"
#include <std_msgs/Float32MultiArray.h>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

Mat src; Mat src_gray, canny_output;
int thresh = 100;
int max_thresh = 255;
RNG rng(12345);
vector<vector<Point> > contours;
vector<Vec4i> hierarchy;

/** @function main */
int main( int argc, char** argv )
{
  /// Load source image and convert it to gray
  src = imread( argv[1], 1 );
  /// Convert image to gray and blur it
  cvtColor( src, src_gray, CV_BGR2GRAY );
  blur( src_gray, src_gray, Size(3,3) );

  /// Create Window
  char* source_window = "Source";
  Canny( src_gray, canny_output, thresh, thresh*2, 3 );
  //CONVERT BACK TO COLOR
  findContours( canny_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
  cvtColor(canny_output, canny_output, CV_GRAY2RGB);

  if(contours.size() > 0){
    //cout << "\n\n Found "<< contours.size() << " contours\n\n" << endl;
    for(int i = 0; i < contours[0].size(); i++){
      //out << "\t Found "<< contours[0].size() << " contours\n" << endl;

      circle(canny_output, contours[0][i], 2, Scalar(0,0,255));
    }
  }
  if(contours.size() > 1){
    //cout << "\n\n Found "<< contours.size() << " contours\n\n" << endl;
    for(int i = 0; i < contours[1].size(); i++){
      //out << "\t Found "<< contours[0].size() << " contours\n" << endl;

      circle(canny_output, contours[1][i], 2, Scalar(255,255,0));
    }
  }
  
  imshow( "Contours", canny_output );


  waitKey(0);
  return(0);
}