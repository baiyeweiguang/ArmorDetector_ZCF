#include<iostream>
#include<opencv2/opencv.hpp>
#include"ArmorDetector.h"
using namespace std;
using namespace cv;

int main()
{
	ArmorDetector ArmorDetector;	//装甲师别模块
	ArmorDetector.COLOR = RED;	//设定敌方颜色
	VideoCapture video("D:/Study/videos/步兵素材红车侧面-ev--3.MOV");
	if (!video.isOpened())
	{
		cout << "打开失败" << endl;
		system("pause");

	}
	while (1)
	{
		Mat copy, src;	//copy用来imshow，src用来提取灯条
		video >> src;
		src.copyTo(copy);
		if (src.empty())
		{
			break;
		}
		ArmorDetector.pretreating(src, src); //预处理
		vector <RotatedRect> lights;	//用来放灯条拟合的旋转矩形
		ArmorDetector.find_light_contours(src, lights);	//识别灯条
		vector<ArmorInfo> armors;	//用来放装甲板
		ArmorDetector.match_lights(lights, armors);	//灯条匹配,计算中心坐标
		copy = ArmorDetector.draw_armor(copy, armors);	//把结果画出来
		namedWindow("test", WINDOW_NORMAL);
		imshow("test", copy);
		waitKey(50 / video.get(CAP_PROP_FPS));
	}

	return 0;
}