#pragma once
#include <Vector>
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

vector<vector<vector<double>>> NewtonPol(cv::Mat& src);
