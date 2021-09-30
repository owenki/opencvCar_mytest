
// myCameraDlg.h : ͷ�ļ�
//
#include <cv.h>

#pragma once



// CmyCameraDlg �Ի���
class CmyCameraDlg : public CDialogEx
{
// ����
public:
	CmyCameraDlg(CWnd* pParent = NULL);	// ��׼���캯��
	
// �Ի�������
	enum { IDD = IDD_MYCAMERA_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;
	IplImage* photo;

	CEdit msgBox;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	
public:
	afx_msg void OnBnClickedloadfile();
	afx_msg void OnBnClickedstart();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedend();
	afx_msg void OnBnClickedpause();
	afx_msg void OnBnClickedrun();
	void setMessageBox(CString msg);//
	void drawImageToChepai(IplImage* src);//

	afx_msg void OnBnClickedAuto();
	afx_msg void OnBnClickedreset();
};
