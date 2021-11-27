#include<iostream>
#include<opencv2/opencv.hpp>
#include"ArmorDetector.h"
using namespace std;
using namespace cv;

int main()
{
	ArmorDetector ArmorDetector;	//װ��ʦ��ģ��
	ArmorDetector.COLOR = RED;	//�趨�з���ɫ
	VideoCapture video("D:/Study/videos/�����زĺ쳵����-ev--3.MOV");
	if (!video.isOpened())
	{
		cout << "��ʧ��" << endl;
		system("pause");

	}
	while (1)
	{
		Mat copy, src;	//copy����imshow��src������ȡ����
		video >> src;
		src.copyTo(copy);
		if (src.empty())
		{
			break;
		}
		ArmorDetector.pretreating(src, src); //Ԥ����
		vector <RotatedRect> lights;	//�����ŵ�����ϵ���ת����
		ArmorDetector.find_light_contours(src, lights);	//ʶ�����
		vector<ArmorInfo> armors;	//������װ�װ�
		ArmorDetector.match_lights(lights, armors);	//����ƥ��,������������
		copy = ArmorDetector.draw_armor(copy, armors);	//�ѽ��������
		namedWindow("test", WINDOW_NORMAL);
		imshow("test", copy);
		waitKey(50 / video.get(CAP_PROP_FPS));
	}

	return 0;
}