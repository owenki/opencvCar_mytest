#include "stdafx.h"
#include "MyPhoto.h"
#include "array.h"
#include "tesseract.h"
using namespace std;
using namespace cv;

bool orderbyx(const CvRect &v1, const CvRect &v2)
{
	return v1.x < v2.x;//升序排列  
}
bool orderbyx_inv(const CvRect &v1, const CvRect &v2)
{
	return v1.x > v2.x;//升序排列  
}

bool orderbyy(const CvRect &v1, const CvRect &v2)
{
	return v1.y < v2.y;//升序排列  
}

bool orderbyheight(const CvRect &v1, const CvRect &v2)
{
	return v1.height < v2.height;//升序排列  
}

bool orderbywidth(const CvRect &v1, const CvRect &v2)
{
	return v1.width < v2.width;//升序排列  
}

void lhMorpRErode(const IplImage* src, const IplImage* msk, IplImage* dst, IplConvKernel* se = NULL, int iterations = -1)

{

	assert(src != NULL  && msk != NULL && dst != NULL && src != dst);

	if (iterations < 0)

	{

		//腐蚀重建

		cvMax(src, msk, dst);

		cvErode(dst, dst, se);

		cvMax(dst, msk, dst);

		IplImage*  temp1 = cvCreateImage(cvGetSize(src), 8, 1);

		IplImage*  temp2 = cvCreateImage(cvGetSize(src), 8, 1);

		do

		{

			cvCopy(dst, temp1);

			cvErode(dst, dst, se);

			cvMax(dst, msk, dst);

			cvCmp(temp1, dst, temp2, CV_CMP_NE);

		}

		while (cvSum(temp2).val[0] != 0);

		cvReleaseImage(&temp1);

		cvReleaseImage(&temp2);

		return;

	}

	else if (iterations == 0)

	{

		cvCopy(src, dst);

	}

	else

	{

		//普通测地腐蚀 p137(6.2)

		cvMax(src, msk, dst);

		cvErode(dst, dst, se);

		cvMax(dst, msk, dst);

		for (int i = 1; i<iterations; i++)

		{

			cvErode(dst, dst, se);

			cvMax(dst, msk, dst);

		}

	}

}
void lhMorpRDilate(const IplImage* src, const IplImage* msk, IplImage* dst, IplConvKernel* se = NULL, int iterations = -1)

{

	assert(src != NULL && msk != NULL && dst != NULL && src != dst);

	if (iterations < 0)

	{

		//膨胀重建

		cvMin(src, msk, dst);

		cvDilate(dst, dst, se);

		cvMin(dst, msk, dst);

		IplImage*  temp1 = cvCreateImage(cvGetSize(src), 8, 1);

		IplImage*  temp2 = cvCreateImage(cvGetSize(src), 8, 1);

		do

		{

			cvCopy(dst, temp1);

			cvDilate(dst, dst, se);

			cvMin(dst, msk, dst);

			cvCmp(temp1, dst, temp2, CV_CMP_NE);

		}

		while (cvSum(temp2).val[0] != 0);

		cvReleaseImage(&temp1);

		cvReleaseImage(&temp2);

		return;

	}

	else if (iterations == 0)

	{

		cvCopy(src, dst);

	}

	else

	{

		//普通测地膨胀 p136(6.1)

		cvMin(src, msk, dst);

		cvDilate(dst, dst, se);

		cvMin(dst, msk, dst);

		for (int i = 1; i<iterations; i++)

		{

			cvDilate(dst, dst, se);

			cvMin(dst, msk, dst);

		}

	}

}

void lhMorpROpen(const IplImage* src, IplImage* dst, IplConvKernel* se = NULL, int iterations = 1)

{

	assert(src != NULL  && dst != NULL && src != dst);

	IplImage*  temp = cvCreateImage(cvGetSize(src), 8, 1);

	cvErode(src, temp, se, iterations);

	lhMorpRDilate(temp, src, dst, se, -1);

	cvReleaseImage(&temp);

}
void lhMorpRClose(const IplImage* src, IplImage* dst, IplConvKernel* se = NULL, int iterations = 1)

{

	assert(src != NULL  && dst != NULL && src != dst);

	IplImage*  temp = cvCreateImage(cvGetSize(src), 8, 1);

	cvDilate(src, temp, se, iterations);

	lhMorpRErode(temp, src, dst, se, -1);

	cvReleaseImage(&temp);

}


CMyPhoto::~CMyPhoto()
{
	cvReleaseImage(&yuantu);
}
CMyPhoto::CMyPhoto()
{
	yuantu = NULL;
	way1_chepai_1 = 0;
	way1_chepai_2 = 0;
	way2_chepai_1 = 0;
}
CMyPhoto::CMyPhoto(IplImage* src)
{
	yuantu = NULL;
	way1_chepai_1 = 0;
	way1_chepai_2 = 0;
	way2_chepai_1 = 0;
	yuantu = cvCreateImage(cvSize(src->width, src->height), src->depth, src->nChannels);
	cvCopy(src, yuantu);
}

/*
初始化 
1.修改原图尺寸
*/
void CMyPhoto::chushihua()
{
	if (isnull(1)){
		limiteImageSize(yuantu, 600, 800);
		/*cvShowImage("初始化", yuantu);*/
	}
	else
	{
		throw -1;
	}
}

/*/辅助函数/*/
int CMyPhoto::isnull(int src)
{
	switch (src)
	{
	case 1:
		if (yuantu == NULL) return -1;
		break;
	case 2:
		if (chepai == NULL) return -1;
		break;
	case 3:
		if (record == NULL) return -1;
		break;
	default:
		break;
	}
	return 1;
}

/*修改src尺寸*/
int CMyPhoto::limiteImageSize(IplImage* src, int maxHeight, int maxWidth)
{
	int maxSize = 0;
	float temp_width = src->width;
	float temp_height = src->height;
	float temp_bili = temp_width / temp_height;
	float bili = maxWidth / maxHeight;
	int fog = 0;
	CvSize resize;
	if (temp_width > maxWidth || temp_height > maxHeight){
		int a;
		a = maxWidth / temp_bili;
		if (a > maxHeight)
		{
			resize.height = maxHeight;
			resize.width = maxHeight*temp_bili;
		}
		else
		{
			resize.width = maxWidth;
			resize.height = a;
		}
		
	}
	else
	{
		fog = 1;
	}
	if (fog == 1)
	{
		return maxSize;
	}
	else
	{
		IplImage* temp_image = cvCreateImage(cvSize(src->width,src->height),src->depth,src->nChannels);
		cvCopy(src, temp_image);
		cvReleaseImage(&src);
		src = cvCreateImage(resize,temp_image->depth,temp_image->nChannels);
		cvResize(temp_image, src, CV_INTER_AREA);
		cvReleaseImage(&temp_image);
		maxSize = src->width > src->height ? src->width : src->height;

		return maxSize;
	}

}

/*蓝色区域比较方法*/
int CMyPhoto::chazhibijiao(uchar &a, int &b, int chazhi){
	return abs(a - b) > chazhi ? 0 : 1;
}

/*自适应二值化操作*/
int CMyPhoto::mycanny(IplImage* src, IplImage* des)
{
	int max = 0, min = 255, i = 0, j = 0, n = 0;

	for (j = 0; j<src->height; j++)
	for (i = 0; i<src->width; i++)
	{
		if (CV_IMAGE_ELEM(src, uchar, j, i)>max)
			max = CV_IMAGE_ELEM(src, uchar, j, i);
		else if (CV_IMAGE_ELEM(src, uchar, j, i)<min)
			min = CV_IMAGE_ELEM(src, uchar, j, i);
	}
	int canny_low = my_canny_low(src, (max + min) / 2);
	int canny_hight = 0.7*max;
	cvCanny(src, des, canny_low, canny_hight);
	return 1;
}

/*自适应 二值 阈值计算*/
int CMyPhoto::my_canny_low(IplImage* src, int t)
{
	int i, j;
	int white = 0, w_num = 0, black = 0, b_num = 0;
	for (j = 0; j<src->height; j++)
	for (i = 0; i<src->width; i++)
	{
		if (CV_IMAGE_ELEM(src, uchar, j, i)>t){
			white += CV_IMAGE_ELEM(src, uchar, j, i);
			w_num++;
		}
		else if (CV_IMAGE_ELEM(src, uchar, j, i)<t){
			black += CV_IMAGE_ELEM(src, uchar, j, i);
			b_num++;
		}
	}

	int temp = 0.5*(white / w_num + black / b_num);
	if (temp != t)
		my_canny_low(src, temp);
	else
		return temp;
}

/*旋转图片 返回新图*/
IplImage* CMyPhoto::RoateImage(const IplImage* src, float &angle, int &fog)
{
	IplImage* dst=NULL;
	#define PI  3.1415926535897932384626433832795
	float jiaodu = angle*PI / 180;
	double a = fabs(sin(jiaodu));
	double b = fabs(cos(jiaodu));
	if (angle > 90)
		angle = angle - 90;
	if (angle >= 45.0&&angle <= 90.0)
	{
		angle = 90 - angle;
		fog = -fog;
	}
	if (angle >= 0 && angle < 45.0){
		angle = angle;
	}
	int w_src = src->width;
	int h_src = src->height;

	int w_dst = (h_src * a + w_src * b)*2;
	int h_dst =(w_src * a + h_src * b)*2;
	dst = cvCreateImage(cvSize(w_dst, h_dst), src->depth, src->nChannels);
	cvZero(dst);
	cvSetImageROI(dst, cvRect(w_dst / 2.0 - src->width / 2.0, h_dst / 2.0 - src->height / 2.0, src->width, src->height));
	cvCopy(src, dst);
	cvResetImageROI(dst);	
	CvPoint2D32f center;
	center.x = w_dst / 2;
	center.y = h_dst / 2;
	double mymap[6];
	CvMat arr = cvMat(2, 3, CV_32FC1, mymap);
	cv2DRotationMatrix(center, angle*fog, 1, &arr);	
	cvWarpAffine(dst, dst, &arr);
	/*cvShowImage("jiaodu", dst);*/
	return dst;
}

IplImage* CMyPhoto::returnRecord()
{
	return record;
}


int CMyPhoto::way1_sobel_threshold(IplImage* src)
{
	if (src == NULL)
		return -1;
	
	IplImage *gray = cvCreateImage(cvSize(src->width, src->height), IPL_DEPTH_8U, 1);//灰度图片  单通道
	cvCvtColor(src, gray, CV_RGB2GRAY);
	cvSmooth(gray, gray, CV_MEDIAN);//中值滤波

	IplImage *sobel = cvCreateImage(cvGetSize(gray), IPL_DEPTH_16S, 1);//灰度图片  单通道
	cvSobel(gray, sobel, 2, 0, 3);

	IplImage *normalize = cvCreateImage(cvGetSize(gray), IPL_DEPTH_8U, 1);//灰度图片  单通道
	IplImage *normalize2 = cvCreateImage(cvGetSize(gray), IPL_DEPTH_8U, 1);//灰度图片  单通道
	cvNormalize(sobel, normalize, 255, 0, CV_MINMAX);//归一化

	cvThreshold(normalize, normalize, 100, 255, CV_THRESH_BINARY);//二值化
	
	for (int i = 0; i < normalize->height; i++){//二值化结果取反
		for (int j = 0; j < normalize->width; j++)
		{
			CV_IMAGE_ELEM(normalize, uchar, i, j) = 255 - CV_IMAGE_ELEM(normalize, uchar, i, j);
		}
	}

	IplConvKernel* K = cvCreateStructuringElementEx(3, 1, 0, 0, CV_SHAPE_RECT);
	IplConvKernel* K1 = cvCreateStructuringElementEx(3, 3, 0, 0, CV_SHAPE_RECT);

	
	
	lhMorpRClose(normalize, normalize2, K, 8);
	//cvMorphologyEx(gray, gray, temp, K, CV_MOP_CLOSE, 13);//闭操作

	lhMorpROpen(normalize2, normalize, K1, 1);
	//cvMorphologyEx(normalize, normalize, temp, K1, CV_MOP_OPEN, 1);//开操作

	return way1_find(normalize);

	
}
int CMyPhoto::way1_find(IplImage* src)
{
	CvMemStorage* storage = cvCreateMemStorage(0);
	IplImage* dst = cvCreateImage(cvGetSize(src), 8, 3);
	cvCvtColor(src, dst, CV_GRAY2BGR);
	CvScalar color = CV_RGB(255, 0, 0);
	CvSeq* contours = 0;
	cvFindContours(src, storage, &contours, sizeof(CvContour), CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);
	for (; contours != 0; contours = contours->h_next)
	{
		//使用边界框的方式
		CvRect aRect = cvBoundingRect(contours, 1);
		int tmparea = aRect.height*aRect.height;
		if (((double)aRect.width / (double)aRect.height>3)&& ((double)aRect.width / (double)aRect.height<6.3) && tmparea >= 140  )
		{
			cvRectangle(dst, cvPoint(aRect.x, aRect.y), cvPoint(aRect.x + aRect.width, aRect.y + aRect.height), color, 2);

			method_one.push_back(aRect);
			way1_chepai_1++;
		
		}
	}

	return way1_chepai_1;
}

int CMyPhoto::way1_sobel_canny(IplImage* src)
{
	if (src == NULL)
		return -1;

	IplImage *gray = cvCreateImage(cvSize(src->width, src->height), IPL_DEPTH_8U, 1);//灰度图片  单通道
	cvCvtColor(src, gray, CV_RGB2GRAY);
	/*cvShowImage("src", src);
	cvShowImage("gray", gray);*/
	cvSmooth(gray, gray, CV_MEDIAN);//中值滤波

	IplImage *sobel = cvCreateImage(cvGetSize(gray), IPL_DEPTH_16S, 1);//灰度图片  单通道
	cvSobel(gray, sobel, 2, 0, 3);
	//cvShowImage("sobel", sobel);
	IplImage *normalize = cvCreateImage(cvGetSize(gray), IPL_DEPTH_8U, 1);//灰度图片  单通道
	cvNormalize(sobel, normalize, 255, 0, CV_MINMAX);//归一化

//二值化
	
	mycanny(normalize, normalize);

	
	IplConvKernel* K = cvCreateStructuringElementEx(3, 1, 0, 0, CV_SHAPE_RECT);
	IplConvKernel* K1 = cvCreateStructuringElementEx(3, 3, 0, 0, CV_SHAPE_RECT);

	IplImage *temp = cvCreateImage(cvGetSize(gray), IPL_DEPTH_8U, 1);//灰度图片  单通道

	//cvMorphologyEx(normalize, normalize, temp, K, CV_MOP_CLOSE, 13);//闭操作

	//cvMorphologyEx(normalize, normalize, temp, K1, CV_MOP_OPEN, 1);//开操作
	lhMorpRClose(normalize, temp, K, 8);
	

	lhMorpROpen(temp, normalize, K1, 1);
	
	return way1_find_canny(normalize);


}
int CMyPhoto::way1_find_canny(IplImage* src)
{
	CvMemStorage* storage = cvCreateMemStorage(0);
	IplImage* dst = cvCreateImage(cvGetSize(src), 8, 3);
	cvCvtColor(src, dst, CV_GRAY2BGR);
	CvScalar color = CV_RGB(255, 0, 0);
	CvSeq* contours = 0;
	cvFindContours(src, storage, &contours, sizeof(CvContour), CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);
	for (; contours != 0; contours = contours->h_next)
	{
		//使用边界框的方式
		CvRect aRect = cvBoundingRect(contours, 1);
		int tmparea = aRect.height*aRect.height;
		if (((double)aRect.width / (double)aRect.height>3) && ((double)aRect.width / (double)aRect.height<6.3) && tmparea >= 500)
		{
			cvRectangle(dst, cvPoint(aRect.x, aRect.y), cvPoint(aRect.x + aRect.width, aRect.y + aRect.height), color, 2);
			method_two.push_back(aRect);
			way1_chepai_2++;

		}
	}

	return way1_chepai_2;
}

int CMyPhoto::way2_blue_Contours(IplImage* src)
{
	if (src == NULL)
		return -1;

	int stand_R = 28;
	int stand_G = 63;
	int stand_B = 138;

	IplImage *fuben = cvCreateImage(cvSize(src->width, src->height), IPL_DEPTH_8U, 3);
	IplImage *B = cvCreateImage(cvSize(src->width, src->height), IPL_DEPTH_8U, 1);
	IplImage *G = cvCreateImage(cvSize(src->width, src->height), IPL_DEPTH_8U, 1);
	IplImage *R = cvCreateImage(cvSize(src->width, src->height), IPL_DEPTH_8U, 1);
	IplImage *gray = cvCreateImage(cvSize(src->width, src->height), IPL_DEPTH_8U, 1);
	IplImage *gray2 = cvCreateImage(cvSize(src->width, src->height), IPL_DEPTH_8U, 1);
	cvZero(gray);
	cvSplit(src, B,G,R, 0);

	for (int i = 0; i < src->height; i++)
	{
		for (int j= 0; j <src->width; j++)
		{
			if (chazhibijiao(CV_IMAGE_ELEM(R, uchar, i, j), stand_R, 50) && \
				chazhibijiao(CV_IMAGE_ELEM(G, uchar, i, j), stand_G, 50) && \
				chazhibijiao(CV_IMAGE_ELEM(B, uchar, i, j), stand_B, 60))
			{
				CV_IMAGE_ELEM(gray, uchar, i, j) = 255;
			}
			else
			{
				CV_IMAGE_ELEM(gray, uchar, i, j) = 0;
			}
		}
	}


	IplConvKernel* K = cvCreateStructuringElementEx(3, 1, 0, 0, CV_SHAPE_RECT);
	IplConvKernel* K1 = cvCreateStructuringElementEx(3, 3, 0, 0, CV_SHAPE_RECT);

	
	lhMorpRClose(gray, gray2, K, 3);
	//cvMorphologyEx(gray, gray, temp, K, CV_MOP_CLOSE, 13);//闭操作

	lhMorpROpen(gray2, gray, K1, 1);

	//cvMorphologyEx(gray, gray, temp, K1, CV_MOP_OPEN, 1);//开操作
	//cvShowImage("blue", gray);
	return way2_find(gray);
	
}
int CMyPhoto::way2_find(IplImage* src)
{
	CvMemStorage* storage = cvCreateMemStorage(0);
	IplImage* dst = cvCreateImage(cvGetSize(src), 8, 3);
	cvCvtColor(src, dst, CV_GRAY2BGR);
	CvScalar color = CV_RGB(255, 0, 0);
	CvSeq* contours = 0;
	cvFindContours(src, storage, &contours, sizeof(CvContour), CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);
	for (; contours != 0; contours = contours->h_next)
	{
		//使用边界框的方式
		CvRect aRect = cvBoundingRect(contours, 1);
		int tmparea = aRect.height*aRect.height;
		if (((double)aRect.width / (double)aRect.height>3) && ((double)aRect.width / (double)aRect.height<6.3) && tmparea >= 140)
		{
			cvRectangle(dst, cvPoint(aRect.x, aRect.y), cvPoint(aRect.x + aRect.width, aRect.y + aRect.height), color, 2);
			method_thr.push_back(aRect);
			way2_chepai_1++;

		}
	}
	return way2_chepai_1;
}


/*预定位*/
int CMyPhoto::dingwei()
{
	way1_sobel_canny(yuantu);
	way1_sobel_threshold(yuantu);
	way2_blue_Contours(yuantu);
	
	return 1;
}

/*筛选*/
int CMyPhoto::shaixuan()
{
	xuanze(method_one, "so");
	xuanze(method_two, "cn");
	xuanze(method_thr, "bl");

	return 1;
}

/*返回待识别区域数量*/
int CMyPhoto::chepaiquyu()
{
	return way1_chepai_1 + way1_chepai_2 + way2_chepai_1;
}
/* 精确筛选 识别*/
int CMyPhoto::shibie(IplImage* src,char* name)
{
	IplImage *gray = cvCreateImage(cvGetSize(src), src->depth, 1);
	IplImage *gray2 = cvCreateImage(cvGetSize(src), src->depth, 3);
	cvCvtColor(src, gray,CV_RGB2GRAY);
	cvNormalize(gray, gray, 255, 0, CV_MINMAX);
	cvThreshold(gray, gray, 150, 255, CV_THRESH_BINARY);
	//mycanny(gray, gray);
		CvSeq *contour=NULL;
		cvCvtColor(gray, gray2, CV_GRAY2BGR);
		int count = 0;

		CvMemStorage *storage = cvCreateMemStorage();
		CvScalar color = cvScalar(0,0,255);// 
		CvContourScanner scanner = NULL;
		/*scanner= cvStartFindContours(img_test, storage, sizeof(CvContour), CV_RETR_CCOMP, CV_CHAIN_APPROX_NONE, cvPoint(0, 0));*/
		scanner = cvStartFindContours(gray, storage, 88, CV_RETR_EXTERNAL);
		
		//开始遍历轮廓树   
		CvRect rect;
		daifengge.clear();
		while (contour = cvFindNextContour(scanner))
		{
			
			rect = cvBoundingRect(contour, 0);
			if (rect.height > rect.width && rect.height>gray->height / 3.0&&rect.width<gray->width / 5.0/*&& rect.height<gray->height*0.8*/)
			{
				cvRectangleR(gray2, rect, color, 1);
				count++;
				daifengge.push_back(rect);
			}
		}
		cvShowImage("r", gray2);
		cvEndFindContours(&scanner);
		
		cvReleaseMemStorage(&storage);
		
		if (count >=6)
		{
			
			vector<CvRect> temp;
			
			for (int i = 0; i < count; i++)
			{
				temp.push_back(  daifengge.at(i));
			}

			sort(temp.begin(), temp.end(), orderbyheight);
			
			int height = temp.at((temp.size()-1)/2).height;

			for (int i = 0; i < count; i++)
			{
				if (abs(height - temp.at(i).height) > 5){
					temp.at(i).height = 0;
				}
			}
			int count_num = 0;
			for (int i = 0; i < count; i++)
			{
				if ( temp.at(i).height!=0){
					order.push_back(temp.at(i));
					count_num++;
				}
			}
			
			if (count_num == 6 || count_num == 7)
			{

				CvRect tempsize;
				if (count_num == 7){
					sort(order.begin(), order.end(), orderbyx_inv);
					order.pop_back();
				}
				sort(order.begin(), order.end(), orderbyy);
				tempsize.y = order.at(0).y;
				tempsize.height = order.at(order.size() - 1).y + order.at(order.size() - 1).height - tempsize.y;

				sort(order.begin(), order.end(), orderbyx);
				tempsize.x = order.at(0).x;
				tempsize.width = order.at(order.size() - 1).x + order.at(order.size() - 1).width - tempsize.x;

				int i,m=0;
			
				for (i=0; i < 6; i++)
				{
					int t;
					/*cvRectangleR(gray2, temp.at(i), color, 1);*/
					cvSetImageROI(src, order.at(i));
					
					t=bppredict(src);
					number[m] = recognition(t);
					cvResetImageROI(src);
					m++;
				}
				//cvShowImage("result", gray2);
				
				cvSetImageROI(src,tempsize);
				//cvShowImage("src", src);
				IplImage* binary;
				record = cvCreateImage(cvSize(src->roi->width, src->roi->height), src->depth, 3);
				binary = cvCreateImage(cvSize(src->roi->width, src->roi->height), src->depth, 1);
				cvCopy(src, record);
				cvCvtColor(record, binary, CV_RGB2GRAY);
				cvNormalize(binary, binary, 255, 0, CV_MINMAX);
				cvThreshold(binary, binary, 122, 255, CV_THRESH_BINARY_INV);
		
				cvSaveImage("record.jpg", binary);

				/*system("tesseract record.jpg -psm 7 result");*/
				char* temp = mystudyTesseract("record.jpg");
				strncpy(number_tesseract, temp,7);
				cvResetImageROI(src);
				//cvShowImage("chepai", record);
			}

			return 1;
		}
		else
		{
			/*daifengge.clear();*/
			return -1;
		}
			
		
}


/*根据 预定位 进行精确筛选*/
int CMyPhoto::xuanze(vector<CvRect> method,char* name)
{
	int size = method.size();
	
	for (int i = 0; i < size; i++)
	{
		char subname[20];
		float angle = 0;
		int fog = 1;
		IplImage* localimage=NULL;
		sprintf(subname, "%s%d", name,i);
	
		cvSetImageROI(yuantu, method.at(i));
		chepai = cvCreateImage(cvSize(method.at(i).width, method.at(i).height), yuantu->depth, yuantu->nChannels);
		cvCopy(yuantu, chepai);
		getangle(chepai,angle,fog);
			
		if (angle > 10.0){
			
			localimage = RoateImage(chepai, angle, fog);
			shibie(localimage, subname);
		}
		else
		{
			shibie(chepai, subname);
		}
			
		cvResetImageROI(yuantu);
	}
	return 1;
}

/*获取车牌角度*/
int CMyPhoto::getangle(IplImage* src, float &angle, int &fog)
{
	IplImage *gray = cvCreateImage(cvGetSize(src), src->depth, 1);
	IplImage *gray2 = cvCreateImage(cvGetSize(src), src->depth, 3);
	
	cvCvtColor(src, gray, CV_RGB2GRAY);
	cvNormalize(gray, gray, 255, 0, CV_MINMAX);
	cvThreshold(gray, gray, 150, 255, CV_THRESH_BINARY_INV);
	//cvShowImage("s", gray);
	mycanny(gray, gray);
	cvCvtColor(gray, gray2, CV_GRAY2BGR);
	CvMemStorage* storage = cvCreateMemStorage(0);
	CvSeq* lines = 0;

	int length = 0;
	int max = 0;
	CvPoint pt1=cvPoint(0,0), pt2=cvPoint(0,0);

	lines = cvHoughLines2(gray, storage, CV_HOUGH_PROBABILISTIC, 1, CV_PI / 180, gray->width/3, 5, 20);

	for (int i = 0; i <lines->total; i++)
	{
		CvPoint* line = (CvPoint*)cvGetSeqElem(lines, i);
		
		length = (line[0].x - line[1].x)*(line[0].x - line[1].x) + (line[0].y - line[1].y)*(line[0].y - line[1].y);
		if (length>max){
			max = length;
			pt1 = line[0];
			pt2 = line[1];
		}
		
	}
	if (((pt1.x - pt2.x)<0 && (pt1.y - pt2.y)<0) || ((pt1.x - pt2.x)>0 && (pt1.y - pt2.y)>0))
		fog = 1; //旋转点为左下角
	else
		fog = -1;//旋转点为右下角

	angle = fabs(atan((pt1.y - pt2.y)*1.0 / (pt1.x - pt2.x)*1.0) * 180/PI);
	if (pt1.x - pt2.x == 0)
		angle = 0;

	return 1;
}

/*训练BP神经网络*/
int CMyPhoto::bptrain()
{
	CvANN_MLP bp;

	CvANN_MLP_TrainParams param;
	param.term_crit = cvTermCriteria(CV_TERMCRIT_ITER, 10000, 0.001);  //设置结束条件
	param.train_method = CvANN_MLP_TrainParams::BACKPROP;       //训练方法采用BackProgation
	param.bp_dw_scale = 0.1;
	param.bp_moment_scale = 0.1;

	float trainingData[valuenum*yangbenshu][valuesize];

	for (int i = 0; i <valuenum; i++)
	{
		for (int m = 0; m < yangbenshu; m++)
		{
			char add[255] = "";
			strcpy(add, address[i]);

			if (i<10)
				sprintf(add, "%s%c (%d).png", add, i + 48, m + 1);
			else if (i>9 && i<18)
				sprintf(add, "%s%c (%d).png", add, i + 55, m + 1);
			else if (i>17 && i<23)
				sprintf(add, "%s%c (%d).png", add, i + 56, m + 1);
			else
				sprintf(add, "%s%c (%d).png", add, i + 57, m + 1);
			//cout << i << " load " << " " << add << endl;

			Mat mm = imread(add);
			getfeature(mm);
			for (int j = 0; j < valuesize; j++)
			{
				trainingData[i*yangbenshu + m][j] = feature.at(j);
			}
			mm.release();
			feature.clear();
		}

	}

	Mat trainingDataMat(valuenum*yangbenshu, valuesize, CV_32FC1, trainingData);
	float labels[valuenum*yangbenshu][6];

	for (int m = 0; m< valuenum; m++)
	{
		for (int n = 0; n < yangbenshu; n++)
		{
			for (int s = 0; s < 6; s++)
			{
				labels[yangbenshu * m + n][s] = num[m][s];
			}

		}

	}

	Mat labelsMat(valuenum*yangbenshu, 6, CV_32FC1, labels);
	Mat layerSizes = (Mat_<int>(1, 3) << valuesize, 47, 6);
	bp.create(layerSizes, CvANN_MLP::SIGMOID_SYM, 1, 1);//缺省参数不能丢弃
	bp.train(trainingDataMat, labelsMat, Mat(), Mat(), param);  //训练
	bp.save("43is样本50.xml"); //save classifier

	return 1;
}

/*图像线性归一*/
int CMyPhoto::guiyi(IplImage* src,CvSize size)
{
	IplImage* dst = cvCreateImage(cvGetSize(src), src->depth, src->nChannels);
	cvCopy(src, dst);
	cvReleaseImage(&src);
	dst = cvCreateImage(size, dst->depth, dst->nChannels);
	cvResize(dst, src, CV_INTER_LINEAR);
	cvEqualizeHist(src, src);
	return 1;
}

/*获取图像识别特征*/
void CMyPhoto::getfeature(Mat& src)
{
	Mat yuantu;
	resize(src, yuantu, cvSize(_Width,_Height));
	Mat gray;

	cvtColor(yuantu, gray, CV_RGB2GRAY);
	equalizeHist(gray, gray);

	float mask[3][3] = { { 1, 2, 1 }, { 0, 0, 0 }, { -1, -2, -1 } };

	Mat y_mask = Mat(3, 3, CV_32F, mask) / 8;
	Mat x_mask = y_mask.t(); // 转置
	Mat sobelX, sobelY;

	filter2D(gray, sobelX, CV_32F, x_mask);
	filter2D(gray, sobelY, CV_32F, y_mask);

	sobelX = abs(sobelX);
	sobelY = abs(sobelY);

	float totleValueX = sumMatValue(sobelX);
	float totleValueY = sumMatValue(sobelY);
	//    imwrite("myx.jpg",sobelX);
	//    imwrite("myy.jpg",sobelY);

	feature.clear();

	for (int i = 0; i < gray.rows; i = i + 4)
	{
		for (int j = 0; j < gray.cols; j = j + 4)
		{
			Mat subImageX = sobelX(Rect(j, i, 4, 4));
			//						cout<<endl<<sumMatValue(subImageX) / totleValueX<<" ";
			feature.push_back(sumMatValue(subImageX) / totleValueX);
			Mat subImageY = sobelY(Rect(j, i, 4, 4));
			//						cout<<" "<<sumMatValue(subImageY) / totleValueY<<endl;
			feature.push_back(sumMatValue(subImageY) / totleValueY);
		}
	}
	//	nine=nine/8.0;
	//	feature.push_back( nine );
	//	float ten=sumLine(gray,10);
	//	float eleven=sumLine(gray,20);
	//	float tw=sumLine(gray,30);

	//	float thr=sumcol(gray,7);
	//	float fort=sumcol(gray,10);
	//	float sad=sumcol(gray,13);
	//	feature.push_back( ten );
	//	feature.push_back( eleven );
	//	feature.push_back( tw );
	//
	//	feature.push_back( thr );
	//	feature.push_back( fort );
	//	feature.push_back( sad );
	//
	resize(gray, gray, cvSize(4, 8));

	//  adaptiveThreshold(gray, gray, 255, CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY_INV, 3, 0);
	//	imwrite("asd.jpg",gray);
	//	cout<<endl;
	for (int i = 0; i < gray.rows; i++)
	{
		uchar* data = gray.ptr<uchar>(i);
		for (int j = 0; j < gray.cols; j++)
		{

			float b = data[j];

			b = b / 255.0;
			//			cout<< b<<" ";
			feature.push_back(b);

		}
		//		cout<<endl;
	}

	//        imwrite("ss.jpg",gray);
	//	    system("pause");
}

/*获取 Mat 灰度像素值之和 获取图像特征的手段*/
float CMyPhoto::sumMatValue(const Mat& image)
{
	float sumValue = 0;
	int r = image.rows;
	int c = image.cols;
	if (image.isContinuous())
	{
		c = r*c;
		r = 1;
	}
	for (int i = 0; i < r; i++)
	{
		const uchar* linePtr = image.ptr<uchar>(i);
		for (int j = 0; j < c; j++)
		{
			sumValue += linePtr[j];
		}
	}
	return sumValue;
}

/*BP 识别图片*/
int CMyPhoto::bppredict(IplImage* src)
{
	Mat mat_src( src,1);
	CvANN_MLP bp;
	bp.load("43is样本50.xml");
	/*Mat my = imread("charSamples\\5\\5 \(10\).png");*/
	feature.clear();
	getfeature(mat_src);
	float test[valuesize];
	for (int i = 0; i < valuesize; i++)
	{
		test[i] = feature.at(i);
	}

	Mat testMat(1, valuesize, CV_32FC1, test);
	Mat output;
	bp.predict(testMat, output);
	float mys[6];
	for (int i = 0; i < 6; i++)
	{
		mys[i] = output.at<float>(i);
		if (mys[i]>0.8)
			mys[i] = 1;
		else
			mys[i] = 0;
	}
	int result = mys[5] * 1 + mys[4] * 2 + mys[3] * 4 + mys[2] * 8 + mys[1] * 16 + mys[0] * 32;
	return result;

}

/*根据 BP识别结果反馈字符*/
char CMyPhoto::recognition(int result)
{
	char num;
	switch (result)
	{
	case 1:case 2:case 3:case 4:case 5:case 6:case 7:case 8:case 9:case 0:
			num = result+48;
			break;
	case 10:case 11:case 12:case 13: case 14: case 15: case 16: case 17:
			num = result + 55;
			break;
	case 18:case 19: case 20: case 21: case 22:
			num = result + 56;
			break;
	case 23:case 24:case 25:case 26:case 27:case 28:case 29:case 30:case 31:case 32:case 33:
			num = result + 57;
			break;
	default:
			num = '*';
		break;
	}
	return num;

}

char* CMyPhoto::returnNumber()
{
	return number;
}

char* CMyPhoto::returnTesseract()
{
	return number_tesseract;
}