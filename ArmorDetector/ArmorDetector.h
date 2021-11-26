#pragma once
#include<iostream>
#include<opencv2/opencv.hpp>
#include"ArmorInfo.h"
using namespace std;
using namespace cv;

#define RED  1
#define BLUE 2
#define SMALL 1
#define BIG 2
class ArmorDetector //装甲板识别类，识别过程函数都写在这里面
{
public:
	int COLOR;	//颜色
	int CHANNEL_SUB_THRESH = 50;	//通道相减的阈值
	int LIGHT_RECT_SIZE_THRESH = 10;	//灯条最小大小的阈值
	int LIGHT_RECT_ANGLE_THRESH = 20;	//灯条旋转角度的阈值
	double LIGHT_RECT_LEN_WID_RATIO = 1.5;	//灯条长宽比的阈值
	int LIGHT_MATCH_ANGLE_THRESH = 10;	//两个灯条角度差的阈值
	double LIGHT_MATCH_LEN_RATIO = 0.3;	//长度差比原长的阈值
	double LIGHT_MATCH_DIST_LEN_RATIO = 1.5;	//距离和长度比的最小阈值
	double LIGHT_MATCH_DIST_LEN_RATIO1 = 4;		//距离和长度比的最大阈值
	double LIGHT_MATCH_DIST_LEN_RATIO2 = 2.5;	//大装甲板候选阈值
	double LIGHT_MATCH_Y_RATIO = 0.1;	//高度差比的阈值
public:
	void pretreating(Mat& src,Mat& dst); //预处理
	void find_light_contours(Mat& src,vector<RotatedRect>& lights);//找灯条
	void match_lights(vector<RotatedRect>& lights,vector<ArmorInfo>& armors);//灯条匹配
	Mat draw_armor(Mat& image,vector<ArmorInfo>& armors);//画线
};