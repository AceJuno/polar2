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
			ii[i][k] = 0.5*(I[0][i][k] + I[1][i][k] + I[2][i][k] + I[3][i][k]);

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
	for (int i = 0; i < S0.rows; i++) {
		for (int k = 0; k < S0.cols; k++) {
			S0.at<double>(i, k) = S0.at<double>(i, k) * 255;
			S1.at<double>(i, k) = S1.at<double>(i, k) * 255;
			S2.at<double>(i, k) = S2.at<double>(i, k) * 255;
			if (S1.at<double>(i, k) < 0) S1.at<double>(i, k) += 0.001;

		}
	}
	imwrite("S0.jpg", S0);
	imwrite("S1.jpg", S1);
	imwrite("S2.jpg", S2);
	//waitKey(0);
	
	normalize(S0, S0, 1.0, 0, NORM_MINMAX);
	normalize(S1, S1, 1.0, -1.0, NORM_MINMAX);
	normalize(S2, S2, 1.0, -1.0, NORM_MINMAX);
	stokes.push_back(S0);
	stokes.push_back(S1);
	stokes.push_back(S2);

	return stokes;
}
//fmod(0.5 * atan2(S[2].at<double>(i, k), S[1].at<double>(i, k)), 3.14159265358979323846);
Mat aolp(vector<Mat> S)
{
	Mat result = Mat(S[0].rows, S[0].cols, DataType<double>::type);
	for (int i = 0; i < result.rows; i++) {
		for (int k = 0; k < result.cols; k++) {
			result.at<double>(i, k) = fmod(0.5 * atan2(S[2].at<double>(i, k), S[1].at<double>(i, k)), 3.14159265358979323846);
			//cout << result.at<double>(i, k) << " ";
		}
	}
	Mat show = Mat(S[0].rows, S[0].cols, DataType<double>::type);
	normalize(result, show, 1.0, 0, NORM_MINMAX);
	Mat S0_fin = Mat(S[0].rows / 2, S[0].cols / 2, DataType<double>::type);
	resize(show, S0_fin, S0_fin.size());
	for (int i = 0; i < result.rows; i++) {
		for (int k = 0; k < result.cols; k++) {
			show.at<double>(i, k) = 255 * show.at<double>(i, k);
		}
	}
	imwrite("AoLP.jpg", show);
	imshow("AoLP", S0_fin);
	//waitKey(0);

	return result;
}

Mat dolp(vector<Mat> S)
{
	Mat result = Mat(S[0].rows, S[0].cols, DataType<double>::type);
	for (int i = 0; i < result.rows; i++) {
		for (int k = 0; k < result.cols; k++) {
			result.at<double>(i, k) = sqrt(S[1].at<double>(i, k) * S[1].at<double>(i, k) + S[2].at<double>(i, k) * S[2].at<double>(i, k))/ S[0].at<double>(i, k);
		}
	}
	Mat show = Mat(S[0].rows, S[0].cols, DataType<double>::type);
	normalize(result, show, 1.0, 0, NORM_MINMAX);
	Mat S0_fin = Mat(S[0].rows / 2, S[0].cols / 2, DataType<double>::type);
	resize(result, S0_fin, S0_fin.size());
	for (int i = 0; i < result.rows; i++) {
		for (int k = 0; k < result.cols; k++) {
			result.at<double>(i, k) = 255 * result.at<double>(i, k);
		}
	}
	imwrite("DoLP.jpg", show);
	imshow("DoLP", S0_fin);
	//waitKey(0);

	return result;
}


Mat aolpColor(vector<Mat> S)
{
	Mat a = aolp(S);
	Mat d = dolp(S);
	Mat result = Mat(S[0].rows, S[0].cols, CV_64FC3);
	for (int i = 0; i < result.rows; i++) {
		for (int k = 0; k < result.cols; k++) {
			double x = (sqrt(a.at<double>(i, k) * a.at<double>(i, k))) * 179;
			//cout << x;
			result.at<Vec3d>(i, k)[0] = (sqrt(a.at<double>(i, k) * a.at<double>(i, k)) / 3.14159265358979323846) * 179;
			result.at<Vec3d>(i, k)[1] = double(255);
			result.at<Vec3d>(i, k)[2] = d.at<double>(i, k) *255;
		}
	}
	Mat imHSV = Mat(S[0].rows, S[0].cols, CV_32FC3);
	//normalize(result, result, 1.0, 0, NORM_MINMAX);
	result.convertTo(result, CV_32FC3);
	cvtColor(result, imHSV, 54);

	Mat S0_fin = Mat(S[0].rows / 2, S[0].cols / 2, DataType<double>::type);
	resize(imHSV, S0_fin, S0_fin.size());
	imwrite("AoLP - color.jpg", result);
	imshow("AoLP - color", S0_fin);
	//waitKey(0);

	return result;
}


Mat dolpColor(vector<Mat> S)
{
	Mat a = dolp(S);
	Mat result = Mat(S[0].rows, S[0].cols, CV_64FC3);
	for (int i = 0; i < result.rows; i++) {
		for (int k = 0; k < result.cols; k++) {
			result.at<Vec3d>(i, k)[0] = a.at<double>(i, k)* 179;
			result.at<Vec3d>(i, k)[1] = double(255);
			result.at<Vec3d>(i, k)[2] = double(255);
		}
	}
	Mat imHSV = Mat(S[0].rows, S[0].cols, CV_32FC3);
	//normalize(result, result, 1.0, 0, NORM_MINMAX);
	result.convertTo(result, CV_32FC3);
	cvtColor(result, imHSV, 54);

	Mat S0_fin = Mat(S[0].rows / 2, S[0].cols / 2, DataType<double>::type);
	resize(imHSV, S0_fin, S0_fin.size());
	imwrite("DoLP - color.jpg", result);
	imshow("DoLP - color", S0_fin);
	waitKey(0);

	return result;
}