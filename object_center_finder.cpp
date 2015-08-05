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

vector<Point> pointList;
vector<Point> pointList2;


bool foundEdge=false;

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

Point findCenter(Mat im, Point A, Point B, Point C){
	Point center;
    float yDelta_a = B.y - A.y;
    float xDelta_a = B.x - A.x;
    float yDelta_b = C.y - B.y;
    float xDelta_b = C.x - B.x;

	float aSlope = yDelta_a/xDelta_a;
    float bSlope = yDelta_b/xDelta_b;  

    center.x = 0;
    center.y = 0;

    center.x = (aSlope*bSlope*(A.y - C.y) + bSlope*(A.x + B.x)- aSlope*(B.x+C.x) )/(2* (bSlope-aSlope) );
    if(aSlope ==0){
    	center.y = -1 * (center.x - (B.x + C.x) / 2) / bSlope +  (B.y + C.y) / 2;
    }
    else{
    	center.y = -1*(center.x - (A.x+B.x)/2)/aSlope +  (A.y+B.y)/2;
    }

	return center;
}
int findDistance(int p1x, int p1y, int p2x, int p2y){
	double dx = abs(p1x - p2x);
	double dy = abs(p1y - p2y);
	return sqrt(pow(dx,2.0)+pow(dy,2.0));
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
  
  while(currX < im.cols && !foundEdge){
  	if(colorDifference(im,currX,currY,currX+5,currY) > 200){
  		pointList.push_back(Point(currX+5,currY));
  		foundEdge=true;
  	}
  	currX++;
  }

  foundEdge=false;
  currX=centerX;
  while(currX > 5 && !foundEdge){
  	if(colorDifference(im,currX,currY,currX-5,currY) > 200){
  		pointList.push_back(Point(currX-5,currY));
  		foundEdge=true;
  	}
  	currX--;
  }

  foundEdge=false;
  currX = centerX;
  while(currY < im.rows - 5 && !foundEdge){
  	if(colorDifference(im,currX,currY,currX,currY+5) > 200){
  		pointList.push_back(Point(currX,currY+5));
  		foundEdge=true;
  	}
  	currY++;
  }
  
  while(pointList.size() > 0){
  	Point individual = pointList.back();
  	int x = individual.x;
  	int y = individual.y;
  	circle(im,Point(x,y),0,colorScalar_RED,2);
  	pointList2.push_back(individual);
  	pointList.pop_back();
  }
  cout<<"Finding circle: Time: "<<(clock()-start)/(double)(CLOCKS_PER_SEC/1000)<<" ms\n";

  Point p1 = pointList2.back();
  pointList2.pop_back();
  Point p2 = pointList2.back();
  pointList2.pop_back();
  Point p3 = pointList2.back();
  pointList2.pop_back();
  
  Point centre = findCenter(im,p1,p2,p3);
  cout<<"Time: "<<(clock()-start)/(double)(CLOCKS_PER_SEC/1000)<<" ms\n";

  double radius = findDistance(p1.x,p1.y,centre.x,centre.y);
  circle(im,centre,radius,colorScalar_RED,1);
  cout<<centre<<" "<<radius<<endl;

  cout<<"Time: "<<(clock()-start)/(double)(CLOCKS_PER_SEC/1000)<<" ms\n";
  imshow("image", im);
  waitKey(0);

  return 0;
}