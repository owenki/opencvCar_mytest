
// myCameraDlg.cpp : ʵ���ļ�
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
CString myMSG = _T("��ӭ��������ʶ��ϵͳ");
UINT n = 1;

int kaiguan = 0;
// CmyCameraDlg �Ի���


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


// CmyCameraDlg ��Ϣ�������

BOOL CmyCameraDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������
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
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CmyCameraDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CmyCameraDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CmyCameraDlg::OnBnClickedloadfile()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	setMessageBox(_T("׼������ͼƬ"));
	BOOL isOpen = TRUE;     //�Ƿ��(����Ϊ����)  
	CString defaultDir = L"D:\\";   //Ĭ�ϴ򿪵��ļ�·��  
	CString fileName = L"";         //Ĭ�ϴ򿪵��ļ���  
	CString filter = L"�ļ� (*.JPG)|*.JPG|λͼ�ļ� ��*.bmp)|*.bmp|�����ļ� ��*.*��|*.*||";   //�ļ����ǵ�����  
	CFileDialog openFileDlg(isOpen, defaultDir, fileName, OFN_READONLY, filter, NULL);
	INT_PTR result = openFileDlg.DoModal();
	setMessageBox(_T("�����ĵȴ�����"));
	CString filePath = defaultDir + "0test.jpg";
	if (result == IDOK) {
		filePath = openFileDlg.GetPathName();
	}
	CWnd::SetDlgItemTextW(IDC_fileaddress, filePath);
	/*filePath.Replace(_T("\\"),_T("\\\\"));*/
	setMessageBox(_T("��ѡ��ͼƬ��")+filePath);
	pwnd = GetDlgItem(IDC_image);
	pDC = pwnd->GetDC();
	hDC = pDC->GetSafeHdc();
	pwnd->GetClientRect(&rect);

	IplImage* yuantu;
	yuantu = cvLoadImage((CT2CA)filePath, 1);//ct2ca    cw2aex��Unicode���  ���ַ���խ�ַ�������ӳ�����������
	photo = cvCreateImage(cvSize(yuantu->width, yuantu->height), yuantu->depth, yuantu->nChannels);
	cvCopy(yuantu, photo);
	/*yuantu = cvLoadImage("D:\\0test.jpg", 1);*/
	CvvImage m_CvvImage;
	m_CvvImage.CopyOf(photo, 3);
	m_CvvImage.DrawToHDC(hDC, &rect);
	ReleaseDC(pDC);
	setMessageBox(_T("ͼƬ�������"));
	setMessageBox(_T("�ɽ��г���ʶ��"));
	GetDlgItem(IDC_run)->EnableWindow(TRUE);
	cvReleaseImage(&yuantu);
	
}


void CmyCameraDlg::OnBnClickedstart()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	setMessageBox(_T("��ʼ������ͷ"));
	pwnd = GetDlgItem(IDC_camera);
	pDC = pwnd->GetDC();
	hDC = pDC->GetSafeHdc();
	pwnd->GetClientRect(&rect);

	if (!capture)
	{
		capture = cvCaptureFromCAM(0);
		setMessageBox(_T("����ͷ��ʼ�����"));
	}

	if (!capture)
	{
		AfxMessageBox(_T("�޷�������ͷ"));
		setMessageBox(_T("����ͷ��ʼ��ʧ�ܣ�����"));
		setMessageBox(_T("==.��ȷ������ͷ��������"));
		setMessageBox(_T("==.��������ͷ�����Ƿ�����"));
		return ;
	}

	// ����  
	IplImage* m_Frame;
	setMessageBox(_T("׼����������ͷ����"));
	m_Frame = cvQueryFrame(capture);
	CvvImage m_CvvImage;
	m_CvvImage.CopyOf(m_Frame, 1);
	if (true)
	{
		m_CvvImage.DrawToHDC(hDC, &rect);
		//cvWaitKey(10);  
	}
	setMessageBox(_T("����ͷ��ʼ����"));
	// ���ü�ʱ��,ÿ10ms����һ���¼�  
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
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ

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
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	setMessageBox(_T("׼���ر�����ͷ"));
	cvReleaseCapture(&capture);
	KillTimer(1);
	setMessageBox(_T("����ͷ�ر����"));
	GetDlgItem(IDC_start)->EnableWindow(TRUE);
	GetDlgItem(IDC_loadfile)->EnableWindow(TRUE);
	GetDlgItem(IDC_pause)->EnableWindow(FALSE);
	GetDlgItem(IDC_end)->EnableWindow(FALSE);
	GetDlgItem(IDC_AUTO)->EnableWindow(FALSE);
	GetDlgItem(IDOK)->EnableWindow(TRUE);
}


void CmyCameraDlg::OnBnClickedpause()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
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
	setMessageBox(_T("��ͼ�ɹ�"));
	GetDlgItem(IDC_run)->EnableWindow(TRUE);
}


void CmyCameraDlg::OnBnClickedrun()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	try{
		GetDlgItem(IDC_loadfile)->EnableWindow(FALSE);
		GetDlgItem(IDC_pause)->EnableWindow(FALSE);
		setMessageBox(_T("���������Ͽ�ʼ"));
		CString data1, time1;
		SYSTEMTIME sti;
		GetLocalTime(&sti);
		data1.Format(_T("%4d%.2d%.2d"), sti.wYear, sti.wMonth, sti.wDay);
		time1.Format(_T("%.2d%.2d%.2d"), sti.wHour, sti.wMinute, sti.wSecond);
		CString localaddress = _T("newPhoto\\");
		CreateDirectory(localaddress, NULL);
		localaddress = localaddress + data1 + time1 + _T(".jpg");
		setMessageBox(_T("ͼƬ�Ա����� ") + localaddress);
		cvSaveImage((CT2CA)localaddress, photo);

	//	/*cvShowImage("src", photo);*/
	//	IplImage* sobel = cvCreateImage(cvGetSize(photo), photo->depth, 1);//�Ҷ�ͼƬ  ��ͨ��
	//	IplImage* gray = cvCreateImage(cvGetSize(photo), photo->depth, 1);//�Ҷ�ͼƬ  ��ͨ��
	//	IplImage* src2 = cvCreateImage(cvGetSize(photo), photo->depth, 3);//�Ҷ�ͼƬ  ��ͨ��
	//	cvCopy(photo, src2);
	///*	cvShowImage("src2", src2);*/
	//	cvCvtColor(src2, gray, CV_RGB2GRAY);
	//	cvSobel(gray, sobel, 2, 0, 3);
	//	//cvShowImage("sebel", sobel);
	//	cvThreshold(gray, gray, 130, 255, CV_THRESH_BINARY);
	//	cvShowImage("erzhi", gray);
		setMessageBox(_T("��ʼͼ����--------��ʼ��"));
		
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

	

	
	//	Mat m=imread("thr3_�ַ�3.jpg");
	///*	imshow("asd",m);
	//	phototest.bppredict(m);*/


		//CString num;
		//int n = phototest.bppredict(m);
		//char s=	phototest.recognition(n);
		//num.Format(_T("%d  %c"),n,s);
		//setMessageBox(_T("_") + num);
	
	/*	phototest.chushihua();
		setMessageBox(_T("ͼ����--------����"));
		cvReleaseImage(&photo);
		phototest.getBlueArea();
		setMessageBox(_T("ͼ����--------��б����"));
		phototest.qingxiejiaozheng();
		setMessageBox(_T("ͼ����--------��ֵ��"));
		phototest.erzhihua();
		setMessageBox(_T("ͼ����--------�����ĵȴ�"));
		setMessageBox(_T("ͼ����--------��λ"));
		phototest.dingwei();
		IplImage* src = NULL;
		src = phototest.returnimage();
		drawImageToChepai(src);*/
		//phototest.myway();
		setMessageBox(_T("ͼ�������"));
	}
	catch (int e){
		switch (e)
		{
		case -1:
			setMessageBox(_T("��ԭͼ"));
			break;
		default:
			break;
		}
		setMessageBox(_T("ͼ����ʧ��"));
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
	
	
	//MsgBox�Զ���ʾ���¶�
	
	

	
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
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	kaiguan = 0;
	setMessageBox(_T("�Զ�������ʼ"));
	CString res;
	GetDlgItem(IDC_reset)->EnableWindow(TRUE);
	GetDlgItem(IDC_end)->EnableWindow(FALSE);
	GetDlgItem(IDC_pause)->EnableWindow(FALSE);
	_beginthread(beginThreadFunc, 0, &res);
	GetDlgItem(IDC_AUTO)->EnableWindow(FALSE);
	
}

void CmyCameraDlg::OnBnClickedreset()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	kaiguan = 1;
	GetDlgItem(IDC_reset)->EnableWindow(FALSE);
	setMessageBox(_T("�Զ����������"));
	GetDlgItem(IDC_AUTO)->EnableWindow(TRUE);
	GetDlgItem(IDC_end)->EnableWindow(TRUE);
	GetDlgItem(IDC_pause)->EnableWindow(TRUE);
}
