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
using namespace cv;
using namespace std;


PictureSeries wczytaj_serie();


int main()
{
    Mat img;
    String s = "./foto/polarcam/polm16.raw";
    int w = 2448;
    int h = 2048;
    //Demosaicing(s, img, w, h);
    //imshow("test", img);
    //waitKey(0);
	Mat out1 = Mat(h, w, DataType<double>::type);

	Mat RawImage;
	int rows = 2048;
	int cols = 2448;;
	RawImage.create(rows, cols, CV_8UC1);
	ifstream inFile;
	inFile.open(s, ios::binary);
	if (!inFile.is_open()) {
		cout << "Unable to open file" << endl;
		return -1;
	}
	
	// 16 bit -> short
	// Stop eating new lines in binary mode!!!
	inFile.unsetf(std::ios::skipws);
	// get its size:
	std::streampos fileSize;
	inFile.seekg(0, std::ios::end);
	fileSize = inFile.tellg();
	inFile.seekg(0, std::ios::beg);
	//read data
	inFile.read((char*)RawImage.data, cols * rows * sizeof(char)); //<-- 16 bit short
	inFile.close();
	//imshow("raw", RawImage);
	//waitKey(0);
	/*
	unsigned char max = 0.0;
	unsigned char min = 0.0;
	for (int i = 0; i < RawImage.rows; i++) {
		for (int k = 0; k < RawImage.cols; k++) {
			
			if (RawImage.at<unsigned char>(i, k) < min) min = RawImage.at<unsigned char>(i, k);
			if (RawImage.at<unsigned char>(i, k) > max) max = RawImage.at<unsigned char>(i, k);
		}
	}
	cout << endl << "MIN/MAX dla rawimage" << endl;
	cout << min << endl;
	cout << max << endl;
	*/


	vector<vector<vector<double>>> newton = NewtonPol(RawImage);

	vector<Mat> sVector = stokesVector(newton);

	double max = 0.0;
	double min = 0.0;
	/*
	for (int g = 0; g < 4; g++) {
		for (int i = 0; i < 2048; i++) {
			for (int k = 0; k < 2448; k++) {
				if (newton[g][i][k] < min) min = newton[g][i][k];
				if (newton[g][i][k] > max) max = newton[g][i][k];
			}
		}	
	}
	*/






	/*
		out = np.zeros(DOLPP.shape, np.double)
		DOLPP = bytescale(normalize(DOLPP, out, 1.0, 0.0,
			NORM_MINMAX, dtype = CV_32F))
		
		aop = (1 / 2) * np.arctan2(u, q)

		AOPP = np.double(aop)

		out = np.zeros(AOPP.shape, np.double)
		AOPP = bytescale(normalize(AOPP, out, 1.0, 0.0,
			NORM_MINMAX, dtype = CV_32F))

		#SOP = np.uint8(SOP)
		#DOLPP = np.uint8(DOLPP)
		#AOPP = np.uint8(AOPP)

		imshow('Intensity', SOP)
		waitKey(0);
		destroyAllWindows()

		imshow('DOP', DOLPP)
		waitKey(0);
		destroyAllWindows()

		imshow('AOP', AOPP)
		waitKey(0);
		
*/
    //wczytaj_serie();
    


    return 0;
}





void wypisz_menu()
{
    cout << "Polar - program do przetwarzania obrazów polaryzacyjncyh" << endl;
    cout << "Wybierz opcje poniżej i zatwierdź Enter" << endl;
    cout << "1 - Wczytaj obraz polaryzacyjny" << endl;
    cout << "2 - Wczytaj serię obrazów z kolejnymi kątami polaryzacji" << endl;
    cout << "3 - " << endl;
    cout << "4 - " << endl;
    cout << "5 - " << endl;

}

PictureSeries wczytaj_serie()
{
    cout << "Czy dodałeś obrazy do folderu foto? (Y/N)" << endl;
    cout << "Podaj liczbe obrazów: ";
    int l;
    cin >> l;
    cout << "Podaj wartość kąta: ";
    int angle;
    cin >> angle;

    cout << "Poprawna nazwa obrazu to {nazwa}_{kąt}.{rozszerzenie}" << endl;
    cout << "Podaj nazwe pierwszego obrazu: ";
    string name;
    cin >> name;

    PictureSeries pics(name, angle, l);
    return pics;
}

RawImage wczytaj_raw() 
{
    int width, height;
    string name;
    /*
    cout << "Podaj wysokość obrazu: ";
    cin >> height;
    cout << "Podaj szerokość obrazu: ";
    cin >> width;
    cout << "Poprawna nazwa obrazu to \"{nazwa}_{kąt}.{rozszerzenie}\": ";
    cin >> name;

    do testów */
    name = "polm8.raw";
    width = 2448;
    height = 2048;
    RawImage r(name, width, height);
    return r;
}