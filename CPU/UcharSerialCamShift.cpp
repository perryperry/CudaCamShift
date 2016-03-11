//
//  SerialCamShift.cpp
//  ThesisSerialCamShift
//
//  Created by Matthew Perry on 1/28/16.
//  Copyright © 2016 Matthew Perry. All rights reserved.
//

#include "UcharSerialCamShift.hpp"

void SerialCamShift::createHistogram(uchar * hsv, int step, RegionOfInterest * roi,  Mat * frame, double ** histogram)
{
    
    Point topLeft = (*roi).getTopLeft();
    Point bottomRight = (*roi).getBottomRight();
    int hue = 0;
    
    double total_pixels = (double) (*roi).getTotalPixels();
    
    for(int row = topLeft.y; row < bottomRight.y; row++)
    {
        for(int col = topLeft.x; col < bottomRight.x; col ++)
        {
            hue = hsv[ step * row + col ];
          //  cout << "UCHAR: " << hue << endl;
            if(hue > 10 )//trying thresholding
                (*histogram)[hue / BUCKET_WIDTH] ++;
        }
        
    }
    
    for(int i =0; i < BUCKETS; i++)//convert to probability
    {
        
        (*histogram)[i] /= total_pixels;
        (*histogram)[i] *= 255;
        
        // cout << "i = " << i << " has " << (*histogram)[i] << endl;
    }
    
}


void SerialCamShift::meanshift(uchar * hueArray, int step, RegionOfInterest * roi, double * histogram)
{

    Point topLeft = (*roi).getTopLeft();
    Point bottomRight = (*roi).getBottomRight();
    long M00 = 0, M1x = 0, M1y = 0;
    int xc = 0;
    int yc = 0;
    int hue = 0;
    long probability = 0;
    bool converging = true;
    
    int prevX = xc;
    int prevY = yc;
    
    while(converging)
    {
        
        for(int col = topLeft.x; col < bottomRight.x; col ++)
        {
            for(int row = topLeft.y; row < bottomRight.y; row++)
            {
                
                hue = hueArray[ step * row + col ];
                probability = (int) histogram[hue / BUCKET_WIDTH];
                M00 += probability;
                M1x += col * probability;
                M1y += row * probability;
                
                
                
            }
          //  printf("NEW--> %d %d %d %d\n", hue, (int)M00, (int)M1x,(int) M1y);

            
        }
        
        if(M00 > 0){//Can't divide by zero...
            
            xc = (int)(M1x / M00);
            yc = (int)(M1y / M00);
        }
        
        if(prevX - xc < 1 && prevY - yc < 1)
            converging = false;
        else
        {
            prevX = xc;
            prevY = yc;
        }
        
    }//end of converging
    
    (*roi).setCentroid(Point(xc, yc));

}