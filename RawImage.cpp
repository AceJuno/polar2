#define _CRT_SECURE_NO_DEPRECATE
#include "RawImage.h"




RawImage::RawImage(String  name, int w, int h)
{
    this->width = w;
    this->height = h;
    name = "./foto/" + name;
    FILE* fp = fopen(name.c_str(), "rb");
    char* imgData = (char*)malloc(sizeof(char) * w * h);
    fread(imgData, sizeof(char), w * h, fp);
    img.create(h, w, CV_8UC1);

    memcpy(img.data, imgData, w * h);

    free(imgData);

    fclose(fp);
    imshow("test", img);
    waitKey(0);

    this->img = img;
}