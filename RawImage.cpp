#define _CRT_SECURE_NO_DEPRECATE
#include "RawImage.h"




RawImage::RawImage(char strInputFile[], int w, int h)
{
    this->width = w;
    this->height = h;

    this->img = imageInput(strInputFile, w, h);
}

Mat RawImage::imageInput(char strInputFile[], int w, int h)
{
    Mat img;
    int width = w;
    int height = h;
    img.create(h, w, CV_8UC1);
    FILE* pInput = NULL;
    char* buf = new char[w * h];
    pInput = fopen(strInputFile, "rb");

    fread((char*)img.data, 1, w * h, pInput);

    fclose(pInput);

    return Mat();
}
