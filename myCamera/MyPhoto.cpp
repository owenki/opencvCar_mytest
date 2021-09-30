#include "stdafx.h"
#include "MyPhoto.h"
#include <vector>
using namespace std;


CMyPhoto::~CMyPhoto()
{
}

CMyPhoto::CMyPhoto(IplImage* src)
{
	yuantu = NULL;
	gray = NULL;
	erzhi = NULL;
	daifengge = NULL;
	statue = 1;
	jishu = 0;
	myresize(src);
	yuantu = cvCreateImage(cvSize(src->width, src->height), src->depth, src->nChannels);
	cvCopy(src, yuantu);
	
}
int CMyPhoto::chushihua()
{
	blue = cvCreateImage(cvSize(yuantu->width, yuantu->height), yuantu->depth, 3);
	cvCvtColor(yuantu, blue, CV_RGB2HSV);
	
	return 1;
}

int CMyPhoto::erzhihua()
{
	if (isnull(1) )
	{
		/*liantong(gray, 1);*/
		cvSetImageROI(yuantu, chepai_blue);

		gray = cvCreateImage(cvSize(yuantu->roi->width, yuantu->roi->height), yuantu->depth, 1);
		cvCvtColor(yuantu, gray, CV_RGB2GRAY);
		erzhi = cvCreateImage(cvSize(yuantu->roi->width, yuantu->roi->height), yuantu->depth, 1);

		
		mycanny(gray, erzhi);
		cvReleaseImage(&gray);
		cvSmooth(erzhi, erzhi);
		gray = cvCreateImage(cvSize(yuantu->roi->width, yuantu->roi->height), yuantu->depth, 3);
		cvCopyImage(yuantu, gray);
		cvResetImageROI(yuantu);
		return 1;
	}
	else
		return -21;
}

int CMyPhoto::isnull(int src)
{
	switch (src)
	{
	case 1:
		if (yuantu == NULL) return -1;
	case 2:
		if (gray == NULL) return -2;
	case 3:
		if (erzhi == NULL) return -3;
	case 4:
		if (daifengge == NULL) return -4;
	case 5:
		if (blue == NULL) return -5;
	default:
		break;
	}

	return 1;
}

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
	if (temp == t)
		return temp;
	else
		my_canny_low(src, temp);

}

int CMyPhoto::myresize(IplImage* src)
{
	if (src == NULL)
		return -11;
	int max_size=0;
	src->width > src->height ? max_size = src->width : max_size = src->height;

	while (max_size > 3000||src->height>1000){
		IplImage* temp;
		temp = cvCreateImage(cvSize(src->width, src->height), src->depth, src->nChannels);
		cvCopy(src, temp);
		cvReleaseImage(&src);
		src = cvCreateImage(cvSize(temp->width *0.8, temp->height *0.8), temp->depth, temp->nChannels);
		cvResize(temp, src, CV_INTER_AREA);
		cvReleaseImage(&temp);
		src->width > src->height ? max_size = src->width : max_size = src->height;
	}
	return 1;

}

int CMyPhoto::liantong(IplImage* src, int n)
{
	//cvErode(src, src);//细化
	IplConvKernel *a = cvCreateStructuringElementEx(3, 1, 0, 0, CV_SHAPE_RECT);//3x1填充算子
	for (int i = 0; i<n; i++){
		cvErode(src, src,a);//细化
		cvDilate(src, src, a);
	}
	return 1;
}

int CMyPhoto::getBlueArea()
{
	int i = 0, j = 0;
	IplImage* h;
	IplImage* s;
	IplImage* v;
	IplImage* img_test;
	h = cvCreateImage(cvSize(blue->width, blue->height), blue->depth, 1);
	s = cvCreateImage(cvSize(blue->width, blue->height), blue->depth, 1);
	v = cvCreateImage(cvSize(blue->width, blue->height), blue->depth, 1);
	img_test = cvCreateImage(cvSize(blue->width, blue->height), blue->depth, 1);
	cvSplit(blue, h, s, v, 0);
	for (i = 0; i < h->height; i++){
		for (j = 0; j < h->width; j++){
			if (CV_IMAGE_ELEM(h, uchar, i, j) >= 0 * _H&&CV_IMAGE_ELEM(h, uchar, i, j) <= 30 * _H){
				CV_IMAGE_ELEM(h, uchar, i, j) = 255;
			}
			else
				CV_IMAGE_ELEM(h, uchar, i, j) = 0;
		}
	}
	for (i = 0; i < s->height; i++){
		for (j = 0; j < s->width; j++){
			if (CV_IMAGE_ELEM(s, uchar, i, j) >= 0.16*_S&&CV_IMAGE_ELEM(s, uchar, i, j) <= 1.0*_S){

				CV_IMAGE_ELEM(s, uchar, i, j) = 255;
			}
			else
				CV_IMAGE_ELEM(s, uchar, i, j) = 0;
		}
	}
	for (i = 0; i < v->height; i++){
		for (j = 0; j < v->width; j++){
			if (CV_IMAGE_ELEM(v, uchar, i, j) >= 0.18*_V&&CV_IMAGE_ELEM(v, uchar, i, j) <= 1.0*_V){

				CV_IMAGE_ELEM(v, uchar, i, j) = 255;
			}
			else
				CV_IMAGE_ELEM(v, uchar, i, j) = 0;
		}
	}

	cvAnd(h, s, img_test);
	cvAnd(img_test, v, img_test);

	cvReleaseImage(&h);
	cvReleaseImage(&s);
	cvReleaseImage(&v);

	//cvShowImage("s_t", img_test);
	/*cvShowImage("pro", img_test);*/
	cvSmooth(img_test, img_test);//先进行高斯平滑处理
	//cvShowImage("aft", img_test);
	liantong(img_test, 2);//后进行形态学连通  有利于减小车牌区域定位的误差
	
	/*过滤小面积的蓝色 噪点*/
	CvSeq *contour=NULL;

	double minarea = _MAX_area;
	double tmparea = 0.0;

	
	CvMemStorage *storage = cvCreateMemStorage();
	CvScalar color = cvScalar(255, 0, 0);// 
	CvContourScanner scanner = NULL;
	scanner= cvStartFindContours(img_test, storage, sizeof(CvContour), CV_RETR_CCOMP, CV_CHAIN_APPROX_NONE, cvPoint(0, 0));
	//开始遍历轮廓树   
	CvRect rect;
	while (contour = cvFindNextContour(scanner))
	{
		tmparea = cvContourArea(contour);
		rect = cvBoundingRect(contour, 0);
		/*cvRectangleR(yuantu, rect, cvScalar(0,255,0));*/
		if (tmparea < minarea)
		{
			for (int y = rect.y; y < rect.y + rect.height; y++)
			{
				for (int x = rect.x; x < rect.x + rect.width; x++)
				{
					CV_IMAGE_ELEM(img_test, uchar, y, x) = 0;

				}
			}

		}
		else
		{
			continue;
		}
		
	}
	int a = 0;
	//cvShowImage("yuantu", yuantu);
	//cvShowImage("s", img_test);

	//cvClearMemStorage(storage);//清空   重置 memstorage
	//cvRestoreMemStoragePos(storage, 0);
	/*cvReleaseMemStorage(&storage);*/
	/*cvReleaseMemStorage(&storage);*/
	/*storage = NULL;*/
	int x = img_test->width, y = img_test->height, width = 0, height = 0;

	for (i = 1; i < img_test->height; i++){
		for (j = 1; j < img_test->width; j++){
			if (CV_IMAGE_ELEM(img_test, uchar, i, j) != 0 && j<x){
				x = j;
			}
			if (CV_IMAGE_ELEM(img_test, uchar, i, j) != 0 && i<y){
				y = i;
			}
			if (CV_IMAGE_ELEM(img_test, uchar, i, j) != 0 && j>width){
				width = j;
			}
			if (CV_IMAGE_ELEM(img_test, uchar, i, j) != 0 && i>height){
				height = i;
			}
		}
	}

	cvReleaseImage(&img_test);
	cvReleaseImage(&blue);
	/*CString S;
	S.Format(_T("x:%d wid:%d \n y:%d hei:%d"), x, width, y, height);
	HWND pwnd=FindWindow(_T(""),NULL);
	MessageBox(pwnd,S,_T("tile"),IDOK);*/

	if (width - _RY <= x || y >= height - _RY){
		chepai_blue = cvRect(0,0,200,200);
		/*cvRectangleR(yuantu, chepai_blue, cvScalar(255, 0, 0)); */
		/*cvShowImage("ERROR", img_test);*/
		statue = 100;
		throw  -100;
		return -23;
	}
	else{
		chepai_blue = cvRect(x, y, width - x, height - y);
		//添加图片蓝色区域冗余
		if (x > _RY)
			chepai_blue.x = x - _RY;
		else
			chepai_blue.x = 0;

		if (y > _RY)
			chepai_blue.y = y - _RY;
		else
			chepai_blue.y = 0;

		if (width < yuantu->width - _RY)
			chepai_blue.width = width-x + _RY;
		else
			chepai_blue.width = yuantu->width;

		if (height < yuantu->height - _RY)
			height = height-y + _RY;
		else
			height = yuantu->height;

		

		/*cvRectangleR(yuantu, chepai_blue, cvScalar(255, 0, 0));*/

		return 1;
	}
	
}

int CMyPhoto::dingwei()
{
	if (isnull(3)){
		if (statue == 100)
			return -100;//蓝色区域定位未发现车牌
		/*cvShowImage("er", erzhi);*/

		int i = 0, j = 0, k = 0, n = 0, m = 0;
		int max_height = erzhi->height;
		vector<int> v_line_v(max_height,0);

		IplImage* shui_temp = cvCreateImage(cvSize(erzhi->width,erzhi->height), erzhi->depth, erzhi->nChannels);
		cvCopyImage(erzhi, shui_temp);

		////举行检测
		//IplImage* dst = NULL;

		//CvMemStorage* storage = cvCreateMemStorage(0);
		//CvMemStorage* storage1 = cvCreateMemStorage(0);
		//CvSeq* contour = 0;
		//CvSeq* cont;
		//CvSeq* mcont;


		//dst = cvCreateImage(cvGetSize(erzhi), 8, 3);
		//liantong(erzhi, 3);
		//cvSmooth(erzhi, erzhi);
		//cvShowImage("er", erzhi);
		//cvFindContours(erzhi, storage, &contour, sizeof(CvContour), CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);
		//
		//if (contour)
		//{
		//	CvTreeNodeIterator iterator;
		//	cvInitTreeNodeIterator(&iterator, contour, 1);
		//	while (0 != (cont = (CvSeq*)cvNextTreeNode(&iterator)))
		//	{
		//		mcont = cvApproxPoly(cont, sizeof(CvContour), storage1, CV_POLY_APPROX_DP, cvContourPerimeter(cont)*0.02, 0);
		//		if (mcont->total >= 4 && mcont->total <= 6)
		//			cvDrawContours(dst, mcont, CV_RGB(255, 0, 0), CV_RGB(0, 0, 100), 0, 1, 8, cvPoint(0, 0));
		//	}
		//}
		//cvShowImage("Contour", dst);
		//cvReleaseMemStorage(&storage);
	
		//进行水平扫描 并获得最大值
		for (i = 0; i<shui_temp->height; i++){
			for (j = 0; j<shui_temp->width; j++){
				CV_IMAGE_ELEM(shui_temp, uchar, i, j) = CV_IMAGE_ELEM(shui_temp, uchar, i, j + 1) - CV_IMAGE_ELEM(shui_temp, uchar, i, j);
				v_line_v.at(i) += CV_IMAGE_ELEM(shui_temp, uchar, i, j) / 50;
			}
		}

		vector<int> y;
		y.reserve(_LT);
		vector<int> height;
		height.reserve(_LT);

		int count = 0;//水平区域密度值
		/*水平区域分割*/
		for (int i = 0; i < max_height; i++)
		{
			if (v_line_v.at(i)>_KD){
				for (int j = 1; j < max_height - 1 - i; j++)
				{
					if (chazhibijiao(v_line_v.at(i), v_line_v.at(i+j), _CZ))
					{
						count = count + 1;
					}
					else
					{
						if (chazhibijiao(v_line_v.at(i), v_line_v.at(i+j+1), _CZ))
						{
							count = count + 1;
						}
						else
						{
							break;
						}
					}
				}
				if (count < _GD)
				{
					count = 0;
				}
				else{
					if (jishu == 0)
					{
						y.push_back( i);
						height.push_back(count);
						count = 0;
						jishu = jishu + 1;
					}
					else
					{
						if (i > (y.at(jishu - 1) + height.at(jishu - 1)))
						{
							y.push_back( i);
							height.push_back( count);
							count = 0;
							jishu = jishu + 1;
						}
						else
						{
							if ((i + count)<(y.at(jishu - 1) + height.at(jishu - 1)))
							{
								count = 0;
							}
							else
							{
								height.at(jishu - 1) = i - y.at(jishu - 1) + count;
								count = 0;
							}

						}

					}
				}

			}
			else
			{
				continue;
			}
		}

		vector<int>().swap(v_line_v);
	
		/*水平定位显示*/
		/*cvSetImageROI(yuantu,chepai_blue );
		for (int i = 0; i < jishu; i++)
		{
			cvRectangleR(yuantu, cvRect(i * 5, y.at(i), 300, height.at(i)), cvScalar(20 * i, 10 * i, i));
		}
		cvShowImage("sssa", yuantu);*/
		/*垂直扫描*/
		int max_width = shui_temp->width;
		
		vector<int> x(jishu, 0);
		vector<int> width(jishu, 0);
		int dianhao = 0;
		int max_size = 0;
		/*筛选水平宽度*/
		for (n = 0; n < jishu; n++)
		{
			vector<int> v_col_v(max_width, 0);
			dianhao = _dian*height.at(n);
			count = 0;
			
			for (i = 0; i < shui_temp->width; i++)//扫描垂直点数
			{
				for (j = y.at(n); j < height.at(n)+y.at(n); j++)
				{
					if(CV_IMAGE_ELEM(shui_temp, uchar, j, i) !=0)
						v_col_v.at(i) += 1;
				}
			}
			/*显示运行中图片*/
		/*	if (n == 4){
				cvSetImageROI(shui_temp, cvRect(0, y.at(n), shui_temp->width, height.at(n)));
				cvShowImage("my", shui_temp);
				cvResetImageROI(shui_temp);
			}*/
			max_size = 0;
			int kai = ceil(_kai*height.at(n));
			int xu1 = ceil(_xu*height.at(n));
			int xu = 0;
			int fog = 0;
			for (i = 0; i < shui_temp->width; i++)
			{
				int a = v_col_v.at(i);
			
				if (v_col_v.at(i) >= kai){
					count += 1;
					for (j = 1; j < shui_temp->width - i - 1; j++)
					{
						int c = i + j;
						int b = v_col_v.at(i + j);
						if (v_col_v.at(i + j) > 10)//from !=0 to 20
							count += 1;
						else
						{
							for (m = 0; m < dianhao; m++)
							{
								if (i + j<shui_temp->width - dianhao){
									if (v_col_v.at(i + j + m)>0)
									{
										count += m + 1;
										j += m;
										break;
									}
									else
									{
										if (m == dianhao-1)
											fog = 1;
									}
								}
							}
							
								
						}
						if (fog == 1)
						{
							fog = 0;
							break;
						}

					}
					if (count > max_size)
					{
						max_size = count;
						x.at(n) = i;
						width.at(n) = count;
					}
					count = 0;

				}
				else
				{
					continue;
				}

			}
			vector<int>().swap(v_col_v);	
			
		}
		//删除不合规则区域
		for ( i = 0; i < jishu; i++)
		{
			if (width.at(i)>height.at(i) * 6 || width.at(i) < height.at(i) * 3)
				width.at(i) = 0;
		}
	
		double temp = 0;
		double max_count = -1;
		int max_n = 0;
		/*/连通区域筛选/*/
		for (n = 0; n<jishu; n++)
		{	
			for (i = y.at(n); i<y.at(n)+height.at(n); i++){
				for (j = x.at(n); j<width.at(n)+x.at(n); j++){
					if (CV_IMAGE_ELEM(shui_temp, uchar, i, j) != 0)
						count++;
				}
			}
		
			if (width.at(n)*height.at(n) == 0)
				temp = -1;
			else
			{
				temp = count / ((width.at(n)*height.at(n)*1.0));
			}
			
			if (temp>max_count)
			{
				max_count = temp;
				max_n = n;
				count = 0;
				temp = 0;
			}
			else
			{
				count = 0;
				temp = 0;
			}
		}

		chepai = cvRect(x.at(max_n) + chepai_blue.x, y.at(max_n) + chepai_blue.y, width.at(max_n) + 1, height.at(max_n));
		
		return 1;

	}
	else
		return -24;
	
}

IplImage* CMyPhoto::returnimage()
{
		IplImage* src;
		IplImage* temp;
		src = cvCreateImage(cvSize(330, 90), yuantu->depth, yuantu->nChannels);
		temp = cvCreateImage(cvSize(chepai.width, chepai.height), yuantu->depth, yuantu->nChannels);
		cvSetImageROI(yuantu, chepai);
		/*cvShowImage("lll",yuantu);*/
		cvCopy(yuantu, temp);
		cvResetImageROI(yuantu);
		cvResize(temp, src);
		return src;
	
}

int CMyPhoto::chazhibijiao(int &a, int &b, int chazhi){
	return abs(a - b) > chazhi ? 0 : 1;
}