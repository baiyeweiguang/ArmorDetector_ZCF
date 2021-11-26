#pragma once
#include"ArmorDetector.h"
//利用指针进行通道相减
void ArmorDetector::pretreating(Mat& src,Mat& dst)
{
	Mat src_binary = Mat::zeros(src.size(), CV_8UC1);
	int threshold = CHANNEL_SUB_THRESH;
	uchar* sdata = (uchar*)src.data;
	uchar* bdata = (uchar*)src_binary.data;
	int srcsize = src.rows * src.cols;
	for (int i = 0; i < srcsize; i++)
	{
		if (COLOR == RED)
		{
			if (*(sdata + 2) - *(sdata) > threshold)
				*bdata = 255;
		}
		else
		{
			if (*(sdata)-*(sdata + 2) > threshold)
				*bdata = 255;
		}
		sdata += 3;
		bdata++;
	}
	Mat kernelD = getStructuringElement(MORPH_ELLIPSE, Size(3, 3));
	dilate(src_binary, src_binary, kernelD);
	dst = src_binary;
}

//按照条件找到候选灯条
void ArmorDetector::find_light_contours(Mat& src,vector<RotatedRect>& lights)
{
	vector<vector<Point>> contours;
	findContours(src, contours, 1, 1);//找轮廓
	for (int i = 0; i < contours.size(); i++)
	{
		//排除过小的轮廓
		double contour_size = contourArea(contours[i]);
		if (contour_size < LIGHT_RECT_SIZE_THRESH)
		{
			continue;
		}
		//椭圆拟合轮廓
		RotatedRect rrect = fitEllipse(contours[i]);
		Size  wh = rrect.size;
		//长宽比排除
		if (wh.height / wh.width < LIGHT_RECT_LEN_WID_RATIO)
		{
			continue;
		}
		//角度排除
		if (rrect.angle < (180-LIGHT_RECT_ANGLE_THRESH) 
			&& rrect.angle > LIGHT_RECT_ANGLE_THRESH )
		{
			continue;
		}
		lights.push_back(rrect);
	}
}

//灯条匹配
void ArmorDetector::match_lights(vector<RotatedRect>& lights,vector<ArmorInfo>& armors)
{
	int flag = SMALL;//装甲板类型的标志
	//按灯条中心的x坐标从小到大排列
	sort(lights.begin(), lights.end(), [](const RotatedRect& l1, const RotatedRect& l2)
		{
			return l1.center.x < l2.center.x;
		});

	for (size_t i = 0; i < lights.size(); i++)
	{
		for (size_t j = i + 1; j < lights.size(); j++)
		{
			RotatedRect left = lights[i];
			RotatedRect right = lights[j];

			//根据角度差排除
			if (abs(left.angle - right.angle) > LIGHT_MATCH_ANGLE_THRESH && abs(left.angle - right.angle) < 180 - LIGHT_MATCH_ANGLE_THRESH)
			{
				continue;
			}
			//根据长度比排除
			if ((abs(left.size.height - right.size.height) / left.size.height)
				> LIGHT_MATCH_LEN_RATIO)
			{
				continue;
			}
			//根据y的高度差比
			if (abs(left.center.y - right.center.y) / left.size.height > LIGHT_MATCH_Y_RATIO)
			{
				continue;
			}
			//根据距离和长度之比排除
			float distance = abs(left.center.x - right.center.x);//灯条距离
			if (distance / left.size.height < LIGHT_MATCH_DIST_LEN_RATIO
				|| distance / left.size.height > LIGHT_MATCH_DIST_LEN_RATIO1)
			{
				continue;
			}

			float armor_size = left.size.height * distance;//用灯条距离乘以灯条长作为装甲板大小

			//当同时检测到多个装甲板的时候，有可能是将小板和大板中间部分识别成了一个装甲板
			//if (armors.size() != 0)
			//{
			//	if (abs(armors.back().right_light.center.x - left.center.x) < (left.size.height / 2))
			//	{
			//		continue;
			//	}
			//}
			//判断是大还是小
			if ((distance / left.size.height) > LIGHT_MATCH_DIST_LEN_RATIO2)
			{
				flag = BIG;
			}


			Point2f armor_center;
			armor_center.x = (left.center.x + right.center.x) / 2;
			//if (flag == BIG&&(armor_center.x < 900||armor_center.x > 980))
			//{
			//	continue;
			//}
			armor_center.y = (left.center.y + right.center.y) / 2;
			ArmorInfo armor(left, right, armor_center, flag, armor_size);
			armors.push_back(armor);//将找到的装甲板push进装甲板数组
			break;
		}
	}
}

Mat ArmorDetector::draw_armor(Mat& image,vector<ArmorInfo>& armors)	//画线
{
	vector<RotatedRect> lights;
	for (int i = 0; i < armors.size(); i++)
	{
		lights.push_back(armors[i].left_light);
		lights.push_back(armors[i].right_light);
		Point2f center = armors[i].armor_center;
		circle(image, center, 5, Scalar(0, 0, 255));	
		//putText(image, "ARMOR", center, FONT_HERSHEY_SIMPLEX, 2, Scalar(255, 255, 255));
		if (armors[i].armor_type == BIG)
			cout << "big ";
		else
			cout << "small ";
		cout <<"armor" << i + 1 << "  X:" << armors[i].armor_center.x <<
			"  Y:" << armors[i].armor_center.y << endl;
	}

	for (int i = 0; i < lights.size(); i++)
	{
		Point2f points[4];
		lights[i].points(points);
		for (int j = 0; j < 4; j++)
		{
			if (j == 3)
			{
				line(image, points[j], points[0],Scalar(0,255,255),2);
				break;
			}
			line(image, points[j], points[j + 1], Scalar(0,255,255),2);
		}
	}
	return image;
}