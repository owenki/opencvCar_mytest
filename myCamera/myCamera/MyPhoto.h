#include "cv.h"
#include "highgui.h"
#include <windows.h>
#include "cxcore.h"
#include <math.h>
#include <cmath>
#include <vector>
#include <algorithm> 
#include <opencv2/ml/ml.hpp>//神经网络

#define _H 255/180    //HSV H 180->255
#define _S 255      //HSV S
#define _V 255      //HSV V
#define _LT 20		//连通区域个数
#define _RY 2		//冗余

#define _CZ 40		//数值连通区域的宽度阈值
#define _GD 10		//连通区域高度阈值
#define _KD 40		//车牌最低宽度
#define _MAX_area  1200.00	//最大噪声区域面积
#define _RH 5
#define _dian 0.36 //车牌中央点号的空白区域宽度比例
#define _kai 0.6	//车牌开始位置字符高度所占比例
#define _xu 0.1


#define _BL 440/140 //车牌长宽比例
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
	
	//通用方法
	int isnull(int src);
	
	int chazhibijiao(uchar& a, int& b, int chazhi);
	
	int limiteImageSize(IplImage* src, int maxHeight, int maxWidth);//限定图片大小

	int mycanny(IplImage* src, IplImage* des);//二值的自适应算法
		int my_canny_low(IplImage* src, int t);//递归辅助二值算法
	
	IplImage* RoateImage(const IplImage* src, float &angle, int &fog );//旋转图像
	
	int chepaiquyu();

	int guiyi(IplImage* src, CvSize size);

	void getfeature(Mat& src);

	float sumMatValue(const Mat& image);

	char recognition(int result);

	char* returnNumber();

	char* returnTesseract();
	//图像定位方法
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
	1			初始化



*/