#include "OurBilliards.hpp"

Billiards:: Billiards() {
	count1, count2 = 0;
	pre_ang = 0;
}

Billiards::~Billiards() { }

//main공과 sub공의 거리
double Billiards::GetDistance(const Ball& p1, const Ball& p2) {
	return std::sqrt(double((p1.x - p2.x)*(p1.x - p2.x) + (p1.y - p2.y)*(p1.y - p2.y)));
}

//main공과 sub공의 충돌여부 판단
int Billiards::Ang(const Ball& p1, const Ball& p2) {
	r_flag = 0;
	ang = atan2f(p1.y - p2.y, p1.x - p2.x) * 180 / 3.1415f;
	if (abs(abs(ang) - abs(pre_ang)) > 8) {
		r_flag = 1;
	}
	else{
		r_flag = 0;
	}

	pre_ang = ang;
	return r_flag;
}

//main공이 나머지 2개의 공 모두와 충돌했는지 성공여부판단
void Billiards::collision(Mat& image, Ball& mainBall, Ball& sub1, Ball& sub2)
{
	b_flag =Ang(mainBall, center);
	center.x = mainBall.x;
	center.y = mainBall.y;

	if (GetDistance(mainBall, sub1) >= 22 && GetDistance(mainBall, sub1) < 38 && b_flag) {
		putText(image, "Collision", Point(mainBall.x, mainBall.y), FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 0, 0), 4);
		count1++;
	}
	if (GetDistance(mainBall, sub2) >= 22 && GetDistance(mainBall, sub2) < 38 && b_flag)  {
		putText(image, "Collision", Point(mainBall.x, mainBall.y), FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 0, 0), 4);
		count2++;
	}
	//나머지 2개의 공과 충돌이 모두 일어났을 때,
}

void Billiards::A_findColor(Mat& image, int min_H, int min_S, int min_V, int max_H, int max_S, int max_V, int r, int g, int b, Ball& ball) {

	int range_count = 0;
	int low_hue1 = 0, low_hue2 = 0;
	int high_hue1 = 0, high_hue2 = 0;
	Mat imageHSV = image.clone();
	Mat resultImage;
	vector<cv::Vec3f> v3fCircles;
	Scalar lower(min_H, min_S, min_V);
	Scalar upper(max_H, max_S, max_V);

	if (min_H < 10) {
		range_count = 2;

		high_hue1 = 180;
		low_hue1 = min_H + 180;
		high_hue2 = max_H;
		low_hue2 = 0;
	}
	else if (max_H > 170) {
		range_count = 2;

		high_hue1 = min_H;
		low_hue1 = 180;
		high_hue2 = max_H - 180;
		low_hue2 = 0;
	}
	else {
		range_count = 1;

		low_hue1 = min_H;
		high_hue1 = max_H;
	}

	cvtColor(imageHSV, imageHSV, CV_BGR2HSV);
	resultImage = Mat::zeros(image.rows, image.cols, image.type());

	Mat img_mask1, img_mask2;
	inRange(imageHSV, lower, upper, resultImage);
	if (range_count == 2) {
		inRange(imageHSV, Scalar(low_hue2, 50, 50), Scalar(high_hue2, 255, 255), img_mask2);
		resultImage |= img_mask2;
	}

	dilate(resultImage, resultImage, getStructuringElement(MORPH_ELLIPSE, Size(3, 3)));
	dilate(resultImage, resultImage, getStructuringElement(MORPH_ELLIPSE, Size(1, 1)));
	erode(resultImage, resultImage, getStructuringElement(MORPH_ELLIPSE, Size(3, 3)));

	HoughCircles(resultImage, v3fCircles, CV_HOUGH_GRADIENT, 2, resultImage.rows / 8, 40, 15, 10, 13);

	for (size_t i = 0; i < v3fCircles.size(); i++) {
		Vec3i c = v3fCircles[i];
		//circle(image, Point(c[0], c[1]), c[2], Scalar(r, g, b), 2, LINE_AA);
		ball.x = c[0];
		ball.y = c[1];
		ball.radius = c[2];
	}
}

//메인공과 쿠션의 충돌 횟수 카운트 <수정 필요>
void Billiards::collisionWithWall(Mat& image, Rect wall, Ball ball, int& cushionCount) {
	float ballMinX = wall.x + ball.radius;
	float ballMinY = wall.y + ball.radius;
	float ballMaxX = wall.width - ball.radius;
	float ballMaxY = wall.height - ball.radius;
	//////////비교연산(거리/ 노가다)
	//delay
	if (ball.x < ballMinX) {
		cushionCount++;
		//cout << "cushion" << endl;
		putText(image, "cushion", Point(ball.x, ball.y), FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 0, 255), 4);
	}
	else if (ball.x > ballMaxX) {
		cushionCount++;
		//cout << "cushion" << endl;
		putText(image, "cushion", Point(ball.x, ball.y), FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 0, 255), 4);
	}
	// May cross both x and y bounds
	if (ball.y < ballMinY) {
		cushionCount++;
		//cout << "cushion" << endl;
		putText(image, "cushion", Point(ball.x, ball.y), FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 0, 255), 4);
	}
	else if (ball.y > ballMaxY) {
		cushionCount++;
		//cout << "cushion" << endl;
		putText(image, "cushion", Point(ball.x, ball.y), FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 0, 255), 4);
	}
	return;
}

//1턴 끝난 후
bool Billiards::is3CushionSuccess(int& cushionCount) {
	if (cushionCount < 3) {
		cushionCount = 0;
		return false;
	}
	else {
		cushionCount = 0;
		return true;
	}
}

bool Billiards::Collision_Success() {
	if (count1 >= 1 && count2 >= 1) {
		count1, count2 = 0;
		return true;
	}
	else {
		count1, count2 = 0;
		return false;
	}
}