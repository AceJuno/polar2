#pragma once
#include <vector>
#include <opencv2/core/utility.hpp>
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <vector>

using namespace std;
using namespace cv;

class PictureSeries
{
	vector<Mat> imgSeries;
	int angleStep;
	int numOfPics;
public:
	PictureSeries(String n, int a, int num);

};

