#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

void getEdegeDetection(const Mat& maskX, const Mat& maskY, const Mat& image, Mat& result, uchar thresh)
{
	double offset = 3.0 / 2.0;

	// Mat image 보다 3 픽셀 작은 검정화면으로 만들어줌
	result = Mat::zeros(image.rows - 3, image.cols - 3, image.type());

	uchar sumEdgeX;
	uchar sumEdgeY;
	uchar magnitude;

	for (int r = offset; r < image.rows - offset; r++)
		for (int c = offset; c < image.cols - offset; c++) {
			sumEdgeX = 0;
			sumEdgeY = 0;
			for (int rr = -offset; rr <= offset; rr++) {
				for (int cc = -offset; cc <= offset; cc++) {
					// 영상에 마스크를 연산하는 부분 
					// sumEdgeX : 수직마스크적용, sumEdgeY : 수평마스크적용
					sumEdgeX += image.at<uchar>(r + rr, c + cc) * maskX.at<uchar>(offset + rr, offset + cc);
					sumEdgeY += image.at<uchar>(r + rr, c + cc) * maskY.at<uchar>(offset + rr, offset + cc);
				}
			}
			magnitude = sqrt(pow(sumEdgeY, 2) + pow(sumEdgeX, 2));

			// threshold 적용
			result.at<uchar>(r - offset, c - offset) = ((magnitude > thresh) ? 0 : 255);
		}

}

int main()
{
	Mat src = imread("lena.png", IMREAD_GRAYSCALE);
	Mat roberts, prewitt, sobel;

	Mat robertsX = (Mat_<uchar>(3, 3) << 1, 0, 0, 0, 1, 0, 0, 0, 0);
	Mat robertsY = (Mat_<uchar>(3, 3) << 0, 0, -1, 0, 1, 0, 0, 0, 0);
	getEdegeDetection(robertsX, robertsY, src, roberts, 20);

	Mat prewittX = (Mat_<uchar>(3, 3) << 1, 0, -1, 1, 0, -1, 1, 0, -1);
	Mat prewittY = (Mat_<uchar>(3, 3) << 1, 1, 1, 0, 0, 0, -1, -1, -1);
	getEdegeDetection(prewittX, prewittY, src, prewitt, 50);

	Mat sobelX = (Mat_<uchar>(3, 3) << 1, 0, -1, 2, 0, -2, 1, 0, -1);
	Mat sobelY = (Mat_<uchar>(3, 3) << 1, 2, 1, 0, 0, 0, -1, -2, -1);
	getEdegeDetection(sobelX, sobelY, src, sobel, 50);

	imshow("origin", src);
	imshow("roberts", roberts);
	imshow("prewitt", prewitt);
	imshow("sobel", sobel);
	waitKey(0);

	return 0;
}