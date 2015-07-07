#include "opencv2/highgui/highgui.hpp"
#include <opencv2/core/core.hpp>
#include <iostream>
#include <cv.h>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
using namespace std;

int main(int argc, char** argv)
{
  Mat im = imread(argc == 2 ? argv[1] : "images/mario.jpg", 1);
  if (im.empty())
  {
    cout << "Cannot open image!" << endl;
    return -1;
  }
  Point center;
  center.x=0;
  center.y=0;

  circle( im, Point( 200, 200 ), 32.0, Scalar( 0, 0, 255 ), 1, 8 );
  imshow("image", im);
  waitKey(0);

  return 0;
}