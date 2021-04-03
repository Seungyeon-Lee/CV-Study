#include <opencv2/opencv.hpp>
#include <iostream>
#include <queue>

using namespace cv;
using namespace std;

bool visited[501][501];
Point connectivity[8] = { {-1, 0}, {0, 1}, {1, 0}, {0, -1}, {1, 1}, {-1, 1}, {1, -1}, {-1, -1} };

bool isSameColor(Vec3b a, Vec3b b) {
	return a[0] == b[0] && a[1] == b[1] && a[2] == b[2];
}

void bfs_flood_fill(int r, int c, Mat& src, Vec3b flood) {
	queue <Point> que;
	Vec3b origin = src.at<Vec3b>(Point(c, r));
	que.push({ r, c });
	src.at<Vec3b>(Point(c, r)) = flood;

	while (!que.empty()) {
		Point cur = que.front();
		que.pop();

		for (int i = 0; i < 8; i++) {
			int rr = cur.x + connectivity[i].x;
			int cc = cur.y + connectivity[i].y;

			if (rr < 0 || rr >= src.rows || cc < 0 || cc >= src.cols) continue;
			if (visited[rr][cc]) continue;

			if (isSameColor(origin, src.at<Vec3b>(Point(cc, rr))))
			{
				visited[rr][cc] = true;
				src.at<Vec3b>(Point(cc, rr)) = flood;
				que.push({ rr, cc });
			}
		}
	}
}

int main()
{
	Mat src = imread("resource02.png", IMREAD_COLOR);
	Mat dst = imread("resource02.png", IMREAD_COLOR);

	int count = 0;
	for (int r = 0; r < src.rows; r++)
		for (int c = 0; c < src.cols; c++) {
			if (!visited[r][c]) {
				visited[r][c] = true;
				Vec3b rand_color = Vec3b((unsigned)theRNG() & 255, (unsigned)theRNG() & 255, (unsigned)theRNG() & 255);
				bfs_flood_fill(r, c, dst, rand_color);
			}
		}

	imshow("origin", src);
	imshow("result", dst);
	waitKey(0);

	return 0;
}