#include "Newton.h"

vector<vector<vector<double>>> NewtonPol(cv::Mat& src)
{
	int rows = src.rows;
	int cols = src.cols;

	vector<vector<vector<double>>> result;

	vector <vector <vector <double>>> R(rows, vector<vector<double>>(cols, vector<double>(4)));
	
	//vector <vector <double>> R1(rows, vector<double>(cols));
	//vector <vector <double>> R2(rows, vector<double>(cols));
	//vector <vector <double>> R3(rows, vector<double>(cols));

	//vector <vector <vector <double>>> R;
	//R.push_back(R0);
	//R.push_back(R1);
	//R.push_back(R2);
	//R.push_back(R3);

	src.convertTo(src, CV_64F);
	vector <vector <int>> O(rows,vector<int> (cols));
	for (int i = 0; i < rows; i++) {
		for (int k = 0; k < cols; k++) {
			if (i % 2 == 0 && k % 2 == 0) O[i][k] = 0;
			else if (i % 2 == 0 && k % 2 == 1) O[i][k] = 1;
			else if (i % 2 == 1 && k % 2 == 0) O[i][k] = 2;
			else  O[i][k] = 3;
		}
	}


	//intermediate results
	vector <vector <double>> I(rows, vector<double>(cols));
	

	for (int i = 0; i < rows; i++) {
		for (int k = 0; k < cols; k++) {
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

	//Stage one interpolation : interpolate vertically for case Fig.6(b),
	//interpolate horizontally for case Fig.6(c), interpolate in diagonal
	//directions for case Fig.6(a).The Eqs.(14) - (17) are simplified in this
	//code.

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


	double thao = 7;

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

	




	//Stage two interpolation : interpolate horizontally for case Fig.6(b),
	//interpolate vertically for case Fig.6(c).

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

	//Fusion of the estimations with edge classifier for case Fig.6(b) and Fig.6(c).

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
			I0[i][k] = R[i][k][0];
			I45[i][k] = R[i][k][1];
			I90[i][k] = R[i][k][2];
			I135[i][k] = R[i][k][3];
		}
	}

	result.push_back(I0);
	result.push_back(I45);
	result.push_back(I90);
	result.push_back(I135);

	
	int scols = cols/4 ;
	int srows = rows/4 ;
	Mat i0 = Mat(srows, scols, DataType<double>::type);
	Mat i45 = Mat(srows, scols, DataType<double>::type);
	Mat i90 = Mat(srows, scols, DataType<double>::type);
	Mat i135 = Mat(srows, scols, DataType<double>::type);
	
	for (int i = 0; i < rows - 1; i++) {
		for (int k = 0; k < cols - 1; k++) {
			if (i % 2 == 0 && k % 2 == 0) i0.at<double>(i / 4, k / 4) = src.at<double>(i, k);
			else if (i % 2 == 1 && k % 2 == 0) i45.at<double>(i / 4, k / 4) = src.at<double>(i, k);
			else if (i % 2 == 0 && k % 2 == 1) i90.at<double>(i / 4, k / 4) = src.at<double>(i, k);
			else if (i % 2 == 1 && k % 2 == 1)  i135.at<double>(i / 4, k / 4) = src.at<double>(i, k);
		}
	}
	/*
	for (int i = 0; i < rows; i++) {
		for (int k = 0; k < cols; k++) {
			i0.at<double>(i, k) = I0[i][k];
			i45.at<double>(i, k) = I45[i][k];
			i90.at<double>(i, k) = I90[i][k];
			i135.at<double>(i, k) = I135[i][k];
		}
	}
	*/
	normalize(i0, i0, 1.0, 0, NORM_MINMAX);
	normalize(i45, i45, 1.0, 0, NORM_MINMAX);
	normalize(i90, i90, 1.0, 0, NORM_MINMAX);
	normalize(i135, i135, 1.0, 0, NORM_MINMAX);

	Mat S0_fin = Mat(rows / 4, cols / 4, DataType<double>::type);
	Mat S1_fin = Mat(rows / 4, cols / 4, DataType<double>::type);
	Mat S2_fin = Mat(rows / 4, cols / 4, DataType<double>::type);
	Mat S3_fin = Mat(rows / 4, cols / 4, DataType<double>::type);

	resize(i0, S0_fin, S0_fin.size());
	resize(i45, S1_fin, S1_fin.size());
	resize(i90, S2_fin, S2_fin.size());
	resize(i135, S3_fin, S2_fin.size());
	//imshow("0", S0_fin);
	//imshow("45", S1_fin);
	//imshow("90", S2_fin);
	//imshow("135", S2_fin);
	//waitKey(0);

	return result;
}
