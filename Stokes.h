#pragma once
#include <Vector>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <math.h>


using namespace std;
using namespace cv;



	vector<Mat> stokesVector(vector<vector<vector<double>>> I);
	Mat aolp(vector<Mat> S);
	Mat dolp(vector<Mat> S);
	Mat aolpColor(vector<Mat> S);
	Mat dolpColor(vector<Mat> S);



