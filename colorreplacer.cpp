#include "opencv2/highgui/highgui.hpp"
#include <opencv2/core/core.hpp>
#include <iostream>
#include <cv.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <ctime>

using namespace cv;
using namespace std;
const int accuracy = 50;

int main(int argc, char** argv)
{

  Mat im = imread("images/colortest.jpg", 1);

  clock_t start;
  start=clock();

  if (im.empty())
  {
    cout << "Cannot open image!" << endl;
    return -1;
  }
  //cout <<"Image has "<<im.rows<<" rows"<<endl;
  //cout <<"Image has "<<im.cols<<" cols"<<endl;
  int pointx = 0;
  int pointy = 0;

  Scalar colorScalar = Scalar(0,255,0);
  Scalar colorScalar_BLUE = Scalar(255,255,255);

  for(double cols = im.cols; pointx < cols; pointx+=1){
  	pointy=0;
    for(double rows = im.rows; pointy < rows; pointy+=1){
      if(im.at<Vec3b>(Point(pointx,pointy))[0] < 120 &&
      	 im.at<Vec3b>(Point(pointx,pointy))[1] > 180 &&
      	 im.at<Vec3b>(Point(pointx,pointy))[2] < 100){
  	    circle(im,Point(pointx,pointy), 0,colorScalar_BLUE,1);
  	    //cout<<"y="<<pointy<<"\n";
  	    //cout<<"x="<<pointx<<"\n";
      }
    }

  }
  cout<<"Time: "<<(clock()-start)/(double)(CLOCKS_PER_SEC/1000)<<" ms\n";
  imshow("image", im);
  waitKey(0);

  return 0;
}