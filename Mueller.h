#pragma once
#include <Vector>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <math.h>


using namespace std;
using namespace cv;



class Mueller
{
public:
	vector<vector<Mat>> matrix;

	Mueller(vector<Mat> Images, int N, int angle);

	static vector<vector<Mat>> muellerMatrix(vector<Mat> Images, int N, int angle);
};
