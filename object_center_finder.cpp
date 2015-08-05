#include "opencv2/highgui/highgui.hpp"
#include <opencv2/core/core.hpp>
#include <iostream>
#include <cv.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <ctime>
#include <math.h>

#undef findPoint

using namespace cv;
using namespace std;

const int testPointX=200;
const int testPointY=100;

const int accuracy = 200;
//BGR
Scalar colorScalar_RED = Scalar(0,0,255);
Scalar colorScalar = Scalar(0,255,0);

vector<int> listX;
vector<int> listY;

int colorDifference(Mat im, int p1x, int p1y, int p2x, int p2y){
  int p1r = (int)im.at<Vec3b>(Point(p1x,p1y))[0];
  int p1g = (int)im.at<Vec3b>(Point(p1x,p1y))[1];
  int p1b = (int)im.at<Vec3b>(Point(p1x,p1y))[2];

  int p2r = (int)im.at<Vec3b>(Point(p2x,p2y))[0];
  int p2g = (int)im.at<Vec3b>(Point(p2x,p2y))[1];
  int p2b = (int)im.at<Vec3b>(Point(p2x,p2y))[2];

  p1r = pow(p1r - p2r,2.0);
  p1g = pow(p1g - p2g,2.0);
  p1b = pow(p1b - p2b,2.0);

  cout<<sqrt(p1r+p1g+p1b)<<endl;
  return sqrt(p1r+p1g+p1b);

}


int main(int argc, char** argv)
{

  Mat im = imread("images/dots2.png", 1);
  
  if(accuracy > im.rows || accuracy > im.cols){
  	cout<<"ERROR: acc cant be > rows or cols"<<endl;
  }
  clock_t start;
  start=clock();

  if (im.empty())
  {
    cout << "Cannot open image!" << endl;
    return -1;
  }

  cout <<"Image has "<<im.rows<<" rows"<<endl;
  cout <<"Image has "<<im.cols<<" cols"<<endl;
  int centerX = 200;
  int centerY = 100;
  int currX = centerX;
  int currY = centerY;
  while(currX < im.cols){
  	if(colorDifference(im,currX,currY,currX+5,currY) > 200){
  		listX.push_back(currX);
  		listY.push_back(currY);
  	}
  	currX++;
  }
  while(listX.size() > 0){
  	int x = listX.back();
  	int y = listY.back();
  	circle(im,Point(x,y),3,colorScalar_RED,2);
  	listX.pop_back();
  	listY.pop_back();
  }
    circle(im,Point(centerX,centerY),5,colorScalar_RED,1);


  cout<<"Time: "<<(clock()-start)/(double)(CLOCKS_PER_SEC/1000)<<" ms\n";
  imshow("image", im);
  waitKey(0);

  return 0;
}