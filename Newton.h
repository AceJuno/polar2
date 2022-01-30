#pragma once
#include <Vector>
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

class Newton
{
public:
	int width, height;
	vector<vector<vector<double>>> demoImages;

	Newton(cv::Mat& src);

	static vector<vector<vector<double>>> NewtonPol(cv::Mat& src);

};