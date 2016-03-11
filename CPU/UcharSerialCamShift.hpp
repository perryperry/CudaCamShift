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

#include "RegionOfInterest.hpp"

#define BUCKET_WIDTH 3
#define BUCKETS 60

class SerialCamShift
{

public:
    
    void createHistogram(uchar * hsv, int step, RegionOfInterest * roi,  Mat * frame, double ** histogram);
    void meanshift(uchar * hueArray, int step, RegionOfInterest * roi, double * histogram);

};

#endif /* SerialCamShift_hpp */
