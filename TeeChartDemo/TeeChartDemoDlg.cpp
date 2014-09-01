// TeeChartDemoDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "TeeChartDemo.h"
#include "TeeChartDemoDlg.h"
#include "CAxes.h"
#include "CAxis.h"
#include "CAxisTitle.h"
#include "CTitles.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

static void HandleVLCEvents(const VLCEvent* pEvt, void* pUserData)
{       
	CTeeChartDemoDlg* pDlg = reinterpret_cast<CTeeChartDemoDlg*>(pUserData); 

	switch(pEvt->type)
	{
	case libvlc_MediaPlayerEndReached:
		if (pDlg)
		{
			pDlg->AutoStop();
		}
		break;
	}    
}

// CTeeChartDemoDlg 对话框


CTeeChartDemoDlg::CTeeChartDemoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTeeChartDemoDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	created_ = false;
	m_time = 0;
	m_testIndex = -1;
}

void CTeeChartDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TCHART1, m_myChart);
	DDX_Control(pDX, IDC_VLC_1, m_vlcControl1);
	DDX_Control(pDX, IDC_VLC_2, m_vlcControl2);
	DDX_Control(pDX, IDC_COMBO_TEST, m_slectTest);
	DDX_Control(pDX, IDC_TCHART2, m_myChart2);
}

BEGIN_MESSAGE_MAP(CTeeChartDemoDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_PLAY, &CTeeChartDemoDlg::OnBnClickedButtonPlay)
	ON_WM_CTLCOLOR()
	ON_WM_SIZE()
//	ON_STN_CLICKED(IDC_STATIC_2, &CTeeChartDemoDlg::OnStnClickedStatic2)
ON_CBN_SELCHANGE(IDC_COMBO_TEST, &CTeeChartDemoDlg::OnCbnSelchangeComboTest)
END_MESSAGE_MAP()


// CTeeChartDemoDlg 消息处理程序

BOOL CTeeChartDemoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	m_textFont.CreatePointFont(150, "宋体");
	m_playFlag = false;
	m_pauseFlag = false;
	m_vlcPlayer1.SetOutputWindow((void *)m_vlcControl1.GetSafeHwnd());
	m_vlcPlayer2.SetEventHandler(&HandleVLCEvents, this);
	m_vlcPlayer2.SetOutputWindow((void *)m_vlcControl2.GetSafeHwnd());
	m_series1 = (CSeries)m_myChart.Series(0);
	m_series2 = (CSeries)m_myChart2.Series(0);

	// 查找当前目录下的测试视频
	//CFileFind finder;
	//CFileFind findMatch;
	//BOOL bRet = finder.FindFile(".\\*.h264", 0);
	//while (bRet)
	//{
	//	bRet = finder.FindNextFile();
	//	CString strFileName = finder.GetFileTitle();	// 不带后缀的文件名
	//	if (findMatch.FindFile(strFileName + ".bin"))
	//	{
	//		m_slectTest.AddString(strFileName);
	//		findMatch.Close();
	//	}
	//}
	//finder.Close();
	CStdioFile file("data.dat", CFile::modeRead);
	CString line;
	while (file.ReadString(line))
	{
		CString temp;
		AfxExtractSubString(temp, line, 0, '\t');
		m_slectTest.AddString(temp);
		AfxExtractSubString(temp, line, 1, '\t');
		rate1.push_back((float)atof((char *)(LPTSTR)(LPCTSTR)temp));
		AfxExtractSubString(temp, line, 2, '\t');
		rate2.push_back((float)atof((char *)(LPTSTR)(LPCTSTR)temp));
		AfxExtractSubString(temp, line, 3, '\t');
		psnr1.push_back((float)atof((char *)(LPTSTR)(LPCTSTR)temp));
		AfxExtractSubString(temp, line, 4, '\t');
		psnr2.push_back((float)atof((char *)(LPTSTR)(LPCTSTR)temp));
	}
	file.Close();
	created_ = true;
	ShowWindow(SW_SHOWMAXIMIZED);
	//CSeries chart = (CSeries)m_myChart.Series(0);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CTeeChartDemoDlg::OnPaint()
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
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CTeeChartDemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CTeeChartDemoDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	if (m_playFlag)
	{
		libvlc_media_stats_t stats1;
		libvlc_media_stats_t stats2;
		CString strLabel;
		strLabel.Format("%d", m_time);
		if (m_vlcPlayer1.GetMediaStats(&stats1)&&m_vlcPlayer2.GetMediaStats(&stats2))
		{
			m_series1.Add(stats1.f_demux_bitrate * 8000, strLabel, 1);
			m_series2.Add(stats2.f_demux_bitrate * 8000, strLabel, 1);
			m_time += 5;
		}
	}
	
	CDialog::OnTimer(nIDEvent);
}


void CTeeChartDemoDlg::OnBnClickedButtonPlay()
{
	// TODO: Add your control notification handler code here
	if (m_playFlag)		// 如果播放时点击暂停按钮
	{
		m_vlcPlayer1.Pause();
		m_vlcPlayer2.Pause();
		SetDlgItemText(IDC_BUTTON_PLAY, _T("播放"));
		m_playFlag = false;
		m_pauseFlag = true;
		//Invalidate(TRUE);
	}
	else
	{
		if (m_pauseFlag)
		{
			m_vlcPlayer1.Play();
			m_vlcPlayer2.Play();
			m_playFlag = true;
			SetDlgItemText(IDC_BUTTON_PLAY, _T("暂停"));
			return;
		}
		CString strText;
		m_testIndex = m_slectTest.GetCurSel();
		m_slectTest.GetWindowText(strText);
		if (strText.IsEmpty())
		{
			MessageBox("请先选择测试视频样本", "提示", MB_ICONWARNING|MB_OK);
			return;
		} 
		else
		{
			CString str1;
			CString str2;
			m_vlcPlayer1.OpenMedia((LPCTSTR)(strText + ".h264"));
			m_vlcPlayer2.OpenMedia((LPCTSTR)(strText + ".bin"));
			m_series1.Clear();
			m_series2.Clear();
			str1.Format("H.264文件:%s.h264\n码率:%.2fkbps PSNR:%.2f", (LPCTSTR)strText, rate1[m_testIndex], psnr1[m_testIndex]);
			str2.Format("H.265文件:%s.bin\n码率:%.2fkbps PSNR:%.2f", (LPCTSTR)strText, rate2[m_testIndex], psnr2[m_testIndex]);

			m_vlcPlayer1.Play();
			m_vlcPlayer2.Play();

			Sleep(300);
			if (m_vlcPlayer1.GetState() < libvlc_Paused)
			{
				CString result;
				SetDlgItemText(IDC_BUTTON_PLAY, _T("暂停"));
				m_playFlag = true;
				m_pauseFlag = false;
				m_time = 0;
				SetDlgItemText(IDC_STATIC_1, str1);
				SetDlgItemText(IDC_STATIC_2, str2);
				result.Format("在编码客观质量相同的情况下，H.265相比H.264码率减少了%.2f%%", (rate1[m_testIndex] - rate2[m_testIndex])/rate1[m_testIndex]*100);
				SetDlgItemText(IDC_STATIC_3, result);
				SetTimer(1, 500, NULL);
			}
		}
	}
	
}

HBRUSH CTeeChartDemoDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  Change any attributes of the DC here
	switch (pWnd->GetDlgCtrlID())
	{
	case IDC_STATIC_1:
		pDC->SetTextColor(RGB(255, 0, 0));
		pDC->SelectObject(&m_textFont);
		break;
	case IDC_STATIC_2:
		pDC->SetTextColor(RGB(0, 0, 255));
		pDC->SelectObject(&m_textFont);
		break;
	case IDC_STATIC_3:
		pDC->SetTextColor(RGB(255, 0, 0));
		pDC->SelectObject(&m_textFont);
		break;
	}
	// TODO:  Return a different brush if the default is not desired
	return hbr;
}

void CTeeChartDemoDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	if (!created_)
	{
		return;
	}
	if(cx == 0 && cy == 0)
	{
		CRect rect;
		GetClientRect(&rect);

		cx = rect.Width();
		cy = rect.Height();
	}
	int controlsHeight = 350;
	int newHeight = cy - controlsHeight - 7;
	GetDlgItem(IDC_BUTTON_PLAY)->MoveWindow(cx/2 + 60, 15, 60, 30);
	GetDlgItem(IDC_STATIC_1)->MoveWindow(50, 15, 450, 50);
	GetDlgItem(IDC_STATIC_2)->MoveWindow(cx - 500, 15, 450, 50);
	m_slectTest.MoveWindow(cx/2 - 150, 15, 150, 100);
	// Resize the VLC static control
	m_vlcControl1.MoveWindow(CRect(10, 70, (cx-10)/2 - 10, newHeight));
	m_vlcControl2.MoveWindow(CRect((cx-10)/2 + 10, 70, cx-10, newHeight));
	m_myChart.MoveWindow(CRect(40, newHeight + 30, (cx-40)/2 - 40, cy - 100));
	m_myChart2.MoveWindow(CRect(cx/2 + 40, newHeight + 30, cx - 40, cy - 100));
	GetDlgItem(IDC_STATIC_3)->MoveWindow(cx/2 - 400, cy - 70, 800, 50);
	Invalidate();
}

void CTeeChartDemoDlg::AutoStop()
{
	SetDlgItemText(IDC_BUTTON_PLAY, _T("播放"));
	if (m_playFlag)
	{
		m_playFlag = false;
		m_pauseFlag = false;
		KillTimer(1);
		Invalidate(TRUE);
	}
}

// 组合框改变选项事件
void CTeeChartDemoDlg::OnCbnSelchangeComboTest()
{
	// TODO: Add your control notification handler code here
	m_vlcPlayer1.Stop();
	m_vlcPlayer2.Stop();
	CString strText;
	m_testIndex = m_slectTest.GetCurSel();
	m_slectTest.GetWindowText(strText);
	if (strText.IsEmpty())
	{
		MessageBox("请先选择测试视频样本", "提示", MB_ICONWARNING|MB_OK);
		return;
	} 
	else
	{
		CString str1;
		CString str2;
		m_vlcPlayer1.OpenMedia((LPCTSTR)(strText + ".h264"));
		m_vlcPlayer2.OpenMedia((LPCTSTR)(strText + ".bin"));
		m_series1.Clear();
		m_series2.Clear();
		str1.Format("H.264文件:%s.h264\n码率:%.2fkbps PSNR:%.2f", (LPCTSTR)strText, rate1[m_testIndex], psnr1[m_testIndex]);
		str2.Format("H.265文件:%s.bin\n码率:%.2fkbps PSNR:%.2f", (LPCTSTR)strText, rate2[m_testIndex], psnr2[m_testIndex]);

		m_vlcPlayer1.Play();
		m_vlcPlayer2.Play();

		Sleep(300);
		if (m_vlcPlayer1.GetState() < libvlc_Paused)
		{
			CString result;
			SetDlgItemText(IDC_BUTTON_PLAY, _T("暂停"));
			m_playFlag = true;
			m_pauseFlag = false;
			m_time = 0;
			SetDlgItemText(IDC_STATIC_1, str1);
			SetDlgItemText(IDC_STATIC_2, str2);
			result.Format("在编码客观质量相同的情况下，H.265相比H.264码率减少了%.2f%%", (rate1[m_testIndex] - rate2[m_testIndex])/rate1[m_testIndex]*100);
			SetDlgItemText(IDC_STATIC_3, result);
			SetTimer(1, 500, NULL);
		}
	}
}
