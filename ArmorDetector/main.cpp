#include<iostream>
#include<opencv2/opencv.hpp>
#include"ArmorDetector.h"
using namespace std;
using namespace cv;

int main()
{
	ArmorDetector ArmorDetector;
	ArmorDetector.COLOR = RED;
	VideoCapture video("D:/Study/videos/步兵素材红车侧面-ev--3.MOV");
	if (!video.isOpened())
	{
		cout << "打开失败" << endl;
		system("pause");

	}
	while (1)
	{
		Mat copy, src;
		video >> src;
		src.copyTo(copy);
		if (src.empty())
		{
			break;
		}
		ArmorDetector.pretreating(src, src);
		vector <RotatedRect> lights;
		ArmorDetector.find_light_contours(src, lights);
		vector<ArmorInfo> armors;
		ArmorDetector.match_lights(lights, armors);
		copy = ArmorDetector.draw_armor(copy, armors);
		namedWindow("test", WINDOW_NORMAL);
		imshow("test", copy);
		waitKey(50 / video.get(CAP_PROP_FPS));
	}

	return 0;
}