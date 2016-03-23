//
//  SerialCamShift.cpp
//  ThesisSerialCamShift
//
//  Created by Matthew Perry on 1/28/16.
//  Copyright © 2016 Matthew Perry. All rights reserved.
//

#include "UcharSerialCamShift.hpp"

void SerialCamShift::backProjectHistogram(unsigned char * hsv, int step, Mat * frame, RegionOfInterest roi, float * histogram)
{
    int hue = 0, count = 0;
    for(int col = 0; col < roi._width; col ++)
    {
        for(int row = 0; row < roi._height; row++)
        {
            hue = hsv[step * row + col];
            (*frame).at<Vec3b>( row + roi.getTopLeftY(), col + roi.getTopLeftX() )[0] = (int) (255 * histogram[hue / BUCKET_WIDTH]);
            (*frame).at<Vec3b>( row + roi.getTopLeftY(), col + roi.getTopLeftX() )[1] = (int) (255 * histogram[hue / BUCKET_WIDTH]);
            (*frame).at<Vec3b>( row + roi.getTopLeftY(), col + roi.getTopLeftX() )[2] = (int) (255 * histogram[hue / BUCKET_WIDTH]);
        }
    }
}

void SerialCamShift::printHistogram(float * histogram, int length)
{
    printf("********** PRINTING HISTOGRAM **********\n");
    int i = 0;
    for(i =0; i < length; i++)
    {
        printf("%d) %f, ", i, histogram[i]);
        if(i % 10 == 0)
            printf("\n");
    }
     printf("\n********** FINISHED PRINTING HISTOGRAM **********\n");
}

void SerialCamShift::createHistogram(unsigned char * hsv, int step, RegionOfInterest * roi,  Mat * frame, float ** histogram)
{
    
    Point topLeft = (*roi).getTopLeft();
    Point bottomRight = (*roi).getBottomRight();
    int hue = 0;
    
    ofstream fout;
    fout.open ("hist2.txt");


   float total_pixels = (float) (*roi).getTotalPixels();
    
    
    for(int col = 0; col < (*roi)._width; col ++)
    {
        for(int row = 0; row < (*roi)._height; row++)
        {
            hue = hsv[ step * row + col ];
            (*histogram)[hue / BUCKET_WIDTH] ++;
        }
    }
    for(int i =0; i < BUCKETS; i++)//convert to probability
    {
        (*histogram)[i] /= total_pixels;
        
        fout << (*histogram)[i] << endl;
    }
    fout.close();
}

bool SerialCamShift::subMeanShiftTest(unsigned char * hueArray, int step, RegionOfInterest * roi, float * histogram, int * prevX, int * prevY)
{
    float M00 = 0.0, M1x = 0.0, M1y = 0.0;
    int xc = 0;
    int yc = 0;
    int hue = 0;
    float probability = 0.0;
    int rOffset = (*roi).getTopLeftY();
    int cOffset = (*roi).getTopLeftX();
    
    int count = 0;
    
    ofstream myfile;
    myfile.open ("meanshift3.txt", fstream::app);
 
    count ++;
    int total  = 0;
    
    for(int c = 0; c < (*roi)._width; c ++)
    {
        for(int r = 0; r < (*roi)._height; r++)
        {
            hue = hueArray[ step * r + c ];
            total += hue;
            
            probability = histogram[hue / BUCKET_WIDTH];
            M00 += probability;
            M1x += ((float)(c + cOffset)) * probability;
            M1y += ((float)(r + rOffset)) * probability;
        }
        
    }
     myfile << M00 << " " << M1x << " " << M1y << "\n";
    
    
    if(M00 > 0){//Can't divide by zero...
        
        xc = (int) (M1x / M00);
        yc = (int) (M1y / M00);
        (*roi).setCentroid(Point(xc, yc));
       // myfile << xc << " " << yc << "\n";
    }
    
    
    
    
    if(*prevX - xc < 1 && *prevX - xc > -1  && *prevY - yc < 1 && *prevY - yc > -1)
    {
        (*roi).setCentroid(Point(xc, yc));
        myfile << "****\n";
        myfile.close();
        return false;
    }
    else
    {
        (*roi).setCentroid(Point(xc, yc));
        *prevX = xc;
        *prevY = yc;
        myfile.close();
        return true;
    }
}


bool SerialCamShift::test(unsigned char * hueArray, int step, RegionOfInterest * roi, float * histogram)
{
    float M00 = 0.0, M1x = 0.0, M1y = 0.0;
    int xc = 0;
    int yc = 0;
    int hue = 0;
    float probability = 0.0;
    bool converging = true;
    
    int prevX = (*roi).getCenterX();
    int prevY = (*roi).getCenterY();
    int count = 0;
    
    ofstream myfile2;
    myfile2.open ("meanshift2.txt", fstream::app);
    
    while(converging)
    {
        count ++;
        int total  = 0;
        M00 = 0.0, M1x = 0.0, M1y = 0.0;
      
         for(int col = 0; col < (*roi)._width; col ++)
         {
             for(int row = 0; row < (*roi)._height; row++)
             {
                 int  r = row + (*roi).getTopLeftY();
                 int c = col + (*roi).getTopLeftX();
                 
                 hue = hueArray[ step * r + c ];
                 total += hue;
                 
                 probability = histogram[hue / BUCKET_WIDTH];
                 M00 += probability;
                 M1x += ((float)c) * probability;
                 M1y += ((float)r) * probability;
                 
             }
         
         }
        myfile2 << M00 << " " << M1x << " " << M1y << "\n";
        if(M00 > 0){//Can't divide by zero...
         
            xc = (int) (M1x / M00);
            yc = (int) (M1y / M00);
            (*roi).setCentroid(Point(xc, yc));
            // myfile2 << xc << " " << yc << "\n";
         }
        
        if(prevX - xc < 1 && prevX - xc > -1  && prevY - yc < 1 && prevY - yc > -1){
            myfile2 << "****\n";
            converging = false;
        }
        else
        {
            prevX = xc;
            prevY = yc;
            (*roi).setCentroid(Point(xc, yc));
        }
        
    }//end of converging

    myfile2.close();
    (*roi).setCentroid(Point(xc, yc));
    
    return true;
}


void SerialCamShift::meanshift(uchar * hueArray, int step, RegionOfInterest * roi, float * histogram)
{
    float M00 = 0.0, M1x = 0.0, M1y = 0.0;
    int xc = (*roi).getCenterX();
    int yc = (*roi).getCenterY();
    int hue = 0;
    float probability = 0.0;
    bool converging = true;
    
    int prevX = xc;
    int prevY = yc;
    int count = 0;
    
    ofstream myfile;
    myfile.open ("meanshift.txt", fstream::app);
   
    while(converging)
    {
        count ++;
        int total  = 0;
         M00 = 0.0, M1x = 0.0, M1y = 0.0;
        
        for(int col = (*roi).getTopLeftX(); col < (*roi).getBottomRightX(); col ++)
       {
            for(int row = (*roi).getTopLeftY(); row < (*roi).getBottomRightY(); row++)
           {
      
                hue = hueArray[ step * row + col ];
                total += hue;
               
                probability =  histogram[hue / BUCKET_WIDTH];
                M00 += probability;
                M1x += ((float)col) * probability;
                M1y += ((float)row) * probability;
        
            }
            
        }
           myfile << M00 << " " << M1x << " " << M1y << "\n";
        if(M00 > 0){//Can't divide by zero...
            
            xc = (int) (M1x / M00);
            yc = (int) (M1y / M00);
            (*roi).setCentroid(Point(xc, yc));
            
          //  myfile << xc << " " << yc << "\n";
        }
        
        if(prevX - xc < 1 && prevX - xc > -1  && prevY - yc < 1 && prevY - yc > -1){
            myfile << "****\n";
            converging = false;
        }
        else
        {
            prevX = xc;
            prevY = yc;
            (*roi).setCentroid(Point(xc, yc));
        }
    
    }//end of converging
    
    myfile.close();
    
    (*roi).setCentroid(Point(xc, yc));

}