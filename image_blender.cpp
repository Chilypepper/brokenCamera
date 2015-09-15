#include "opencv2/highgui/highgui.hpp"
#include <opencv2/core/core.hpp>
#include <iostream>
#include <cv.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <ctime>
#undef findPoint

using namespace cv;
using namespace std;

const int testPointX=200;
const int testPointY=100;

const int accuracy = 200;
Scalar colorScalar_BLUE = Scalar(255,255,0);
Scalar colorScalar = Scalar(0,255,0);

vector<int> listX;
vector<int> listY;

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
  bool cAse = false;
  if(cAse){
    Scalar red = Scalar(127,127,127);
    Mat rect = im(Rect(10,10,118,27));
    Mat color(rect.size(), CV_8UC3, Scalar(85,85,85));
    double alpha = 0.85;
    addWeighted(color,alpha,rect,1.0-alpha,0.0,rect);
    putText(rect,"Target Located",Point(5,17),2,0.45,red,1);
    imshow("image", im);
  }
  else{
    Scalar red = Scalar(255,255,255);
    Mat rect = im(Rect(10,10,118,27));
    Mat color(rect.size(), CV_8UC3, Scalar(0,0,255));
    double alpha = 0.85;
    addWeighted(color,alpha,rect,1.0-alpha,0.0,rect);
    putText(rect,"Target Located",Point(5,17),2,0.45,red,1);
    imshow("image", im);
  }

  cout<<"Time: "<<(clock()-start)/(double)(CLOCKS_PER_SEC/1000)<<" ms\n";
  waitKey(0);

  return 0;
}
