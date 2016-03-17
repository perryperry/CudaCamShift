//
//  SerialCamShift.cpp
//  ThesisSerialCamShift
//
//  Created by Matthew Perry on 1/28/16.
//  Copyright © 2016 Matthew Perry. All rights reserved.
//

#include "UcharSerialCamShift.hpp"

void SerialCamShift::backProjectHistogram(uchar * hsv, int step, Mat * frame, RegionOfInterest roi, float * histogram)
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

void SerialCamShift::createHistogram(uchar * hsv, int step, RegionOfInterest * roi,  Mat * frame, float ** histogram)
{
    
    Point topLeft = (*roi).getTopLeft();
    Point bottomRight = (*roi).getBottomRight();
    int hue = 0;
    
    ofstream fout;
    fout.open ("hist.txt");


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

void SerialCamShift::meanshift(uchar * hueArray, int step, RegionOfInterest * roi, float * histogram)
{

    Point topLeft = (*roi).getTopLeft();
    Point bottomRight = (*roi).getBottomRight();
    float M00 = 0.0, M1x = 0.0, M1y = 0.0;
    unsigned long xc = 0;
    unsigned long yc = 0;
    int hue = 0;
    float probability = 0.0;
    bool converging = true;
    
    unsigned long prevX = xc;
    unsigned long prevY = yc;
    int count = 0;
    
    ofstream myfile;
    myfile.open ("meanshift.txt");
    
   // ofstream myfile2;
    //myfile2.open ("meanshift2.txt");
    
    // printf("HI: %d %d\n", topLeft.x, topLeft.y);
    
   
    while(converging)
    {
        count ++;
        int total  = 0;
        
      cout << "Let's have a look... " << (*roi).getTopLeftX() << " and " << (*roi).getBottomRightX()  << " and "<< (*roi).getTopLeftY()<< " and " << (*roi).getBottomRightY() <<endl;
        
        
        for(int col = (*roi).getTopLeftX(); col < (*roi).getBottomRightX(); col ++)
       {
            for(int row = (*roi).getTopLeftY(); row < (*roi).getBottomRightY(); row++)
           {
      
                hue = hueArray[ step * row + col ];
                total += hue;
                
               myfile << hue << "\n";
                probability =  histogram[hue / BUCKET_WIDTH];
                M00 += probability;
                M1x += ((float)col) * probability;
                M1y += ((float)row) * probability;
            }
            
        }
       // cout << "MEANSHIFT hue total: " << total<< endl;
        
      myfile.close();
        
        
    printf("OLD--> %lf %lf %lf\n",M00, M1x,M1y);
        
        if(M00 > 0){//Can't divide by zero...
            
            xc = (unsigned long) (M1x / M00);
            yc = (unsigned long) (M1y / M00);
            (*roi).setCentroid(Point((int)xc, (int)yc));
            
        }
      //  printf("OLD --> %lu %lu\n", xc, yc);
   /*     M00 = 0;
        M1x = 0;
        M1y = 0;
        total = 0;
        
        
        
        for(int col = 0; col < (*roi)._width; col ++)
        {
            for(int row = 0; row < (*roi)._height; row++)
            {
                
                int  r = row + topLeft.y;
                int c = col + topLeft.x;
                
                hue = hueArray[ step * r + c];
                total += hue;
    
                myfile2 << hue << "\n";
                
                probability = (unsigned long) histogram[hue / BUCKET_WIDTH];
                M00 += probability;
                M1x += c * probability;
                M1y += r * probability;
            }
            
        }
         // printf("NEW--> %lu %lu %lu\n",M00,M1x, M1y);
cout << "MEANSHIFT hue total: " << total<< endl;
        
        myfile2.close();*/
        
    /*    if(M00 > 0){//Can't divide by zero...
            
            xc = (unsigned long) (M1x / M00);
            yc = (unsigned long) (M1y / M00);
        }*/
        
       printf("xc vs prevX: %lu %lu, yc vs prevY: %lu %lu\n", xc, prevX, yc, prevY);
        
        
        if(xc - prevX < 1 && yc - prevY < 1)
            converging = false;
        else
        {
            prevX = xc;
            prevY = yc;
        }
        
     //   printf("#%d M00--> %lu M1x--> %lu M1y --> %lu XC --> %lu YC ---> %lu\n",count, M00, M1x, M1y, xc, yc);
    }//end of converging
  
  printf("************ CONVERGED: %lu, %lu\n", xc, yc);
    (*roi).setCentroid(Point((int)xc, (int)yc));

}