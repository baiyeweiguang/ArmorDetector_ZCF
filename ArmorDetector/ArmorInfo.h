#pragma once
#include<iostream>
#include<opencv2/opencv.hpp>
using namespace std;
using namespace cv;

class ArmorInfo
{
public:
	ArmorInfo(RotatedRect left,RotatedRect right,Point2f center,int type,int size)
	{
		this->armor_center = center;
		this->left_light = left;
		this->right_light = right;
		this->armor_type = type;
		this->armor_size = size;
	}
	Point2f armor_center; 
	int armor_type;;	
	RotatedRect left_light;
	RotatedRect right_light;
	float armor_size;
};