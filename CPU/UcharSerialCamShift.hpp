//
//  SerialCamShift.hpp
//  ThesisSerialCamShift
//
//  Created by Matthew Perry on 1/28/16.
//  Copyright © 2016 Matthew Perry. All rights reserved.
//

#ifndef UcharSerialCamShift_hpp
#define UcharSerialCamShift_hpp

#include <stdio.h>
#include <iostream>
#include <fstream>
#include "RegionOfInterest.hpp"

#define BUCKET_WIDTH 3
#define BUCKETS 60

class SerialCamShift
{

public:
    void createHistogram(unsigned char * hsv, int step, RegionOfInterest * roi,  Mat * frame, float ** histogram);

    void meanshift(unsigned char * hueArray, int step, RegionOfInterest * roi, float * histogram);
    bool test(unsigned char * hueArray, int step, RegionOfInterest * roi, float * histogram);
    bool subMeanShiftTest(unsigned char * hueArray, int step, RegionOfInterest * roi, float * histogram, int * prevX, int * prevY);
    void printHistogram(float * histogram, int length);
    void backProjectHistogram(unsigned char * hsv, int step, Mat * frame, RegionOfInterest roi, float * histogram);
};

#endif /* SerialCamShift_hpp */
