

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

#include "gpuMain.h"

using namespace cv;
using namespace std;

int main(int argc, const char * argv[])
{


    VideoCapture cap("test.mov");



	int x = 179;
	unsigned char y = (unsigned char) x;
int a = 5;
	y += a;
	printf("%d", y);
	




   
    Mat frame;
   cap.read(frame);



cout << endl << "**********************END OF PERRY TEST ***************" << endl;




	gpuMain(argc, argv);








	




   return 0;
}
