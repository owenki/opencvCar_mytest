
// myCameraDlg.h : 头文件
//
#include <cv.h>

#pragma once



// CmyCameraDlg 对话框
class CmyCameraDlg : public CDialogEx
{
// 构造
public:
	CmyCameraDlg(CWnd* pParent = NULL);	// 标准构造函数
	
// 对话框数据
	enum { IDD = IDD_MYCAMERA_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;
	IplImage* photo;

	CEdit msgBox;

	// 生成的消息映射函数
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
