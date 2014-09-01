// TeeChartDemoDlg.h : ͷ�ļ�
//

#pragma once
#include "tchart1.h"
#include "CSeries.h"
#include "afxwin.h"
#include "VLCWrapper.h"
#include <vector>

// CTeeChartDemoDlg �Ի���
class CTeeChartDemoDlg : public CDialog
{
// ����
public:
	CTeeChartDemoDlg(CWnd* pParent = NULL);	// ��׼���캯��
	void DrawLine(double *pX, double *pY, long nNum);
	void AutoStop();

// �Ի�������
	enum { IDD = IDD_TEECHARTDEMO_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

// ʵ��
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

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:	
	afx_msg void OnTimer(UINT_PTR nIDEvent);	
	afx_msg void OnBnClickedButtonPlay();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	// ѡ�������Ƶ
	CComboBox m_slectTest;
	CTchart1 m_myChart;
	CTchart1 m_myChart2;
	CSeries m_series1;
	CSeries m_series2;
	afx_msg void OnCbnSelchangeComboTest();
};
