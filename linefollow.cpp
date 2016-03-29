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

void seperateColors(Mat src, vector<int> colors, Mat& seperated)
{
  Mat input = src.clone();


  GaussianBlur(input,input,Size(21,21),0);
  Mat imageHSV;
  Mat imgThreshold;

  cvtColor( input, imageHSV, COLOR_BGR2HSV );
  circle(src,Point(410,390),2,Scalar(0,0,255));
  //cout << (int)imageHSV.at<Vec3b>(Point(410,390))[1] << " hue"<< endl;
  switch(colors[6]){
    case 0:
        inRange(imageHSV, Scalar(colors[0], colors[2], colors[4]), Scalar(colors[1], colors[3], colors[5]), imgThreshold);
    break;

  }
  //Add red secondary threshhold
  //circle(imgThreshold,Point(0,0),240,Scalar(0,0,0),-1);
  cvtColor( imgThreshold, imgThreshold, COLOR_GRAY2BGR);
  seperated = imgThreshold;
}

void colorAverage(Mat src, vector<int> colors, Point &averagePoint){
  Mat M;
  seperateColors(src,colors, M);
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
  Mat M;
  seperateColors(src,colors, M); 

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
void orientationv2(Mat src, vector<int> colors, linePoint& orientation, Mat& drawing){
	Mat seperated;
	seperateColors(src,colors, seperated);
	vector<vector<Point> > contours;
	//Mat wContours = src.clone();

	cvtColor(seperated,seperated,COLOR_BGR2GRAY);
	findContours(seperated,contours,0,1);

	int MAX_CONT;
	float area = 0;
	for(int i = 0; i < contours.size(); i++){
		float area2 = contourArea(contours[i]);
		if(area2 > area){
			MAX_CONT = i;
			area  = area2;
		}
	}
	vector<Point> minContours;
	double epsilon = .1 * arcLength(contours[MAX_CONT],true);
	approxPolyDP(contours[MAX_CONT], minContours, epsilon,true);

	if(drawing.data){

		Mat wContours;
		seperateColors(src,colors, wContours);

		for(int j = 0; j < contours[MAX_CONT].size(); j++){
			circle(wContours,contours[MAX_CONT][j],2,Scalar(0,0,255));
		}	

		for(int j = 0; j < minContours.size(); j++){
			circle(wContours,minContours[j],5,Scalar(255,0,255));
		}
		cout <<"\n\n" << minContours.size() <<"\n\n"<< endl;
		line(wContours,minContours[0],minContours[1],Scalar(255,0,255),2);
		cout <<"Does it ";
		drawing = wContours;
		cout <<"Break?" << endl;
	}
	if(minContours.size() == 2){
		orientation.top = minContours[0];
		orientation.bot = minContours[1];
	}
}
void orientationv3(Mat src, vector<int> colors, linePoint& orientation, Mat& drawing){
  bool big = true;
  Mat seperated;
  seperateColors(src,colors, seperated);
  vector<vector<Point> > contours;
  Mat wContours = src.clone();

  cvtColor(seperated,seperated,COLOR_BGR2GRAY);
  findContours(seperated,contours,0,1);

  int MAX_CONT = -1;
  float area = 0;
  for(int i = 0; i < contours.size(); i++){
    float area2 = contourArea(contours[i]);
    if(area2 > area){
      MAX_CONT = i;
      area  = area2;
    }
  }
  if(MAX_CONT >= 0){
      double epsilon = 0.1 * arcLength(contours[MAX_CONT],true);

      Vec4f bfline;
      fitLine(contours[MAX_CONT], bfline, CV_DIST_L2,0, 0.01, 0.01);

      if(drawing.data){
        Mat wContours;
        seperateColors(src,colors, wContours);

        for(int j = 0; j < contours[MAX_CONT].size(); j++){
          circle(wContours,contours[MAX_CONT][j],2,Scalar(0,0,255));
        }
        circle(wContours,Point(bfline[0],bfline[1]),2,PINK);
        circle(wContours,Point(bfline[2],bfline[3]),2,PINK);
        if(!big) line(wContours,Point(bfline[2],bfline[3]),Point(bfline[2]+bfline[0]*100,bfline[3]+bfline[1]*100),Scalar(255,0,255),2);
        else{
     		line(wContours,Point(bfline[2],bfline[3]),Point(bfline[2]+bfline[0]*1000,bfline[3]+bfline[1]*1000),Scalar(255,0,255),2);   	
     	    line(wContours,Point(bfline[2],bfline[3]),Point(bfline[2]+bfline[0]*-1000,bfline[3]+bfline[1]*-1000),Scalar(255,0,255),2);
        }
        drawing = wContours;
    }
  }
}
void buoyTask(Mat src, RiptideVision::buoyInfo feedback,Mat &drawing){
  Point red;
  colorAverage(src,REDS,red);
  Point green;
  colorAverage(src,GREENS,green);
  Point yellow;
  colorAverage(src,YELLOWS,yellow);


  circle(drawing,red,3,PINK,-1);
  circle(drawing,green,3,PINK,-1);
  circle(drawing,yellow,3,PINK,-1);
}
//target is clockwise 0-3
void torpedoTask(Mat src, vector<int> colors, int target, Mat &drawing){
	bool maxX;
	bool maxY;

	switch(target){
		case 0:
			maxX = true;
			maxY = false;
		break;
		case 1:
			maxX = true;
			maxY = true;
		break;
		case 2:
			maxX = false;
			maxY = true;
		break;
		case 3:
			maxX = false;
			maxY = false;
		break;
	}
	Mat seperated;
	seperateColors(src,colors, seperated);
	vector<vector<Point> > contours;

	cvtColor(seperated,seperated,COLOR_BGR2GRAY);
	findContours(seperated,contours,0,1);
	if(contours.size() == 4){
		Mat wContours = seperated.clone();
		cvtColor(wContours, wContours, COLOR_GRAY2BGR);

		vector<Point> largest1;
		int sizes[4];

		for(int q = 0; q < 4; q++){
			float area = 0;
			int largest;
			for(int i = 0; i < contours.size(); i++){
				float area2 = contourArea(contours[i]);
				if(area2 > area){
					area = area2;
					int largest = i;
				}
			}
			sizes[q] = largest;
		}
		if(drawing.data){
			int i = 255;
			for(int p = 0; p < 4; p++){
				Scalar red = Scalar(0,0,i);
				drawContours(wContours,contours[sizes[p]], -1, red);
				i -= 30;
			}		
		}
		drawing = wContours;
	}






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
  Mat image = imread("images/frame0003.jpg",1);
  cout << j << endl;
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
  
  /*Mat yerrow = image.clone();
  yerrow = seperateColors(yerrow,GREENS);


  RiptideVision::buoyInfo q;
  buoyTask(image,q,image);
  */

  if(image.empty()){
  	cout <<"Empty image" << endl;
  }
  else{
  	  namedWindow( "src", CV_WINDOW_AUTOSIZE );
 	 imshow("src",image);
  }
  cout << "1";
  Mat drawing = image;
  cout << "2";
  linePoint orientation;
  seperateColors(drawing,YELLOWS,drawing);
  orientationv3(image,YELLOWS,orientation,drawing);
  //torpedoTask(image,REDS, 1, drawing);
    cout << "3";

  if(drawing.data){
	  namedWindow( "source", CV_WINDOW_AUTOSIZE );
	  imshow("source", drawing);
  }

  
  waitKey(0);

  return 0;
}