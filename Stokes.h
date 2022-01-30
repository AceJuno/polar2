#pragma once
#include <Vector>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <math.h>


using namespace std;
using namespace cv;


class Stokes
{
public:
	Mat aolpImg;
	Mat aolpColorImg;
	Mat dolpImg;
	vector<Mat> stokesParameters;

	Stokes(vector<vector<vector<double>>> I);
	static vector<Mat> stokesVector(vector<vector<vector<double>>> I);
	static Mat aolp(vector<Mat> S);
	static Mat dolp(vector<Mat> S);
	static Mat aolpColor(vector<Mat> S);
};


