#include "Newton.h"


Newton::Newton(cv::Mat& src)
{
	this->height = src.rows;
	this->width = src.cols;
	this->demoImages = NewtonPol(src);
}






vector<vector<vector<double>>> Newton::NewtonPol(cv::Mat& src)
{
	src.convertTo(src, CV_64F);

	int rows = src.rows;
	int cols = src.cols;
	vector<vector<vector<double>>> result;
	vector <vector <vector <double>>> R(rows, 
	vector<vector<double>>(cols, vector<double>(4)));
	vector <vector <double>> I(rows, vector<double>(cols));

	vector <vector <int>> O(rows,vector<int> (cols));
	for (int i = 0; i < rows; i++) {
		for (int k = 0; k < cols; k++) {
			if (i % 2 == 0 && k % 2 == 0) O[i][k] = 0;
			else if (i % 2 == 0 && k % 2 == 1) O[i][k] = 1;
			else if (i % 2 == 1 && k % 2 == 0) O[i][k] = 2;
			else  O[i][k] = 3;

			R[i][k][0] = src.at<double>(i, k);
			R[i][k][1] = src.at<double>(i, k);
			R[i][k][2] = src.at<double>(i, k);
			R[i][k][3] = src.at<double>(i, k);
			I[i][k] = src.at<double>(i, k);
		}
	}

	vector <vector <double>> Y1 = I;
	vector <vector <double>> Y2 = I;
	vector <vector <double>> XX1 = I;
	vector <vector <double>> XX2 = I;
	vector <vector <double>> YY1 = I;
	vector <vector <double>> YY2 = I;
	vector <vector <vector <double>>> RR = R;

	for (int i = 3; i < rows - 4; i++) {
		for (int j = 3; j < cols - 4; j++) {
			R[i][j][O[i][j]] = I[i][j];
			R[i][j][O[i][j + 1]] = 0.5 * I[i][j] + 0.0625 * I[i][j - 3] - 0.25 * I[i][j - 2]
				+ 0.4375 * I[i][j - 1] + 0.4375 * I[i][j + 1]
				- 0.25 * I[i][j + 2] + 0.0625 * I[i][j + 3];
			R[i][j][O[i + 1][j]] = 0.5 * I[i][j] + 0.0625 * I[i - 3][j] - 0.25 * I[i - 2][j]
				+ 0.4375 * I[i - 1][j] + 0.4375 * I[i + 1][j]
				- 0.25 * I[i + 2][j] + 0.0625 * I[i + 3][j];
			Y1[i][j] = 0.5 * I[i][j] + 0.0625 * I[i - 3][j - 3] - 0.25 * I[i - 2][j - 2] + 0.4375
				* I[i - 1][j - 1] + 0.4375 * I[i + 1][j + 1]
				- 0.25 * I[i + 2][j + 2] + 0.0625 * I[i + 3][j + 3];
			Y2[i][j] = 0.5 * I[i][j] + 0.0625 * I[i - 3][j + 3] - 0.25 * I[i - 2][j + 2] + 0.4375
				* I[i - 1][j + 1] + 0.4375 * I[i + 1][j - 1]
				- 0.25 * I[i + 2][j - 2] + 0.0625 * I[i + 3][j - 3];
		}
	}


	double thao = 5.8;

	for (int i = 3; i < rows - 4; i++) {
		for (int j = 3; j < cols - 4; j++) {
			double pha1 = 0.0;
			double pha2 = 0.0;

			for (int k = -2; k < 3; k += 2) {
				for (int l = -2; l < 3; l += 2) {
					pha1 = pha1 + abs(Y1[i + k][j + l] - I[i + k][j + l]);
					pha2 = pha2 + abs(Y2[i + k][j + l] - I[i + k][j + l]);
				}
			}

			if ((pha1 / pha2) > thao) R[i][j][O[i + 1][j + 1]] = Y2[i][j];
			else if ((pha2 / pha1) > thao) R[i][j][O[i + 1][j + 1]] = Y1[i][j];
			else if (((pha1 / pha2) < thao) && ((pha2 / pha1) < thao)) {
				double d1 = abs(I[i - 1][j - 1] - I[i + 1][j + 1])
					+ abs(2 * I[i][j] - I[i - 2][j - 2] - I[i + 2][j + 2]);
				double d2 = abs(I[i + 1][j - 1] - I[i - 1][j + 1])
					+ abs(2 * I[i][j] - I[i + 2][j - 2] - I[i - 2][j + 2]);
				double epsl = 0.000000000000001;
				double w1 = 1 / (d1 + epsl);
				double w2 = 1 / (d2 + epsl);
				R[i][j][O[i + 1][j + 1]] = (w1 * Y1[i][j] + w2 * Y2[i][j]) / (w1 + w2);
			}
		}
	}

	for (int i = 3; i < rows - 4; i++) {
		for (int j = 3; j < cols - 4; j++) {
			XX1[i][j] = R[i][j][O[i][j + 1]];
			XX2[i][j] = 0.5 * I[i][j] + 0.0625
				* R[i - 3][j][O[i][j + 1]] - 0.25 * I[i - 2][j];
			XX2[i][j] = XX2[i][j] + 0.4375
				* R[i - 1][j][O[i][j + 1]] + 0.4375 * R[i + 1][j][O[i][j + 1]];
			XX2[i][j] = XX2[i][j] - 0.25 * I[i + 2][j] + 0.0625 * R[i + 3][j][O[i][j + 1]];
			YY1[i][j] = R[i][j][O[i + 1][j]];
			YY2[i][j] = 0.5 * I[i][j] + 0.0625
				* R[i][j - 3][O[i + 1][j]] - 0.25 * I[i][j - 2];
			YY2[i][j] = YY2[i][j] + 0.4375
				* R[i][j - 1][O[i + 1][j]] + 0.4375 * R[i][j + 1][O[i + 1][j]];
			YY2[i][j] = YY2[i][j] - 0.25 * I[i][j + 2] + 0.0625 * R[i][j + 3][O[i + 1][j]];
		}
	}

	for (int i = 3; i < rows - 4; i++) {
		for (int j = 3; j < cols - 4; j++) {
			double pha1 = 0.0;
			double pha2 = 0.0;

			for (int k = -2; k < 3; k += 2) {
				for (int l = -2; l < 3; l += 2) {
					pha1 = pha1 + abs(YY1[i + k][j + l] - I[i + k][j + l]);
					pha2 = pha2 + abs(YY2[i + k][j + l] - I[i + k][j + l]);
				}
			}

			if ((pha1 / pha2) > thao) R[i][j][O[i][j + 1]] = XX2[i][j];
			else if ((pha2 / pha1) > thao) R[i][j][O[i][j + 1]] = XX1[i][j];
			else if (((pha1 / pha2) < thao) && ((pha2 / pha1) < thao)) {
				double d1 = abs(I[i][j - 1] - I[i][j + 1])
					+ abs(2 * I[i][j] - I[i][j - 2] - I[i][j + 2]);
				double d2 = abs(I[i + 1][j] - I[i - 1][j])
					+ abs(2 * I[i][j] - I[i + 2][j] - I[i - 2][j]);
				double epsl = 0.000000000000001;
				double w1 = 1 / (d1 + epsl);
				double w2 = 1 / (d2 + epsl);
				R[i][j][O[i][j + 1]] = (w1 * XX1[i][j] + w2 * XX2[i][j]) / (w1 + w2);
			}
		}
	}

	vector<vector<double>> I0(rows, vector<double>(cols));
	vector<vector<double>> I45(rows, vector<double>(cols));
	vector<vector<double>> I90(rows, vector<double>(cols));
	vector<vector<double>> I135(rows, vector<double>(cols));

	for (int i = 0; i < rows; i++) {
		for (int k = 0; k < cols; k++) {
			I0[i][k] = R[i][k][3];
			I45[i][k] = R[i][k][1];
			I90[i][k] = R[i][k][0];
			I135[i][k] = R[i][k][2];
		}
	}

	result.push_back(I0);
	result.push_back(I45);
	result.push_back(I90);
	result.push_back(I135);

	return result;
}


