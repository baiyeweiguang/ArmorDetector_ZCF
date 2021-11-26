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
class ArmorDetector //װ�װ�ʶ���࣬ʶ����̺�����д��������
{
public:
	int COLOR;	//��ɫ
	int CHANNEL_SUB_THRESH = 50;	//ͨ���������ֵ
	int LIGHT_RECT_SIZE_THRESH = 10;	//������С��С����ֵ
	int LIGHT_RECT_ANGLE_THRESH = 20;	//������ת�Ƕȵ���ֵ
	double LIGHT_RECT_LEN_WID_RATIO = 1.5;	//��������ȵ���ֵ
	int LIGHT_MATCH_ANGLE_THRESH = 10;	//���������ǶȲ����ֵ
	double LIGHT_MATCH_LEN_RATIO = 0.3;	//���Ȳ��ԭ������ֵ
	double LIGHT_MATCH_DIST_LEN_RATIO = 1.5;	//����ͳ��ȱȵ���С��ֵ
	double LIGHT_MATCH_DIST_LEN_RATIO1 = 4;		//����ͳ��ȱȵ������ֵ
	double LIGHT_MATCH_DIST_LEN_RATIO2 = 2.5;	//��װ�װ��ѡ��ֵ
	double LIGHT_MATCH_Y_RATIO = 0.1;	//�߶Ȳ�ȵ���ֵ
public:
	void pretreating(Mat& src,Mat& dst); //Ԥ����
	void find_light_contours(Mat& src,vector<RotatedRect>& lights);//�ҵ���
	void match_lights(vector<RotatedRect>& lights,vector<ArmorInfo>& armors);//����ƥ��
	Mat draw_armor(Mat& image,vector<ArmorInfo>& armors);//����
};