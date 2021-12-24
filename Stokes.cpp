#include "Stokes.h"

vector<Mat> stokesVector(vector<vector<vector<double>>> I)
{
	vector <Mat> stokes;

	int h = I[0].size();	//2048
	int w = I[0][0].size();	//2448

	cout << w << endl;
	cout << h << endl;

	vector<vector<double>> q(h, vector<double>(w));
	for (int i = 0; i < h; i++) {
		for (int k = 0; k < w; k++) {
			q[i][k] = I[0][i][k] - I[2][i][k];
		}
	}

	Mat S1 = Mat(h, w, DataType<double>::type);
	for (int i = 0; i < S1.rows; i++) {
		for (int k = 0; k < S1.cols; k++) {
			S1.at<double>(i, k) = q.at(i).at(k);
		}
	}


	vector<vector<double>> u(h, vector<double>(w));
	for (int i = 0; i < h; i++) {
		for (int k = 0; k < w; k++) {
			u[i][k] = I[1][i][k] - I[3][i][k];
		}
	}

	Mat S2 = Mat(h, w, DataType<double>::type);
	for (int i = 0; i < S2.rows; i++) {
		for (int k = 0; k < S2.cols; k++) {
			S2.at<double>(i, k) = u.at(i).at(k);
		}
	}


	vector<vector<double>> ii(h, vector<double>(w));
	for (int i = 0; i < h; i++) {
		for (int k = 0; k < w; k++) {
			ii[i][k] = I[0][i][k] + I[2][i][k];

		}
	}

	Mat S0 = Mat(h, w, DataType<double>::type);
	for (int i = 0; i < S0.rows; i++) {
		for (int k = 0; k < S0.cols; k++) {
			S0.at<double>(i, k) = ii.at(i).at(k);
		}
	}


	vector<vector<double>> dolp(h, vector<double>(w));
	for (int i = 0; i < h; i++) {
		for (int k = 0; k < w; k++) {
			dolp[i][k] = (sqrt(q[i][k] * q[i][k] + u[i][k] * u[i][k])) / ii[i][k];
		}
	}

	Mat out = Mat(h, w, DataType<double>::type);
	for (int i = 0; i < out.rows; i++) {
		for (int k = 0; k < out.cols; k++) {
			out.at<double>(i, k) = dolp.at(i).at(k);
		}
	}

	/*
	//normalize(out, out, 255, 0, NORM_MINMAX);
	Mat dolp_fin = Mat(h / 2, w / 2, DataType<double>::type);
	resize(out, dolp_fin, dolp_fin.size());
	imshow("dolp", dolp_fin);



	Mat sop = Mat(h, w, DataType<double>::type);
	for (int i = 0; i < sop.rows; i++) {
		for (int k = 0; k < sop.cols; k++) {
			out.at<double>(i, k) = ii.at(i).at(k);
		}
	}

	//normalize(sop, sop, 255, 0, NORM_MINMAX);
	
	Mat sop_fin = Mat(h / 2, w / 2, DataType<double>::type);
	resize(sop, sop_fin, sop_fin.size());

	imshow("sop", sop_fin);
	waitKey(0);
	*/

	normalize(S0, S0, 1.0, 0, NORM_MINMAX);
	normalize(S1, S1, 1.0, 0, NORM_MINMAX);
	normalize(S2, S2, 1.0, 0, NORM_MINMAX);

	Mat S0_fin = Mat(h / 2, w / 2, DataType<double>::type);
	Mat S1_fin = Mat(h / 2, w / 2, DataType<double>::type);
	Mat S2_fin = Mat(h / 2, w / 2, DataType<double>::type);
	resize(S0, S0_fin, S0_fin.size());
	resize(S1, S1_fin, S1_fin.size());
	resize(S2, S2_fin, S2_fin.size());
	imshow("S0", S0_fin);
	imshow("S1", S1_fin);
	imshow("S2", S2_fin);
	waitKey(0);

	stokes.push_back(S0);
	stokes.push_back(S1);
	stokes.push_back(S2);

	return stokes;
}
