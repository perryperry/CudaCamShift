

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
#include "CPU/cpuMain.hpp"

using namespace cv;
using namespace std;

int main(int argc, const char * argv[])
{
	cpuMain(argc, argv);
	cout << endl << "****END OF CPU Serial MeanShift****" << endl;
//	gpuMain(argc, argv);
   return 0;
}
