// TeeChartDemoDlg.h : 头文件
//

#pragma once
#include "tchart1.h"
#include "CSeries.h"
#include "afxwin.h"
#include "VLCWrapper.h"
#include <vector>

// CTeeChartDemoDlg 对话框
class CTeeChartDemoDlg : public CDialog
{
// 构造
public:
	CTeeChartDemoDlg(CWnd* pParent = NULL);	// 标准构造函数
	void DrawLine(double *pX, double *pY, long nNum);
	void AutoStop();

// 对话框数据
	enum { IDD = IDD_TEECHARTDEMO_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

// 实现
protected:
	HICON m_hIcon;
	int m_time;
	int m_testIndex;
	VLCWrapper m_vlcPlayer1;
	VLCWrapper m_vlcPlayer2;
	bool m_playFlag;
	bool m_pauseFlag;
	bool created_;
	CSize currentDlgSize_;
	CStatic m_vlcControl1;
	CStatic m_vlcControl2;
	CFont m_textFont;
	std::vector<float> rate1;
	std::vector<float> rate2;
	std::vector<float> psnr1;
	std::vector<float> psnr2;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:	
	afx_msg void OnTimer(UINT_PTR nIDEvent);	
	afx_msg void OnBnClickedButtonPlay();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	// 选择测试视频
	CComboBox m_slectTest;
	CTchart1 m_myChart;
	CTchart1 m_myChart2;
	CSeries m_series1;
	CSeries m_series2;
	afx_msg void OnCbnSelchangeComboTest();
};
