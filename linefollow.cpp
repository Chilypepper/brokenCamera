#include <opencv2/imgproc/imgproc.hpp>
#include </home/peter/brokenGlasses/src/image_testing/libs/RiptideVision.h>
#include "opencv2/highgui/highgui.hpp"
#include <opencv2/core/core.hpp>
#include <iostream>
#include <ctime>
#include <string>
#define CLOCKS_PER_MS (CLOCKS_PER_SEC / 1000)

using namespace cv;
using namespace std;

struct linePoint{
  Point top;
  Point bot;
};

Mat seperateColors(Mat src, vector<int> colors)
{
  Mat input = src.clone();


  GaussianBlur(input,input,Size(21,21),0);
  Mat imageHSV;
  Mat imgThreshold;

  cvtColor( input, imageHSV, COLOR_BGR2HSV );
  circle(src,Point(500,200),2,Scalar(0,0,255));
  switch(colors[6]){
    case 0:
        inRange(imageHSV, Scalar(colors[0], colors[2], colors[4]), Scalar(colors[1], colors[3], colors[5]), imgThreshold);
    break;

  }
  //Add red secondary threshhold
  circle(imgThreshold,Point(0,0),240,Scalar(0,0,0),-1);
  return imgThreshold;
}

void colorAverage(Mat src, vector<int> colors, Point &averagePoint){
  Mat M = seperateColors(src,colors);
  averagePoint.x = 0;
  averagePoint.y = 0;

  long long int iSum = 0;
  long long int jSum = 0;
  int count = 1;

  for(int i = 0; i < M.cols - 1; i++){
    for(int j = 0; j < M.rows - 1; j++){
      if(M.at<uchar>(j,i) > 0){
        iSum += i;
        jSum += j;
        count++;
      }
    }
  }
  averagePoint.x = iSum / count;
  averagePoint.y = jSum / count;
}
void orientation(Mat src, vector<int> colors, Point averagePoint, linePoint &pair){
  Mat M = seperateColors(src,colors); 

  long long int iSumTop = 0;
  long long int jSumTop = 0;

  long long int iSumBot = 0;
  long long int jSumBot = 0;

  long long int count = 1;
  //**Top half
  for(int i = 0; i < M.cols - 1; i ++){
    for(int j = 0; j < averagePoint.y - 30; j ++){
      if(M.at<uchar>(j,i) > 0){
        iSumTop += i;
        jSumTop += j;
        count++;
      }
    }
  }
  pair.top.x = iSumTop / count;
  pair.top.y = jSumTop / count;

  count = 1;

  //** Bottom half
  for(int i = 0; i < M.cols - 1; i++){
    for(int j = averagePoint.y + 30; j < M.rows - 1; j++){
      //cout << i << " " << j << endl;
      if(M.at<uchar>(j,i) > 0){
        iSumBot += i;
        jSumBot += j;
        count++;
      }
    }
  }
  pair.bot.x = iSumBot / count;
  pair.bot.y = jSumBot / count;

}
void buoyTask(Mat src, RiptideVision::buoyInfo feedback,Mat &drawing){
  Point red;
  colorAverage(src,REDS,red);
  Point green;
  colorAverage(src,GREENS,green);
  Point yellow;
  colorAverage(src,YELLOWS,yellow);
  circle(drawing,red,5,Scalar(125,0,125),-1);
  circle(drawing,green,5,Scalar(125,0,125),-1);
  circle(drawing,yellow,5,Scalar(125,0,125),-1);
}


using namespace cv;
int main(){

  //0 to 180 for opencv
  int hLow = 10 / 2;
  int hUp = 65 / 2;

  // 0 to 255 for opencv
  int sLow = 30;
  int sUp = 150;

  //0 to 255 for oopencv
  int vLow = 150;
  int vUp = 255;

  vector<int> colors(7);
  colors.clear();
  colors.push_back(hLow);
  colors.push_back(hUp);
  colors.push_back(sLow);
  colors.push_back(sUp);
  colors.push_back(vLow);
  colors.push_back(vUp);
  //"is red" bit
  colors.push_back(0);
  String j = "images/";
  int p;

  cin >> p;
  j = j + (char)(p+48) + ".png";
  Mat image = imread(j,1);
  clock_t startTime = clock();
  //Mat seperated = seperateColors(image, colors);
  /************************************************************************************************
  Point k;
  colorAverage(image,colors,k);
  
  linePoint q;
  orientation(image,colors,k,q);
  clock_t endtime = clock();
  Mat seperated2 = seperateColors(image, colors);


  cvtColor(seperated2,seperated2,COLOR_GRAY2BGR);

  circle(seperated2,Point(k.x,k.y),3,Scalar(170,0,120),-1);

  line(seperated2,Point(q.bot.x,q.bot.y),Point(q.top.x,q.top.y), Scalar(255,0,255),2);
  
  cout << (double)(endtime - startTime) / CLOCKS_PER_SEC << endl;
  namedWindow( "sep", CV_WINDOW_AUTOSIZE );
  imshow("sep",seperated2);
  
  *************************************************************************************************/


  RiptideVision::buoyInfo q;
  buoyTask(image,q,image);


  namedWindow( "source", CV_WINDOW_AUTOSIZE );
  imshow("source",image);

  
  waitKey(0);

  return 0;
}