

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

Point topLeft, bottomRight;

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

int main(int argc, const char * argv[])
{
    high_resolution_clock::time_point t1;
    high_resolution_clock::time_point t2;
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
   // gpuMain(argc, argv);
    
    parameterCheck(argc);
    
	//cpuMain(argc, argv);
 
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
    
    
    topLeft = Point(x,y);
    bottomRight = Point(x2,y2);
    
    
    Mat frame, hsv;
    
    double * histogram = (double * ) calloc(sizeof(double), BUCKETS);
    
    cap.read(frame);
    
    
    RegionOfInterest roi(topLeft, bottomRight, cap.get(CV_CAP_PROP_FRAME_WIDTH), cap.get(CV_CAP_PROP_FRAME_HEIGHT));
 
    
    cvtColor(frame, hsv, CV_RGB2HSV);
    
    //This section for 1-D uchar
    std::vector<cv::Mat> hsv_channels;
    cv::split(hsv, hsv_channels);
    cv::Mat h_image = hsv_channels[0];
    int size = h_image.rows * h_image.cols;
    uchar * hueArray = h_image.data;
    int step = h_image.step;
    
    
    camShift.createHistogram(hueArray, step, &roi, &frame, &histogram);
    
    //camShift.printHistogram(histogram, BUCKETS);
    
   camShift.backProjectHistogram(hsv, &frame, roi, histogram);
    
    roi.drawROI(&frame);
  //  roi.printROI();
    writeFrame(frame);
    
 while(cap.read(frame))
 {
        //This is to test instead of using the loop
   //     cap.read(frame);
    
    /* Testing a sub matrix approach */
    
    
    
  //  cout << subframe.total() <<  " <---- TOTAL!!!!" << endl;
 /*   Mat subhsv;
    
    
 
    
   
    cvtColor(frame, subhsv, CV_RGB2HSV);
    
      t1 = high_resolution_clock::now();
 Mat subframe = subhsv(Rect(roi.getTopLeftX(),roi.getTopLeftY(),roi._width,roi._height));


    std::vector<cv::Mat> subhsv_channels;
    split(subframe, subhsv_channels);
    Mat subh_image = subhsv_channels[0];
    uchar *  subhueArray = subh_image.data;


    t2 = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>( t2 - t1 ).count();
    cout << "Duration of preparing submatrix: " << duration / 1000.0 << endl;


    
    step = subh_image.step;

    ofstream fout;
    fout.open ("submatrix.txt");
    int t = 0;
    
   // for(int b = 0; b < subframe.total(); b ++){
    
    

    
    for(int col = 0; col < roi._width; col ++)
    {
        for(int row = 0; row < roi._height; row++)
        {
            int conversion = (int) subhueArray[step * row + col];
            t+= conversion;
           // fout << conversion << "\n";
        }
    }
    
    cout << "SUBMAT Hue Total: " << t << endl;
    cout << "SUBMAT TOTAL " << subframe.total() << endl;
    fout.close();
    */
  
    /* End of Sub-Matrix Testing */
    
    
    
    
    
    
    
    
    
   // Point lastPoint = Point(roi._centroid.x, roi._centroid.y);
    

     t1 = high_resolution_clock::now();
        
        cvtColor(frame, hsv, CV_RGB2HSV);
    
    
        split(hsv, hsv_channels);
        h_image = hsv_channels[0];
        hueArray = h_image.data;
    
    
    
    
    
    t2 = high_resolution_clock::now();
    auto duration2 = duration_cast<microseconds>( t2 - t1 ).count();
   // cout << "Duration of preparing using entire frame: " << duration2 / 1000.0 << endl;
    
    
    
    
    
    
        step = h_image.step;
    
    //for(int col = 0; col < cap.get(CV_CAP_PROP_FRAME_WIDTH); col ++)
        
       // printf("%d vs %d \n", hueArray[col], hsv.at<Vec3b>(0,col)[0]);
    
        camShift.meanshift(hueArray, step, &roi, histogram);
     camShift.backProjectHistogram(hsv, &frame, roi, histogram);

      
        roi.drawROI(&frame);
      //  roi.printROI();
        writeFrame(frame);
        
 }//end while
    
  

    
	cout << endl << "****END OF CPU Serial MeanShift****" << endl;


    
    _outputVideo.release();
    free(histogram);
    
   return 0;
}
