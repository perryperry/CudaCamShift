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
    void createHistogram(uchar * hsv, int step, RegionOfInterest * roi,  Mat * frame, float ** histogram);

    void meanshift(uchar * hueArray, int step, RegionOfInterest * roi, float * histogram);
    bool test(uchar * hueArray, int step, RegionOfInterest * roi, float * histogram);
    bool subMeanShiftTest(uchar * hueArray, int step, RegionOfInterest * roi, float * histogram, int * prevX, int * prevY);
    void printHistogram(float * histogram, int length);
    void backProjectHistogram(uchar * hsv, int step, Mat * frame, RegionOfInterest roi, float * histogram);
};

#endif /* SerialCamShift_hpp */
