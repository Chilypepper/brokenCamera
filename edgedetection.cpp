#include "opencv2/highgui/highgui.hpp"
#include <opencv2/core/core.hpp>
#include <iostream>
#include <cv.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <ctime>
#undef findPoint

using namespace cv;
using namespace std;

/// Global variables

Mat dst, detected_edges;

int edgeThresh = 1;
int lowThreshold;
int const max_lowThreshold = 100;
int ratio = 3;
int kernel_size = 3;
/** @function main */
int main( int argc, char** argv )
{
  /// Load an image
  Mat src = imread("images/dots2.png",1);
  Mat src_gray;

  if(src.empty())
  { 
    cout<<"Nothing found"<<endl;
    return -1;
  }

  /// Create a matrix of the same type and size as src (for dst)
  dst.create( src.size(), src.type() );

  /// Convert the image to grayscale
  cvtColor( src, src_gray, CV_BGR2GRAY );

  /// Create a window
  namedWindow( "edges", CV_WINDOW_AUTOSIZE );

  /// Create a Trackbar for user to enter threshold
  blur( src_gray, detected_edges, Size(3,3) );
  Canny( detected_edges, detected_edges, lowThreshold, lowThreshold*ratio, kernel_size );
  dst = Scalar::all(0);
  src.copyTo( dst, detected_edges);
  imshow("edges",dst);
  /// Show the image
  /// Wait until user exit program by pressing a key
  waitKey(0);

  return 0;
  }