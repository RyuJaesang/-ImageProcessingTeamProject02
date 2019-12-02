#include <opencv2/opencv.hpp>
#include <iostream>


using namespace cv;
using namespace std;

typedef struct {
	int x;
	int y;
} Dot;

typedef struct {
	Dot upside;
	Dot leftside;
} SideDetect;

Mat grayscaling(Mat input, int height, int width) {
	Mat img_grayscale(height, width, CV_8UC1);

	//grayscaling
	for (int y = 0; y < height; y++) {

		uchar* pointer_input = input.ptr<uchar>(y);
		uchar* pointer_output = img_grayscale.ptr<uchar>(y);
		for (int x = 0; x < width; x++) {

			// �÷��̹����� ��� �ȼ��� �о����
			uchar b = pointer_input[x * 3 + 0];
			uchar g = pointer_input[x * 3 + 1];
			uchar r = pointer_input[x * 3 + 2];

			//�׷��̽������� ��� �ȼ��� �����ϱ�
			pointer_output[x] = (r + g + b) / 3.0;
		}
	}
	return img_grayscale;
}

void testPixel(Mat input, int height, int width) {

	for (int y = 0; y < height; y++) {

		uchar* pointer_input = input.ptr<uchar>(y);
		for (int x = 0; x < width; x++) {

			// �÷��̹����� ��� �ȼ��� �о����
			uchar b = pointer_input[x * 3 + 0];
			uchar g = pointer_input[x * 3 + 1];
			uchar r = pointer_input[x * 3 + 2];

			//�׷��̽������� ��� �ȼ��� �����ϱ�
			printf("(%d, %d)�� �ȼ� �� : (%d, %d, %d)\n", x, y, b, g, r);
		}
	}
}

//����Ʈ ���� �˻�
bool isDetect(Mat input, int height, int width) {
	bool isDetect = false;

	//ù �ȼ� �������� �� ���������� ���� ����
	for (int y = 0; y < height; y++) {
		uchar* pointer_input = input.ptr<uchar>(y);
		for (int x = 0; x < width; x++) {

			// �ȼ� �о����
			uchar b = pointer_input[x * 3 + 0];
			uchar g = pointer_input[x * 3 + 1];
			uchar r = pointer_input[x * 3 + 2];

			if (!(b > 200 && g > 200 && r > 200) && !(b < 50 && g < 50 && r < 50)) {
				if (b < 100 && g>150 && r < 50 && b > 50) {
					isDetect = true;
					return true;
				}
				else {
					isDetect = true;
					return false;
				}
			}
			if (isDetect) break;
		}
		if (isDetect) break;
	}
}

//�簢�� ó��
Mat rectFilter(Mat input, int height, int width) {
	//�ʷϻ��� ���� 4�� ã�Ƽ� detect�� 4������ �������� �̹��� �籸��
	//�� ��� ���� ã��
	bool upleftDetect = false;
	Dot upleftDot;
	for (int y = 0; y < height; y++) {

		uchar* pointer_input = input.ptr<uchar>(y);
		for (int x = 0; x < width; x++) {
			// �ȼ� �о����
			uchar b = pointer_input[x * 3 + 0];
			uchar g = pointer_input[x * 3 + 1];
			uchar r = pointer_input[x * 3 + 2];

			if (b>50 && b < 100 && g > 150 && g<200 && r < 20) {
				upleftDot.x = x;
				upleftDot.y = y;
				upleftDetect = true;
			}
			if (upleftDetect) break;
		}
		if (upleftDetect) break;
	}

	bool downleftDetect = false;
	Dot downleftDot;
	for (int y = height-1; y >0; y--) {

		uchar* pointer_input = input.ptr<uchar>(y);
		for (int x = 0; x < width; x++) {
			// �ȼ� �о����
			uchar b = pointer_input[x * 3 + 0];
			uchar g = pointer_input[x * 3 + 1];
			uchar r = pointer_input[x * 3 + 2];

			if (b > 50 && b < 100 && g > 150 && g < 200 && r < 20) {
				downleftDot.x = x;
				downleftDot.y = y;
				downleftDetect = true;
			}
			if (downleftDetect) break;
		}
		if (downleftDetect) break;
	}

	bool downrightDetect = false;
	Dot downrightDot;
	for (int y = height-1; y > 0; y--) {

		uchar* pointer_input = input.ptr<uchar>(y);
		for (int x = width-1; x > 0; x--) {
			// �ȼ� �о����
			uchar b = pointer_input[x * 3 + 0];
			uchar g = pointer_input[x * 3 + 1];
			uchar r = pointer_input[x * 3 + 2];

			if (b > 50 && b < 100 && g > 150 && g < 200 && r < 20) {
				downrightDot.x = x;
				downrightDot.y = y;
				downrightDetect = true;
			}
			if (downrightDetect) break;
		}
		if (downrightDetect) break;
	}

	bool uprightDetect = false;
	Dot uprightDot;
	for (int y = 0; y < height; y++) {

		uchar* pointer_input = input.ptr<uchar>(y);
		for (int x = width-1; x > 0; x--) {
			// �ȼ� �о����
			uchar b = pointer_input[x * 3 + 0];
			uchar g = pointer_input[x * 3 + 1];
			uchar r = pointer_input[x * 3 + 2];

			if (b > 50 && b < 100 && g > 150 && g < 200 && r < 20) {
				uprightDot.x = x;
				uprightDot.y = y;
				uprightDetect = true;
			}
			if (uprightDetect) break;
		}
		if (uprightDetect) break;
	}

	int startX;
	int endX;
	int startY;
	int endY;
	//startX
	if (upleftDot.x > downleftDot.x) {
		startX = upleftDot.x;
	}
	else {
		startX = downleftDot.x;
	}
	//startY
	if (upleftDot.y > uprightDot.y) {
		startY = upleftDot.y;
	}
	else {
		startY = uprightDot.y;
	}
	//endX �� ���� ��
	if (uprightDot.x < downrightDot.x) {
		endX = uprightDot.x;
	}
	else {
		endX = downrightDot.x;
	}
	//endY
	if (downleftDot.y < downrightDot.y) {
		endY = downleftDot.y;
	}
	else {
		endY = downrightDot.y;
	}

	//�̹��� �ٽ� ����
	Mat rectFilterImage((endY-startY), (endX - startX), CV_8UC3);
	for (int y = startY; y < endY; y++) {

		uchar* pointer_input = input.ptr<uchar>(y);
		uchar* pointer_output = rectFilterImage.ptr<uchar>((y - startY));
		for (int x = startX; x < endX; x++) {
			// �ȼ� �о����
			uchar b = pointer_input[x * 3 + 0];
			uchar g = pointer_input[x * 3 + 1];
			uchar r = pointer_input[x * 3 + 2];

			pointer_output[(x - startX) * 3 + 0] = b;
			pointer_output[(x - startX) * 3 + 1] = g;
			pointer_output[(x - startX) * 3 + 2] = r;
		}		
	}
	return rectFilterImage;
}

Mat removeGreenColor(Mat input, int height, int width) {
	Mat removeGreenImage(height, width, CV_8UC3);

	for (int y = 0; y < height; y++) {

		uchar* pointer_input = input.ptr<uchar>(y);
		uchar* pointer_output = removeGreenImage.ptr<uchar>(y);
		for (int x = 0; x < width; x++) {

			// �ȼ� �о����
			uchar b = pointer_input[x * 3 + 0];
			uchar g = pointer_input[x * 3 + 1];
			uchar r = pointer_input[x * 3 + 2];

			if (!(b > 30 && b < 100 && g > 80 && r < 200)) {
				pointer_output[x * 3 + 0] = b;
				pointer_output[x * 3 + 1] = g;
				pointer_output[x * 3 + 2] = r;
			}
			else {
				pointer_output[x * 3 + 0] = 255;
				pointer_output[x * 3 + 1] = 255;
				pointer_output[x * 3 + 2] = 255;
			}
		}
	}

	return removeGreenImage;
}

bool isRottenApple(Mat input, int height, int width) {
	float totalPixel = height * width;
	float blackPixel = 0;;
	float rottenPortion;

	for (int y = 0; y < height; y++) {

		uchar* pointer_input = input.ptr<uchar>(y);
		for (int x = 0; x < width; x++) {
			// �ȼ� �о����
			uchar b = pointer_input[x * 3 + 0];
			uchar g = pointer_input[x * 3 + 1];
			uchar r = pointer_input[x * 3 + 2];

			if (b < 100 && g < 100 && r < 100) {
				blackPixel++;
			}
		}
	}
	rottenPortion = blackPixel / totalPixel;

	if (rottenPortion > 0.1) {
		printf("���� �κ� ���� : %f\n", rottenPortion);
		return true;
	}
	else {
		return false;
	}
}

float redPortionOfApple(Mat input, int height, int width) {
	Mat redColorImage(height, width, CV_8UC3);
	float redPixel = 0;
	float totalPixel = height * width;
	float portion = 0;

	for (int y = 0; y < height; y++) {

		uchar* pointer_input = input.ptr<uchar>(y);
		uchar* pointer_output = redColorImage.ptr<uchar>(y);
		for (int x = 0; x < width; x++) {

			// �÷��̹����� ��� �ȼ��� �о����
			uchar b = pointer_input[x * 3 + 0];
			uchar g = pointer_input[x * 3 + 1];
			uchar r = pointer_input[x * 3 + 2];

			if (b < 200 && g < 200 && r>100) {
				pointer_output[x * 3 + 0] = b;
				pointer_output[x * 3 + 1] = g;
				pointer_output[x * 3 + 2] = r;
				redPixel++;
			}
			else {
				pointer_output[x * 3 + 0] = 255;
				pointer_output[x * 3 + 1] = 255;
				pointer_output[x * 3 + 2] = 255;
			}
		}
	}
	portion = redPixel / totalPixel;

	imshow("���� ��ġ�� ���������� �νĵǾ� �˻�� ��� �̹���", redColorImage);
	return portion;
}

int main()
{
	//Image �޾ƿ���
	String srcImageName = "C:\\Users\\RyuJaesang\\Desktop\\����ó��\\TeamProject02\\testImage\\DetectRottenApple.png";
	Mat img_color = imread(srcImageName, IMREAD_COLOR);
	Mat filteredImage;
	Mat removeGreenImage;
	int height = img_color.rows;
	int width = img_color.cols; 
	int filteredHeight;
	int filteredWidth;
	float appleSize;

	//1�ܰ� : GreenBox ���� ����
	if (isDetect(img_color, height, width)) {
		
		//2�ܰ� : �ڽ� ���� �̹����� ����
		filteredImage = rectFilter(img_color, height, width);
		filteredHeight = filteredImage.rows;
		filteredWidth = filteredImage.cols;

		//3�ܰ� : �ʷϻ� ���� ����
		removeGreenImage = removeGreenColor(filteredImage, filteredHeight, filteredWidth);

		//4�ܰ� : ������� �˻�
		if (isRottenApple(removeGreenImage, filteredHeight, filteredWidth)) {
			printf("�����\n");
		}
		else {
			//5�ܰ� : ������ ���� �˻�
			printf("��� ������ ���� %f\n", redPortionOfApple(removeGreenImage, filteredHeight, filteredWidth));
			appleSize = redPortionOfApple(removeGreenImage, filteredHeight, filteredWidth);

			if (appleSize > 0.6) {
				printf("��� ũ�� : Extra Large");
			}
			else if (appleSize > 0.55) {
				printf("��� ũ�� : Large");
			}
			else if (appleSize > 0.5) {
				printf("��� ũ�� : Medium");
			}
			else if (appleSize > 0.45) {
				printf("��� ũ�� : Small");
			}
			else if (appleSize > 0.4) {
				printf("��� ũ�� : Extra Small");
			}
			else {
				printf("�� ���� ����Դϴ�.");
			}
		}
	}
	//1�ܰ� : ���� �ʷϹڽ� �ƴ�
	else {
		printf("���� �����ڽ��� �˻��� ���� �ƴϾ�\n");
	}

	//test
	imshow("���� �̹���", img_color);

	waitKey(0);

	return 0;
}


