#pragma once
#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>

using namespace std;
using namespace cv;

class RawImage
{
public:
	int width, height;
	Mat img;

	RawImage(char strInputFile[], int w, int h);

	static Mat imageInput(char strInputFile[], int w, int h);

};

