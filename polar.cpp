// polar.cpp : Ten plik zawiera funkcję „main”. W nim rozpoczyna się i kończy wykonywanie programu.
//
#define _CRT_SECURE_NO_DEPRECATE
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <vector>
#include <fstream>
#include <math.h>
#include "PictureSeries.h"
#include "RawImage.h"
#include "Demosaicing.hpp"
#include "Newton.h"
#include "Stokes.h"
#include "Mueller.h"

using namespace cv;
using namespace std;


PictureSeries wczytaj_serie();


int main()
{
	Mat raw;
	int rows = 2048;
	int cols = 2448;;
	raw.create(rows, cols, CV_8UC1);

    char strInputFile[] = "./foto/polarcam/polm16.raw";
    RawImage rrr = RawImage(strInputFile, cols, rows);

    //raw = RawImage::imageInput(strInputFile, cols, rows);
 
	//imshow("raw", RawImage);
	//waitKey(0);
    //vector<vector<vector<double>>> newton = NewtonPol(RawImage);
    Mat dragon = imread("./foto/polarcam/dragon.png", 0);
    //Mat camera = imread("./foto/polarcam/fruits.png", 0);
	vector<vector<vector<double>>> newtondragon = Newton::NewtonPol(dragon);
    //vector<vector<vector<double>>> newtoncamera = Newton::NewtonPol(camera);
    
	vector<Mat> sdVector = Stokes::stokesVector(newtondragon);
    //vector<Mat> scVector = Stokes::stokesVector(newtoncamera);
   
    //Stokes::aolp(sdVector);
    Stokes::aolpColor(sdVector);
    //Stokes::aolpColor(scVector);
    waitKey(0);
    //dolpColor(sVector);
    /*
    
    vector<Mat> images;
    for (int i = 0; i < 4; i++){
        for (int j = 0; j < 4; j++) {
            //Mat image;
            int numl = 45 * i;
            int numc = 45 * j;
            Mat image = imread("./mueller/various_l" + to_string(numl) +"_c" + to_string(numc) + ".exr", -1);
            //cout << "./mueller/various_l" + to_string(numl) + "_c" + to_string(numc) + ".exr";
            images.push_back(image);
            //imshow("l" + to_string(numl) + "_c" + to_string(numc), image);
        }
    }
    vector<vector<Mat>> mueller = muellerMatrix(images, 4, 45);
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            imshow("m" + to_string(i) + to_string(j), mueller[i][j]);

            imwrite("m" + to_string(i) + to_string(j) + ".jpg", mueller[i][j]);

        }
    }
    */

    waitKey(0);

    return 0;
}





