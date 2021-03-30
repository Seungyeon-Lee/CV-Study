
#include <opencv2/opencv.hpp>
#include <iostream>
#include <stdio.h>

using namespace cv;
using namespace std;

int calcOtsh(Mat src) {
    int histogram[256] = { 0, };

    int otsh_threshold = 0;
    double between_variance = 0;

    for (int r = 0; r < src.rows; r++)
        for (int c = 0; c < src.cols; c++) {
            auto color = src.at<Vec3b>(r, c);
            int histo = (color[0] + color[1] + color[2]) / 3;
            histogram[histo] = histogram[histo] + 1; // 누적 합 구하기
        }

    // 0 ~ 256 사이의 임계값 구하기
    for (int threshold = 0; threshold < 256; threshold++) {
        float w0 = 0.0, w1 = 0.0;
        float u0 = 0.0, u1 = 0.0;
        float u = 0.0;

        // w0 계산 (0 ~ t까지의 누적합)
        for (int i = 0; i < threshold; i++)
        {
            w0 += histogram[i];
            u0 = (double)(u0 + i * histogram[i] / w0); // w0 평균 구하기
        }

        // w1 계산 (t + 1 ~ L - 1까지의 누적합)
        for (int i = threshold + 1; i < 256; i++)
        {
            w1 += histogram[i];
            u1 = (double)(u1 + i * histogram[i] / w1); // w1 평균 구하기
        }

        u = (w0 * u0) + (w1 * u1);
        double result = w0 * w1 * (u1 - u0) * (u1 - u0);

        // 가장 큰 V between(t)를 임계값 T로 취한다.
        if (between_variance < result) {
            between_variance = result;
            otsh_threshold = threshold;
        }
    }

    return otsh_threshold;
}


int main()
{
    Mat src = imread("resource01.jpeg", IMREAD_COLOR); 
    Mat dst, otsu;

    int otsh_threshold = calcOtsh(src);

    imshow("origin", src);
    src = imread("resource01.jpeg", IMREAD_GRAYSCALE);
    threshold(src, dst, otsh_threshold, 255, THRESH_BINARY);
    imshow("dst", dst);
    threshold(src, otsu, 100, 255, THRESH_OTSU);
    imshow("otsu", otsu);

    waitKey(0);

	return 0;
}