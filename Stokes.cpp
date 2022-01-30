#include "Stokes.h"

Stokes::Stokes(vector<vector<vector<double>>> I)
{
	this->stokesParameters = stokesVector(I);
	this->aolpImg = aolp(stokesParameters);
	this->dolpImg = dolp(stokesParameters);
	this->aolpColorImg = aolpColor(stokesParameters);

}



vector<Mat> Stokes::stokesVector(vector<vector<vector<double>>> I)
{
	auto t_start = std::chrono::high_resolution_clock::now();
	vector <Mat> stokes;

	int h = I[0].size();
	int w = I[0][0].size();


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
	auto t_end = std::chrono::high_resolution_clock::now();
	cout << "Stokes: " << std::chrono::duration<double, std::milli>(t_end - t_start).count() << " ms" << endl;

	Mat S0_fin = Mat(h / 2, w / 2, DataType<double>::type);
	Mat S1_fin = Mat(h / 2, w / 2, DataType<double>::type);
	Mat S2_fin = Mat(h / 2, w / 2, DataType<double>::type);
	resize(S0, S0_fin, S0_fin.size());
	resize(S1, S1_fin, S1_fin.size());
	resize(S2, S2_fin, S2_fin.size());
	normalize(S0_fin, S0_fin, 1.0, 0, NORM_MINMAX);
	normalize(S1_fin, S1_fin, 1.0, 0, NORM_MINMAX);
	normalize(S2_fin, S2_fin, 1.0, 0, NORM_MINMAX);

	//imshow("S0", S0_fin);
	//imshow("S1", S1_fin);
	//imshow("S2", S2_fin);
	for (int i = 0; i < S0.rows; i++) {
		for (int k = 0; k < S0.cols; k++) {
			//S0.at<double>(i, k) = S0.at<double>(i, k) * 255;
			//S1.at<double>(i, k) = S1.at<double>(i, k) * 255;
			//S2.at<double>(i, k) = S2.at<double>(i, k) * 255;
			//if (S1.at<double>(i, k) < 0) S1.at<double>(i, k) += 0.001;

		}
	}
	normalize(S0_fin, S0_fin, 255.0, 0, NORM_MINMAX);
	normalize(S1_fin, S1_fin, 255.0, 0, NORM_MINMAX);
	normalize(S2_fin, S2_fin, 255.0, 0, NORM_MINMAX);
	imwrite("S0.jpg", S0_fin);
	imwrite("S1.jpg", S1_fin);
	imwrite("S2.jpg", S2_fin);
	//waitKey(0);
	
	normalize(S0, S0, 1.0, 0, NORM_MINMAX);
	normalize(S1, S1, 1.0, -1.0, NORM_MINMAX);
	normalize(S2, S2, 1.0, -1.0, NORM_MINMAX);


	stokes.push_back(S0);
	stokes.push_back(S1);
	stokes.push_back(S2);

	return stokes;
}

Mat Stokes::aolp(vector<Mat> S)
{
	Mat result = Mat(S[0].rows, S[0].cols, DataType<double>::type);
	for (int i = 0; i < result.rows; i++) {
		for (int k = 0; k < result.cols; k++) {
			result.at<double>(i, k) = fmod(0.5 * atan2(S[2].at<double>(i, k), S[1].at<double>(i, k)), 3.14159265358979323846);
		}
	}
	Mat show = Mat(S[0].rows, S[0].cols, DataType<double>::type);
	normalize(result, show, 1, 0, NORM_MINMAX);
	Mat S0_fin = Mat(S[0].rows / 2, S[0].cols / 2, DataType<double>::type);
	resize(show, S0_fin, S0_fin.size());
	imshow("AoLP", S0_fin);
	waitKey(0);

	return result;
}

Mat Stokes::dolp(vector<Mat> S)
{
	Mat result = Mat(S[0].rows, S[0].cols, DataType<double>::type);
	for (int i = 0; i < result.rows; i++) {
		for (int k = 0; k < result.cols; k++) {
			result.at<double>(i, k) = sqrt((S[1].at<double>(i, k) * S[1].at<double>(i, k)) + (S[2].at<double>(i, k) * S[2].at<double>(i, k))) / (32* S[0].at<double>(i, k));
		}
	}

	Mat show = Mat(S[0].rows, S[0].cols, DataType<double>::type);
	Mat S0_fin = Mat(S[0].rows / 2, S[0].cols / 2, DataType<double>::type);
	resize(result, S0_fin, S0_fin.size());
	imshow("DoLP", S0_fin);
	waitKey(0);

	return result;
}


Mat Stokes::aolpColor(vector<Mat> S)
{
	Mat a = aolp(S);
	Mat d = dolp(S);
	normalize(a, a, 180, 0, NORM_MINMAX);
	normalize(d, d, 255, 0, NORM_MINMAX);
	
	a.convertTo(a, CV_8UC3);
	d.convertTo(d, CV_8UC1);

	Mat show = Mat(S[0].rows, S[0].cols, DataType<double>::type);
	Mat result = Mat(S[0].rows, S[0].cols, CV_8UC3);

	for (int i = 0; i < result.rows; i++) {
		for (int k = 0; k < result.cols; k++) {
			result.at<Vec3b>(i, k)[0] = uchar(a.at<char>(i, k)); 
			result.at<Vec3b>(i, k)[1] = uchar(d.at<char>(i, k));
			result.at<Vec3b>(i, k)[2] = uchar(255);
		}
	}

	cvtColor(result, result, 55);

	Mat S0_fin = Mat(S[0].rows / 2, S[0].cols / 2, DataType<double>::type);
	resize(result, S0_fin, S0_fin.size());
	imshow("AoLP - color", S0_fin);
	waitKey(0);

	return result;
}
