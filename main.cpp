

//
//  main.cpp
//  ThesisSerialCamShift
//
//  Created by Matthew Perry on 1/27/16.
//  Copyright © 2016 Matthew Perry. All rights reserved.
//

#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/videoio/videoio.hpp"
#include "opencv2/core/core.hpp"

#include "GPU/gpuMain.h"
#include "CPU/cpuMain.hpp"
#include "CPU/RegionOfInterest.hpp"
#include "CPU/UcharSerialCamShift.hpp"
//#include "CPU/cpuMain.hpp"

#include <chrono>

using namespace cv;
using namespace std;
using namespace std::chrono;

VideoWriter _outputVideo;

#define OUTPUTFILENAME "out.mov"
#define MAXTHREADS 3

void parameterCheck(int argCount)
{
    if(argCount != 3)
    {
        cout << "Usage: </path/to/videofile> </path/to/window/file>" << endl;
        exit(-1);
    }
}

/*
 This method was taken from http://docs.opencv.org/2.4/doc/tutorials/highgui/video-write/video-write.html
 */
void openOutputVideo(cv::VideoCapture inputVideo)
{
    int ex = static_cast<int>(inputVideo.get(CV_CAP_PROP_FOURCC));     // Get Codec Type- Int form
    
    // Transform from int to char via Bitwise operators
    char EXT[] = {(char)(ex & 0XFF) , (char)((ex & 0XFF00) >> 8),(char)((ex & 0XFF0000) >> 16),(char)((ex & 0XFF000000) >> 24), 0};
    
    Size S = Size((int) inputVideo.get(CV_CAP_PROP_FRAME_WIDTH),    // Acquire input size
                  (int) inputVideo.get(CV_CAP_PROP_FRAME_HEIGHT));
    
    // Open the output
    
    _outputVideo.open(OUTPUTFILENAME, ex, inputVideo.get(CV_CAP_PROP_FPS), S, true);
    
    if (! _outputVideo.isOpened())
    {
        cout  << "Could not open the output video for write."<< endl;
    }
    else{
        cout << "#########################################" << endl;
        cout << "Input frame resolution: Width=" << S.width << "  Height=" << S.height
        << " of nr#: " << inputVideo.get(CV_CAP_PROP_FRAME_COUNT) << endl;
        cout << "Input codec type: " << EXT << endl;
        cout << "#########################################" << endl;
    }
}

void writeFrame(Mat frame)
{
    _outputVideo.write(frame);
}

uchar * parseSubHueData(Mat hsvMat, RegionOfInterest roi, int * step)
{
    std::vector<cv::Mat> hsv_channels;
    split(hsvMat, hsv_channels);
    Mat hueMatrix = hsv_channels[0];
    Mat subframe = hueMatrix(Rect(roi.getTopLeftX(), roi.getTopLeftY(), roi._width, roi._height));
  //  cout << subframe.total() << " <----  Smaller T O T A L \n";
    *step = subframe.step;
    return subframe.data;
}

uchar * parseHueData(Mat hsvMat, int * step)
{
    std::vector<cv::Mat> hsv_channels;
    split(hsvMat, hsv_channels);
    Mat h_image = hsv_channels[0];
    *step = h_image.step;
    return h_image.data;
}


int main(int argc, const char * argv[])
{
    bool shouldCPU = true;

    high_resolution_clock::time_point t1;
    high_resolution_clock::time_point t2;
    

   // gpuMain(argc, argv);
    
    parameterCheck(argc);
 
    VideoCapture cap(argv[1]);
    openOutputVideo(cap);
    
    
    int i = 0;
    int x, y, x2, y2;
    
    ifstream infile(argv[2]);
    
    //Read in windows from input file
    while (infile >> x >> y >> x2 >> y2)
    {
        cout << x <<  " " << y << " " << x2 << y2 << endl;
    }
    
    
    SerialCamShift camShift;
    
    Mat frame, hsv;
    
    float * histogram = (float * ) calloc(sizeof(float), BUCKETS);

    cap.read(frame);
    
    RegionOfInterest roi(Point(x,y), Point(x2,y2), cap.get(CV_CAP_PROP_FRAME_WIDTH), cap.get(CV_CAP_PROP_FRAME_HEIGHT));
 
    cvtColor(frame, hsv, CV_RGB2HSV);

    int step = 0;
    
    t1 = high_resolution_clock::now();

     uchar * hueArray = parseSubHueData(hsv, roi, &step);
    
    uchar * hueArrayTest = parseHueData(hsv, &step);

    
     cout << "STEP : " << step << endl;
     camShift.createHistogram(hueArray, step, &roi, &frame, &histogram);
    
    t2 = high_resolution_clock::now();
    auto dur2 = duration_cast<microseconds>( t2 - t1 ).count();
    cout << "hist2: " << dur2 / 1000.0 << endl;
    

  //camShift.printHistogram(histogram, BUCKETS);
    
    bool go = true;
    
    
    int prevX = 0;
    int prevY = 0;
    
 /*  do{
         prevX = roi.getCenterX();
        prevY = roi.getCenterY();
        hueArray = parseSubHueData(hsv, roi, &step);
        
           go = camShift.subMeanShiftTest(hueArray, step, &roi, histogram, &prevX, &prevY);
        
    }while(go);
    
   */
    
    
    
 //camShift.meanshift(hueArrayTest, step, &roi, histogram);
    
    
    
  //  camShift.test(hueArrayTest, step, &roi, histogram);
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
   camShift.backProjectHistogram(hueArray, step, &frame, roi, histogram);
    
    roi.drawROI(&frame);
  //  roi.printROI();
    writeFrame(frame);

if(shouldCPU)
{
     while(cap.read(frame))
     {
     
         cvtColor(frame, hsv, CV_RGB2HSV);

         
         t1 = high_resolution_clock::now();
         
         hueArray = parseHueData(hsv, &step);
         
         t2 = high_resolution_clock::now();
         auto duration2 = duration_cast<microseconds>( t2 - t1 ).count();
        
         
       // cout << "Duration of preparing using entire frame: " << duration2 / 1000.0 << endl;
    
         /* Remember to change below back */
         
         
     // camShift.meanshift(hueArray, step, &roi, histogram);
       // camShift.test(hueArray, step, &roi, histogram);

        go = true;
         
       
         
         while(go){
             prevX = roi.getCenterX();
             prevY = roi.getCenterY();
           hueArray = parseSubHueData(hsv, roi, &step);
           go = camShift.subMeanShiftTest(hueArray, step, &roi, histogram, &prevX, &prevY);
       }
    
       
         hueArray = parseSubHueData(hsv, roi, &step);
         camShift.backProjectHistogram(hueArray, step, &frame, roi, histogram);


            roi.drawROI(&frame);
          //  roi.printROI();
            writeFrame(frame);
            
     }//end while
  
	cout << endl << "****END OF CPU Serial MeanShift****" << endl;


    }//end shouldCPU

    _outputVideo.release();
    free(histogram);
    
   return 0;
}
