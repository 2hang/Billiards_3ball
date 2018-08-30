#include "OurBilliards.hpp"
int MAINBALL = 0;	//1이 노란색 0이 흰색

int main(int argc, char** argv) {

	VideoCapture inputVideo;              
	inputVideo.open(argv[1]);
	//inputVideo.open("sample3.mp4");
	
	if (!inputVideo.isOpened()) {
		cout << "Could not open webcam." << endl;
		return -1;
	}

	Mat src;
	Rect rect(0, 20, 640, 340);
	int cushionCount = 0;

	Billiards billiards;
	Ball ball_yellow;
	Ball ball_red;
	Ball ball_white;

	Ball mainBall;
	Ball sub1Ball;
	bool collisionSuccess = true, cushionSuccess = true;
	bool start = false;

	for (;;) 
	{
		inputVideo >> src;         

		if (src.empty()) 	break;         
		src = src(rect);
		
		//각 공의 정보 저장
		billiards.A_findColor(src, 14, 0, 76, 103, 110, 255, 0, 0, 0, ball_white);	
		billiards.A_findColor(src, 15, 102, 50, 40, 255, 255, 0, 0, 0, ball_yellow);	
		billiards.A_findColor(src, 160, 56, 0, 180, 255, 255, 0, 255, 0, ball_red);

		
		/*
		if (collisionSuccess && cushionSuccess && start)	//맨 처음은 디폴트값으로 모두 true이므로 start로 맨 처음 시작은 제외.
		{
		//성공 점수 1점 통신 부분
			thread t1;
		}
		if (collisionSuccess == false || cushionSuccess == false) {	//한개라도 실패하면 볼칼라 바뀜
			if (MAINBALL == 1) {
				MAINBALL = 0;	mainBall = ball_white;	sub1Ball = ball_yellow;
			}
			else {
				MAINBALL = 1;	mainBall = ball_yellow;	sub1Ball = ball_white;
			}
		}

		start = true;
		*/

		//턴 마다의 메인,서브1 계속 바뀜. (빨간 공은 고정)
		//collisionSuccess = billiards.collision(src, mainBall, sub1Ball, ball_red);
		collisionSuccess = billiards.collision(src, ball_white, ball_yellow, ball_red);

		//쿠션 성공여부 판단
		billiards.collisionWithWall(src, Rect(10, 20, 620, 310), mainBall, cushionCount);
		//billiards.collisionWithWall(src, Rect(10, 20, 620, 310), ball_white, cushionCount);

		imshow("Video", src);
		char c = (char)waitKey(100);
		if (c == 27) 
			break;
	}

	if (billiards.is3CushionSuccess(cushionCount)) //쿠션 성공여부
		cout << "cusionSuccess" << endl;
	if (billiards.Collision_Success()) {
		cout << "collisionSuccess" << endl;
	}
	cushionCount = 0;

	return 0;
}


/*
//정은 생각
충돌이랑 쿠션이 정확히 끝났다는 판단이 있고나서 bool값을 반환해줘야함. 
그렇지 않으면, 계속 false값을 주면 위의 형태에서는 계속하여 main공 값이 변경됨.
다른 방법을 상의해보거나 정확히 끝났다고 판단되었을때 값을 주던가 해야합니당.
*/
