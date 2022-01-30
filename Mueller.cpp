#include "Mueller.h"

Mueller::Mueller(vector<Mat> Images, int N, int angle)
{
	this->matrix = muellerMatrix(Images, N, angle);
}



vector<vector<Mat>> Mueller::muellerMatrix(vector<Mat> Images, int N, int angle)
{
	auto t_start = std::chrono::high_resolution_clock::now();
	int h = Images[0].rows;
	int w = Images[0].cols;
	vector<vector<Mat>> matrix(3);

	Mat coslight;
	Mat coscamera;
	Mat sinlight;
	Mat sincamera;
	Mat A;
	A.create(N*N, 9, CV_32F);


	double pi = 3.14159265359;
	coslight.create(N * N, 1, CV_32F);
	coscamera.create(N * N, 1, CV_32F);
	sinlight.create(N * N, 1, CV_32F);
	sincamera.create(N * N, 1, CV_32F);

	int angleC = 0;
	int angleL = 0;

	for (int i = 0; i < N * N; i++) {
		if (i % N == 0) angleC = 0;
		else angleC = (i % N) * angle;
		if (i % N == 0 && i != 0) angleL += angle;
		coslight.at<float>(i, 0) = cos((angleL) * pi / 90);
		coscamera.at<float>(i, 0) = cos((angleC) * pi / 90);
		sinlight.at<float>(i, 0) = sin((angleL) * pi / 90);
		sincamera.at<float>(i, 0) = sin((angleC) * pi / 90);
	}

	for (int i = 0; i < N * N; i++) {
		A.at<float>(i, 0) = float(1.0);
		A.at<float>(i, 1) = coslight.at<float>(i, 0);
		A.at<float>(i, 2) = sinlight.at<float>(i, 0);
		A.at<float>(i, 3) = coscamera.at<float>(i, 0);
		A.at<float>(i, 4) = coscamera.at<float>(i, 0) * coslight.at<float>(i, 0);
		A.at<float>(i, 5) = coscamera.at<float>(i, 0) * sinlight.at<float>(i, 0);
		A.at<float>(i, 6) = sincamera.at<float>(i, 0);
		A.at<float>(i, 7) = sincamera.at<float>(i, 0) * coslight.at<float>(i, 0);
		A.at<float>(i, 8) = sincamera.at<float>(i, 0) * sinlight.at<float>(i, 0);
	}
	
	Mat At = Mat(9, N*N, A.t().type());
	At = A.t();

	Mat mult = Mat((At * A).size(), CV_32FC(9));
	mult = At * A;
	
	Mat P_inv;
	P_inv.create(9, N*N, CV_32F);
	invert(mult, P_inv, DECOMP_SVD);

	Mat P_inv2;
	P_inv2 = P_inv * At;

	
	for (int z = 0; z < 9; z++) {
		Mat m;
		m.create(h, w, DataType<float>::type);
		m = 0;
		for (int k = 0; k < h; k++) {
			for (int l = 0; l < w; l++) {
				for (int i = 0; i < N * N; i++) {
					m.at<float>(k, l) = m.at<float>(k, l) + Images[i].at<float>(k, l) * P_inv2.at<float>(z, i);
				}
				m.at<float>(k, l) = m.at<float>(k, l);
			}
		}
		Mat S0_fin = Mat(h / 4, w / 4, DataType<double>::type);
		resize(m, S0_fin, S0_fin.size());
		Mat img_color = Mat(S0_fin.size(), CV_8UC3);

		for (int k = 0; k < h / 4; k++) {
			for (int l = 0; l < w / 4; l++) {
				if(S0_fin.at<float>(k, l) > 0) img_color.at<Vec3b>(k, l)[0] = uchar(0);
				else img_color.at<Vec3b>(k, l)[0] = uchar(120);
				img_color.at<Vec3b>(k, l)[1] = uchar(abs(S0_fin.at<float>(k, l))*255);
				img_color.at<Vec3b>(k, l)[2] = uchar(255);
			}
		}
		cvtColor(img_color, img_color, 55);
		matrix[floor(z/3)].push_back(img_color);
	}
	auto t_end = std::chrono::high_resolution_clock::now();
	cout << "Mueller: " << std::chrono::duration<double, std::milli>(t_end - t_start).count() << " ms" << endl;

	return matrix;
}

