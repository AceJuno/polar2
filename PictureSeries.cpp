#include "PictureSeries.h"


PictureSeries::PictureSeries(String name, int angle, int numOfPics)
{
    String ext = name.substr(name.find('.'));
    name = name.substr(0, name.find('_'));
    vector<Mat> imgS;

    for (int i = 0; i < numOfPics; i++)
    {
        int num = angle * i;
        Mat img = imread("./foto/" + name + '_' + to_string(num) + ext, IMREAD_GRAYSCALE);
        cout << img.size;
        //imshow("test",img);
        //waitKey(0);
        imgS.push_back(img);
    }
    this->imgSeries = imgS;
    this->angleStep = angle;
    this->numOfPics = numOfPics;
}