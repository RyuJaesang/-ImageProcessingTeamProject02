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

			// 컬러이미지의 경우 픽셀값 읽어오기
			uchar b = pointer_input[x * 3 + 0];
			uchar g = pointer_input[x * 3 + 1];
			uchar r = pointer_input[x * 3 + 2];

			//그레이스케일의 경우 픽셀값 저장하기
			pointer_output[x] = (r + g + b) / 3.0;
		}
	}
	return img_grayscale;
}

void testPixel(Mat input, int height, int width) {

	for (int y = 0; y < height; y++) {

		uchar* pointer_input = input.ptr<uchar>(y);
		for (int x = 0; x < width; x++) {

			// 컬러이미지의 경우 픽셀값 읽어오기
			uchar b = pointer_input[x * 3 + 0];
			uchar g = pointer_input[x * 3 + 1];
			uchar r = pointer_input[x * 3 + 2];

			//그레이스케일의 경우 픽셀값 저장하기
			printf("(%d, %d)의 픽셀 값 : (%d, %d, %d)\n", x, y, b, g, r);
		}
	}
}

//디텍트 여부 검사
bool isDetect(Mat input, int height, int width) {
	bool isDetect = false;

	//첫 픽셀 검출했을 때 무슨색인지 보고 리턴
	for (int y = 0; y < height; y++) {
		uchar* pointer_input = input.ptr<uchar>(y);
		for (int x = 0; x < width; x++) {

			// 픽셀 읽어오기
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

//사각형 처리
Mat rectFilter(Mat input, int height, int width) {
	//초록색인 끝점 4개 찾아서 detect된 4개점을 바탕으로 이미지 재구성
	//각 방면 끝점 찾기
	bool upleftDetect = false;
	Dot upleftDot;
	for (int y = 0; y < height; y++) {

		uchar* pointer_input = input.ptr<uchar>(y);
		for (int x = 0; x < width; x++) {
			// 픽셀 읽어오기
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
			// 픽셀 읽어오기
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
			// 픽셀 읽어오기
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
			// 픽셀 읽어오기
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
	//endX 더 작은 값
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

	//이미지 다시 생성
	Mat rectFilterImage((endY-startY), (endX - startX), CV_8UC3);
	for (int y = startY; y < endY; y++) {

		uchar* pointer_input = input.ptr<uchar>(y);
		uchar* pointer_output = rectFilterImage.ptr<uchar>((y - startY));
		for (int x = startX; x < endX; x++) {
			// 픽셀 읽어오기
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

			// 픽셀 읽어오기
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
			// 픽셀 읽어오기
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
		printf("썩은 부분 비율 : %f\n", rottenPortion);
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

			// 컬러이미지의 경우 픽셀값 읽어오기
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

	imshow("필터 거치고 빨간색으로 인식되어 검사된 사과 이미지", redColorImage);
	return portion;
}

int main()
{
	//Image 받아오기
	String srcImageName = "C:\\Users\\RyuJaesang\\Desktop\\영상처리\\TeamProject02\\testImage\\DetectRottenApple.png";
	Mat img_color = imread(srcImageName, IMREAD_COLOR);
	Mat filteredImage;
	Mat removeGreenImage;
	int height = img_color.rows;
	int width = img_color.cols; 
	int filteredHeight;
	int filteredWidth;
	float appleSize;

	//1단계 : GreenBox 여부 감지
	if (isDetect(img_color, height, width)) {
		
		//2단계 : 박스 안의 이미지만 남김
		filteredImage = rectFilter(img_color, height, width);
		filteredHeight = filteredImage.rows;
		filteredWidth = filteredImage.cols;

		//3단계 : 초록색 전부 제거
		removeGreenImage = removeGreenColor(filteredImage, filteredHeight, filteredWidth);

		//4단계 : 썩었는지 검사
		if (isRottenApple(removeGreenImage, filteredHeight, filteredWidth)) {
			printf("썩었어\n");
		}
		else {
			//5단계 : 빨간색 비율 검사
			printf("사과 빨간색 비율 %f\n", redPortionOfApple(removeGreenImage, filteredHeight, filteredWidth));
			appleSize = redPortionOfApple(removeGreenImage, filteredHeight, filteredWidth);

			if (appleSize > 0.6) {
				printf("사과 크기 : Extra Large");
			}
			else if (appleSize > 0.55) {
				printf("사과 크기 : Large");
			}
			else if (appleSize > 0.5) {
				printf("사과 크기 : Medium");
			}
			else if (appleSize > 0.45) {
				printf("사과 크기 : Small");
			}
			else if (appleSize > 0.4) {
				printf("사과 크기 : Extra Small");
			}
			else {
				printf("덜 익은 사과입니다.");
			}
		}
	}
	//1단계 : 아직 초록박스 아님
	else {
		printf("아직 빨간박스야 검사할 때가 아니야\n");
	}

	//test
	imshow("원본 이미지", img_color);

	waitKey(0);

	return 0;
}


