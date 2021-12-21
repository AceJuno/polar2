#pragma once
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <vector>
#include "PictureSeries.h"

class RawImage
{
	Mat img;
	int width, height;
public:
	RawImage(String name, int w, int h);

};

