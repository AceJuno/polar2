#include "Mueller.h"

vector<Mat> muellerMatrix(vector<Mat> Stokes)
{
    int w = Stokes[0].rows;
    int h = Stokes[0].cols;
    Mat mueller = Mat(h, w, DataType<double>::type);









    return mueller;
}
