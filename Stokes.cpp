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

	vector<vector<double>> u(h, vector<double>(w));
	for (int i = 0; i < h; i++) {
		for (int k = 0; k < w; k++) {
			u[i][k] = I[1][i][k] - I[3][i][k];
		}
	}


	vector<vector<double>> ii(h, vector<double>(w));
	for (int i = 0; i < h; i++) {
		for (int k = 0; k < w; k++) {
			ii[i][k] = 0.5 * (I[0][i][k] + I[1][i][k] + I[2][i][k] + I[3][i][k]);

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


	return stokes;
}
