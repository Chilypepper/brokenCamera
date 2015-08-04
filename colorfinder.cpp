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

const int accuracy = 10;
Scalar colorScalar_BLUE = Scalar(255,255,0);
Scalar colorScalar = Scalar(0,255,0);

vector<int> listX;
vector<int> listY;

int main(int argc, char** argv)
{

  Mat im = imread("images/dots2.png", 1);

  clock_t start;
  start=clock();

  if (im.empty())
  {
    cout << "Cannot open image!" << endl;
    return -1;
  }

  cout <<"Image has "<<im.rows<<" rows"<<endl;
  cout <<"Image has "<<im.cols<<" cols"<<endl;
  cout<<(int)im.at<Vec3b>(Point(testPointX,testPointY))[0]<<"\n";
  cout<<(int)im.at<Vec3b>(Point(testPointX,testPointY))[1]<<"\n";
  cout<<(int)im.at<Vec3b>(Point(testPointX,testPointY))[2]<<"\n";
  
  #ifdef findPoint
    circle(im,Point(testPointX,testPointY),5,colorScalar_BLUE,1);
  #endif

  int pointx = 0;
  int pointy = 0;
  int i=0;
  int j=0;
  //BGR
  for(double cols = im.cols; pointx < cols; pointx+=cols/accuracy){
  	pointy=0;
    for(double rows = im.rows; pointy < rows; pointy+=rows/accuracy){
    	if(im.at<Vec3b>(Point(pointx,pointy))[0] == 58 &&
    	   im.at<Vec3b>(Point(pointx,pointy))[1] == 193 &&
    	   im.at<Vec3b>(Point(pointx,pointy))[2] == 82){
    	   	#ifdef findPoint
    		circle(im,Point(pointx,pointy),0,colorScalar_BLUE,1);
    		#endif
    		listX.push_back(pointx);
    		listY.push_back(pointy);
    		j++;
       	}

  	    //circle(im,Point(pointx,pointy), 0,colorScalar_BLUE,1);
    }

  }

  //putText(im,"Heading: ",Point(150,150),FONT_HERSHEY_TRIPLEX,0.25,(255,0,0),2,8,false);

  int largestX = 0;
  int largestY = 0;
  int smallestX = im.cols;
  int smallestY = im.rows;

  int currX=0;
  int currY=0; //#india
  while(listX.size()>0){
    currX=listX.back();
    currY=listY.back();

    if(currX > largestX){
    	largestX=currX;
    }
    if(currY > largestY){
    	largestY=currY;
    }
    if(currX < smallestX){
    	smallestX = currX;
    }
    if(currY < smallestY){
    	smallestY = currY;
    }
  	listX.pop_back();
  	listY.pop_back();
  	i++;
  }
  cout<<"Read "<<i<<" Points"<<endl;
  cout<<"Number of points written: "<<j<<endl;
  cout<<"Bottom-right most point: "<<largestX<<"  "<<largestY<<endl;
  cout<<"Top-left most point: "<<smallestX<<"  "<<smallestY<<endl;
  rectangle(im,Point(largestX,largestY),Point(smallestX,smallestY),colorScalar_BLUE,5,8,0);

  cout<<"Time: "<<(clock()-start)/(double)(CLOCKS_PER_SEC/1000)<<" ms\n";
  imshow("image", im);
  waitKey(0);

  return 0;
}
