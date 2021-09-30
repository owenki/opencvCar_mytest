#include "cv.h"
#include "highgui.h"
#include <windows.h>
#include "cxcore.h"
#include <math.h>
#include <cmath>
#include <vector>
#include <algorithm> 
#include <opencv2/ml/ml.hpp>//������

#define _H 255/180    //HSV H 180->255
#define _S 255      //HSV S
#define _V 255      //HSV V
#define _LT 20		//��ͨ�������
#define _RY 2		//����

#define _CZ 40		//��ֵ��ͨ����Ŀ����ֵ
#define _GD 10		//��ͨ����߶���ֵ
#define _KD 40		//������Ϳ��
#define _MAX_area  1200.00	//��������������
#define _RH 5
#define _dian 0.36 //���������ŵĿհ������ȱ���
#define _kai 0.6	//���ƿ�ʼλ���ַ��߶���ռ����
#define _xu 0.1


#define _BL 440/140 //���Ƴ������
#define _BL2 440/90


#define valuesize 48
#define valuenum 34
#define yangbenshu 25


#define _Width 8
#define _Height 16

using namespace std;
using namespace cv;

#pragma once
class CMyPhoto
{
private:
	vector<CvRect> method_one;
	vector<CvRect> method_two;
	vector<CvRect> method_thr;

	vector<CvRect> daifengge;
	vector<CvRect> order;

	CvANN_MLP bp;
	vector<float> feature;

	

	int way1_chepai_1;
	int way1_chepai_2;
	int way2_chepai_1;
public:
	char number[6];

	char number_tesseract[7];
	CMyPhoto(IplImage* src);
	~CMyPhoto();
	CMyPhoto();

	void chushihua();
	
	//ͨ�÷���
	int isnull(int src);
	
	int chazhibijiao(uchar& a, int& b, int chazhi);
	
	int limiteImageSize(IplImage* src, int maxHeight, int maxWidth);//�޶�ͼƬ��С

	int mycanny(IplImage* src, IplImage* des);//��ֵ������Ӧ�㷨
		int my_canny_low(IplImage* src, int t);//�ݹ鸨����ֵ�㷨
	
	IplImage* RoateImage(const IplImage* src, float &angle, int &fog );//��תͼ��
	
	int chepaiquyu();

	int guiyi(IplImage* src, CvSize size);

	void getfeature(Mat& src);

	float sumMatValue(const Mat& image);

	char recognition(int result);

	char* returnNumber();

	char* returnTesseract();
	//ͼ��λ����
	int dingwei();

	int way1_sobel_threshold(IplImage* src);
		int way1_find(IplImage* src);

	int way1_sobel_canny(IplImage* src);
		int way1_find_canny(IplImage* src);

	int way2_blue_Contours(IplImage* src);
		int way2_find(IplImage* src);

		int getangle(IplImage* src,float &angle,int &fog);

		int shaixuan();
		int shibie(IplImage* src,char* name);
		int xuanze(vector<CvRect> method,char* name);
	
		IplImage* returnRecord();

	int bptrain();
	int bppredict(IplImage* src);

	
	//bool orderbyheight(const CvRect &v1,const CvRect &v2);

	IplImage* yuantu;				//1
	IplImage* chepai;
	IplImage* record;
	
			
};

/*
error msg 
	1			��ʼ��



*/