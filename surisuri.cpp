#include "OurBilliards.hpp"
int MAINBALL = 0;	//1�� ����� 0�� ���

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
		
		//�� ���� ���� ����
		billiards.A_findColor(src, 14, 0, 76, 103, 110, 255, 0, 0, 0, ball_white);	
		billiards.A_findColor(src, 15, 102, 50, 40, 255, 255, 0, 0, 0, ball_yellow);	
		billiards.A_findColor(src, 160, 56, 0, 180, 255, 255, 0, 255, 0, ball_red);

		
		/*
		if (collisionSuccess && cushionSuccess && start)	//�� ó���� ����Ʈ������ ��� true�̹Ƿ� start�� �� ó�� ������ ����.
		{
		//���� ���� 1�� ��� �κ�
			thread t1;
		}
		if (collisionSuccess == false || cushionSuccess == false) {	//�Ѱ��� �����ϸ� ��Į�� �ٲ�
			if (MAINBALL == 1) {
				MAINBALL = 0;	mainBall = ball_white;	sub1Ball = ball_yellow;
			}
			else {
				MAINBALL = 1;	mainBall = ball_yellow;	sub1Ball = ball_white;
			}
		}

		start = true;
		*/

		//�� ������ ����,����1 ��� �ٲ�. (���� ���� ����)
		//collisionSuccess = billiards.collision(src, mainBall, sub1Ball, ball_red);
		collisionSuccess = billiards.collision(src, ball_white, ball_yellow, ball_red);

		//��� �������� �Ǵ�
		//billiards.collisionWithWall(src, Rect(10, 20, 620, 310), mainBall, cushionCount);
		billiards.collisionWithWall(src, Rect(10, 20, 620, 310), ball_white, cushionCount);

		imshow("Video", src);
		char c = (char)waitKey(100);
		if (c == 27) 
			break;
	}

	if (billiards.is3CushionSuccess(cushionCount)) //��� ��������
		cout << "cusionSuccess" << endl;
	if (billiards.Collision_Success()) {
		cout << "collisionSuccess" << endl;
	}
	cushionCount = 0;

	return 0;
}


/*
//���� ����
�浹�̶� ����� ��Ȯ�� �����ٴ� �Ǵ��� �ְ��� bool���� ��ȯ�������. 
�׷��� ������, ��� false���� �ָ� ���� ���¿����� ����Ͽ� main�� ���� �����.
�ٸ� ����� �����غ��ų� ��Ȯ�� �����ٰ� �ǴܵǾ����� ���� �ִ��� �ؾ��մϴ�.
*/