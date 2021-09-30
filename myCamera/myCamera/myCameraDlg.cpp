
// myCameraDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "myCamera.h"
#include "myCameraDlg.h"
#include "afxdialogex.h"
#include "cv.h"
#include "highgui.h"
#include "CvvImage.h"
#include "MyPhoto.h"
#include <iostream>
#include <fstream>
using namespace cv;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
IplImage* old;
IplImage* xin;
CvCapture* capture;
CRect rect;
CDC *pDC;
HDC hDC;
CWnd *pwnd;
CString myMSG = _T("欢迎来到车牌识别系统");
UINT n = 1;

int kaiguan = 0;
// CmyCameraDlg 对话框


CmyCameraDlg::CmyCameraDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CmyCameraDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON1);
}

void CmyCameraDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CmyCameraDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_loadfile, &CmyCameraDlg::OnBnClickedloadfile)
	ON_BN_CLICKED(IDC_start, &CmyCameraDlg::OnBnClickedstart)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_end, &CmyCameraDlg::OnBnClickedend)
	ON_BN_CLICKED(IDC_pause, &CmyCameraDlg::OnBnClickedpause)
	ON_BN_CLICKED(IDC_run, &CmyCameraDlg::OnBnClickedrun)
	ON_BN_CLICKED(IDC_AUTO, &CmyCameraDlg::OnBnClickedAuto)
	ON_BN_CLICKED(IDC_reset, &CmyCameraDlg::OnBnClickedreset)
END_MESSAGE_MAP()


// CmyCameraDlg 消息处理程序

BOOL CmyCameraDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:  在此添加额外的初始化代码
	CEdit* pedit = (CEdit*)GetDlgItem(IDC_message);
	pedit->LineScroll(pedit->GetLineCount());
	pedit->SetSel(-1, -1);
	pedit->ReplaceSel(myMSG);

	CString data1, time1;
	SYSTEMTIME sti;
	GetLocalTime(&sti);
	data1.Format(_T("%4d-%.2d-%.2d"), sti.wYear, sti.wMonth, sti.wDay);
	time1.Format(_T("%.2d:%.2d:%.2d"), sti.wHour, sti.wMinute, sti.wSecond);
	GetDlgItem(IDC_riqi)->SetWindowTextW(data1);
	GetDlgItem(IDC_shijian)->SetWindowTextW(time1);
	SetTimer(2, 1000, NULL);


	GetDlgItem(IDC_end)->EnableWindow(FALSE);
	GetDlgItem(IDC_pause)->EnableWindow(FALSE);
	GetDlgItem(IDC_run)->EnableWindow(FALSE);
	GetDlgItem(IDC_AUTO)->EnableWindow(FALSE);
	GetDlgItem(IDC_reset)->EnableWindow(FALSE);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CmyCameraDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CmyCameraDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CmyCameraDlg::OnBnClickedloadfile()
{
	// TODO:  在此添加控件通知处理程序代码
	setMessageBox(_T("准备加载图片"));
	BOOL isOpen = TRUE;     //是否打开(否则为保存)  
	CString defaultDir = L"D:\\";   //默认打开的文件路径  
	CString fileName = L"";         //默认打开的文件名  
	CString filter = L"文件 (*.JPG)|*.JPG|位图文件 （*.bmp)|*.bmp|所有文件 （*.*）|*.*||";   //文件过虑的类型  
	CFileDialog openFileDlg(isOpen, defaultDir, fileName, OFN_READONLY, filter, NULL);
	INT_PTR result = openFileDlg.DoModal();
	setMessageBox(_T("请耐心等待……"));
	CString filePath = defaultDir + "0test.jpg";
	if (result == IDOK) {
		filePath = openFileDlg.GetPathName();
	}
	CWnd::SetDlgItemTextW(IDC_fileaddress, filePath);
	/*filePath.Replace(_T("\\"),_T("\\\\"));*/
	setMessageBox(_T("已选择图片：")+filePath);
	pwnd = GetDlgItem(IDC_image);
	pDC = pwnd->GetDC();
	hDC = pDC->GetSafeHdc();
	pwnd->GetClientRect(&rect);

	IplImage* yuantu;
	yuantu = cvLoadImage((CT2CA)filePath, 1);//ct2ca    cw2aex的Unicode入口  宽字符变窄字符解决汉子出现乱码问题
	photo = cvCreateImage(cvSize(yuantu->width, yuantu->height), yuantu->depth, yuantu->nChannels);
	cvCopy(yuantu, photo);
	/*yuantu = cvLoadImage("D:\\0test.jpg", 1);*/
	CvvImage m_CvvImage;
	m_CvvImage.CopyOf(photo, 3);
	m_CvvImage.DrawToHDC(hDC, &rect);
	ReleaseDC(pDC);
	setMessageBox(_T("图片加载完成"));
	setMessageBox(_T("可进行车牌识别"));
	GetDlgItem(IDC_run)->EnableWindow(TRUE);
	cvReleaseImage(&yuantu);
	
}


void CmyCameraDlg::OnBnClickedstart()
{
	// TODO:  在此添加控件通知处理程序代码
	setMessageBox(_T("初始化摄像头"));
	pwnd = GetDlgItem(IDC_camera);
	pDC = pwnd->GetDC();
	hDC = pDC->GetSafeHdc();
	pwnd->GetClientRect(&rect);

	if (!capture)
	{
		capture = cvCaptureFromCAM(0);
		setMessageBox(_T("摄像头初始化完成"));
	}

	if (!capture)
	{
		AfxMessageBox(_T("无法打开摄像头"));
		setMessageBox(_T("摄像头初始化失败！！！"));
		setMessageBox(_T("==.请确认摄像头正常工作"));
		setMessageBox(_T("==.请检查摄像头连接是否正常"));
		return ;
	}

	// 测试  
	IplImage* m_Frame;
	setMessageBox(_T("准备加载摄像头画面"));
	m_Frame = cvQueryFrame(capture);
	CvvImage m_CvvImage;
	m_CvvImage.CopyOf(m_Frame, 1);
	if (true)
	{
		m_CvvImage.DrawToHDC(hDC, &rect);
		//cvWaitKey(10);  
	}
	setMessageBox(_T("摄像头开始工作"));
	// 设置计时器,每10ms触发一次事件  
	SetTimer(1, 10, NULL);

	GetDlgItem(IDC_end)->EnableWindow(TRUE);
	GetDlgItem(IDC_pause)->EnableWindow(TRUE);
	GetDlgItem(IDC_AUTO)->EnableWindow(TRUE);
	GetDlgItem(IDC_start)->EnableWindow(FALSE);

	GetDlgItem(IDC_loadfile)->EnableWindow(FALSE);

	GetDlgItem(IDOK)->EnableWindow(FALSE);
}


void CmyCameraDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值

	if (nIDEvent == 1){
		pwnd = GetDlgItem(IDC_camera);
		//pwnd->MoveWindow(35,30,352,288);  
		pDC = pwnd->GetDC();
		//pDC =GetDC();  
		hDC = pDC->GetSafeHdc();
		pwnd->GetClientRect(&rect);
		IplImage* m_Frame;
		m_Frame = cvQueryFrame(capture);
		if (old == NULL)
		{
			old = cvCreateImage(cvGetSize(m_Frame), m_Frame->depth, m_Frame->nChannels);
			cvCopy(m_Frame, old);
		}
		else if (old!=NULL&&xin==NULL)
		{

			xin = cvCreateImage(cvGetSize(m_Frame), m_Frame->depth, m_Frame->nChannels);
			cvCopy(m_Frame, xin);
		}
		else
		{
			old = cvCreateImage(cvGetSize(m_Frame), m_Frame->depth, m_Frame->nChannels);
			cvCopy(xin, old);
			xin = cvCreateImage(cvGetSize(m_Frame), m_Frame->depth, m_Frame->nChannels);
			cvCopy(m_Frame, xin);
			
		}
		CvvImage m_CvvImage;
		m_CvvImage.CopyOf(m_Frame, 1);
		m_CvvImage.DrawToHDC(hDC, &rect);
	}
	else if (nIDEvent == 2){
		CString data1, time1;
		SYSTEMTIME sti;
		GetLocalTime(&sti);
		data1.Format(_T("%4d-%.2d-%.2d"), sti.wYear, sti.wMonth, sti.wDay);
		time1.Format(_T("%.2d:%.2d:%.2d"), sti.wHour, sti.wMinute, sti.wSecond);
		GetDlgItem(IDC_riqi)->SetWindowTextW(data1);
		GetDlgItem(IDC_shijian)->SetWindowTextW(time1);
	}
	CDialogEx::OnTimer(nIDEvent);
}


void CmyCameraDlg::OnBnClickedend()
{
	// TODO:  在此添加控件通知处理程序代码
	setMessageBox(_T("准备关闭摄像头"));
	cvReleaseCapture(&capture);
	KillTimer(1);
	setMessageBox(_T("摄像头关闭完成"));
	GetDlgItem(IDC_start)->EnableWindow(TRUE);
	GetDlgItem(IDC_loadfile)->EnableWindow(TRUE);
	GetDlgItem(IDC_pause)->EnableWindow(FALSE);
	GetDlgItem(IDC_end)->EnableWindow(FALSE);
	GetDlgItem(IDC_AUTO)->EnableWindow(FALSE);
	GetDlgItem(IDOK)->EnableWindow(TRUE);
}


void CmyCameraDlg::OnBnClickedpause()
{
	// TODO:  在此添加控件通知处理程序代码
	IplImage* m_Frame;
	m_Frame = cvQueryFrame(capture);
	photo = cvCreateImage(cvSize(m_Frame->width, m_Frame->height), m_Frame->depth, m_Frame->nChannels);
	cvCopy(m_Frame, photo);

	pwnd = GetDlgItem(IDC_image);
	pDC = pwnd->GetDC();
	hDC = pDC->GetSafeHdc();
	pwnd->GetClientRect(&rect);
	CvvImage m_CvvImage;
	m_CvvImage.CopyOf(m_Frame, 1);
	if (true)
	{
		m_CvvImage.DrawToHDC(hDC, &rect);
	}
	setMessageBox(_T("截图成功"));
	GetDlgItem(IDC_run)->EnableWindow(TRUE);
}


void CmyCameraDlg::OnBnClickedrun()
{
	// TODO:  在此添加控件通知处理程序代码
	try{
		GetDlgItem(IDC_loadfile)->EnableWindow(FALSE);
		GetDlgItem(IDC_pause)->EnableWindow(FALSE);
		setMessageBox(_T("………马上开始"));
		CString data1, time1;
		SYSTEMTIME sti;
		GetLocalTime(&sti);
		data1.Format(_T("%4d%.2d%.2d"), sti.wYear, sti.wMonth, sti.wDay);
		time1.Format(_T("%.2d%.2d%.2d"), sti.wHour, sti.wMinute, sti.wSecond);
		CString localaddress = _T("newPhoto\\");
		CreateDirectory(localaddress, NULL);
		localaddress = localaddress + data1 + time1 + _T(".jpg");
		setMessageBox(_T("图片以保存至 ") + localaddress);
		cvSaveImage((CT2CA)localaddress, photo);

	//	/*cvShowImage("src", photo);*/
	//	IplImage* sobel = cvCreateImage(cvGetSize(photo), photo->depth, 1);//灰度图片  单通道
	//	IplImage* gray = cvCreateImage(cvGetSize(photo), photo->depth, 1);//灰度图片  单通道
	//	IplImage* src2 = cvCreateImage(cvGetSize(photo), photo->depth, 3);//灰度图片  单通道
	//	cvCopy(photo, src2);
	///*	cvShowImage("src2", src2);*/
	//	cvCvtColor(src2, gray, CV_RGB2GRAY);
	//	cvSobel(gray, sobel, 2, 0, 3);
	//	//cvShowImage("sebel", sobel);
	//	cvThreshold(gray, gray, 130, 255, CV_THRESH_BINARY);
	//	cvShowImage("erzhi", gray);
		setMessageBox(_T("开始图像处理--------初始化"));
		
		CMyPhoto phototest(photo);
		phototest.chushihua();
		
		phototest.dingwei();
		phototest.shaixuan();
		CString num;
		num.Format(_T("%d"), phototest.chepaiquyu());
		setMessageBox(_T("_") + num);
		IplImage* src = NULL;
		src = phototest.returnRecord();
		drawImageToChepai(src); 


	/*	
		CString ss1(phototest.number);
		setMessageBox(ss1);

		CString tt1(phototest.number_tesseract);
		setMessageBox(tt1);
		*/
	/*	CString ss(phototest.returnNumber());
		setMessageBox(ss);*/

		CString tt(phototest.returnTesseract());
		setMessageBox(tt);

	

	
	//	Mat m=imread("thr3_字符3.jpg");
	///*	imshow("asd",m);
	//	phototest.bppredict(m);*/


		//CString num;
		//int n = phototest.bppredict(m);
		//char s=	phototest.recognition(n);
		//num.Format(_T("%d  %c"),n,s);
		//setMessageBox(_T("_") + num);
	
	/*	phototest.chushihua();
		setMessageBox(_T("图像处理--------降噪"));
		cvReleaseImage(&photo);
		phototest.getBlueArea();
		setMessageBox(_T("图像处理--------倾斜矫正"));
		phototest.qingxiejiaozheng();
		setMessageBox(_T("图像处理--------二值化"));
		phototest.erzhihua();
		setMessageBox(_T("图像处理--------请耐心等待"));
		setMessageBox(_T("图像处理--------定位"));
		phototest.dingwei();
		IplImage* src = NULL;
		src = phototest.returnimage();
		drawImageToChepai(src);*/
		//phototest.myway();
		setMessageBox(_T("图像处理完成"));
	}
	catch (int e){
		switch (e)
		{
		case -1:
			setMessageBox(_T("无原图"));
			break;
		default:
			break;
		}
		setMessageBox(_T("图像处理失败"));
	}

	GetDlgItem(IDC_loadfile)->EnableWindow(TRUE);
	CButton* button = (CButton*)GetDlgItem(IDC_end);
	if (button->IsWindowEnabled()==TRUE)
		GetDlgItem(IDC_pause)->EnableWindow(TRUE);
	GetDlgItem(IDC_run)->EnableWindow(FALSE);
	
}

void CmyCameraDlg::setMessageBox( CString msg)
{
	CString num;
	num.Format(_T("%3d"), n);

	CEdit* pedit = (CEdit*)GetDlgItem(IDC_message);

	pedit->LineScroll(pedit->GetLineCount());
	pedit->SetSel(-1, -1);
	pedit->ReplaceSel(_T("\r\n") + num + _T("->") + msg);
	n = n + 1;
	
	
	//MsgBox自动显示最下端
	
	

	
}

void CmyCameraDlg::drawImageToChepai(IplImage* src)
{
	pwnd = GetDlgItem(IDC_chepaiarea);
	pDC = pwnd->GetDC();
	hDC = pDC->GetSafeHdc();
	pwnd->GetClientRect(&rect);
	CvvImage m_CvvImage;
	m_CvvImage.CopyOf(src, 3);
	m_CvvImage.DrawToHDC(hDC, &rect);
	ReleaseDC(pDC);
}
void __cdecl beginThreadFunc(LPVOID  res){
	
	

	while (old != NULL&&xin != NULL)
	{
		IplImage *result = cvCreateImage(cvGetSize(old), IPL_DEPTH_8U, 3);
		cvAbsDiff(old, xin, result);
		CMyPhoto phototest(result);
		phototest.chushihua();

		phototest.dingwei();
		phototest.shaixuan();
	
		
		IplImage* src = NULL;
		src = phototest.returnRecord();
		res = phototest.returnTesseract();
		
		if (kaiguan != 0)
			break;
		/*	 cvReleaseImage(&old);
		cvReleaseImage(&xin);*/
		cvReleaseImage(&result);

	}
}
void CmyCameraDlg::OnBnClickedAuto()
{
	// TODO:  在此添加控件通知处理程序代码
	kaiguan = 0;
	setMessageBox(_T("自动化处理开始"));
	CString res;
	GetDlgItem(IDC_reset)->EnableWindow(TRUE);
	GetDlgItem(IDC_end)->EnableWindow(FALSE);
	GetDlgItem(IDC_pause)->EnableWindow(FALSE);
	_beginthread(beginThreadFunc, 0, &res);
	GetDlgItem(IDC_AUTO)->EnableWindow(FALSE);
	
}

void CmyCameraDlg::OnBnClickedreset()
{
	// TODO:  在此添加控件通知处理程序代码
	kaiguan = 1;
	GetDlgItem(IDC_reset)->EnableWindow(FALSE);
	setMessageBox(_T("自动化处理结束"));
	GetDlgItem(IDC_AUTO)->EnableWindow(TRUE);
	GetDlgItem(IDC_end)->EnableWindow(TRUE);
	GetDlgItem(IDC_pause)->EnableWindow(TRUE);
}
