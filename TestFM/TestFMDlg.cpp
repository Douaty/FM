// TestFMDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TestFM.h"
#include "TestFMDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CTestFMDlg dialog

CTestFMDlg::CTestFMDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTestFMDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTestFMDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CTestFMDlg, CDialog)
#if defined(_DEVICE_RESOLUTION_AWARE) && !defined(WIN32_PLATFORM_WFSP)
	ON_WM_SIZE()
#endif
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, &CTestFMDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CTestFMDlg message handlers

BOOL CTestFMDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	GetModuleFileName(NULL,strTemp.GetBuffer(60),60);
	strTemp.ReleaseBuffer();
	strTemp = strTemp.Left(strTemp.ReverseFind('\\'));
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

#if defined(_DEVICE_RESOLUTION_AWARE) && !defined(WIN32_PLATFORM_WFSP)
void CTestFMDlg::OnSize(UINT /*nType*/, int /*cx*/, int /*cy*/)
{
	if (AfxIsDRAEnabled())
	{
		DRA::RelayoutDialog(
			AfxGetResourceHandle(), 
			this->m_hWnd, 
			DRA::GetDisplayMode() != DRA::Portrait ? 
			MAKEINTRESOURCE(IDD_TESTFM_DIALOG_WIDE) : 
			MAKEINTRESOURCE(IDD_TESTFM_DIALOG));
	}
}
#endif


void CTestFMDlg::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	CString strFMPath = strTemp + _T("\\FM.exe");

	UINT WM_DlgFM_Refresh = RegisterWindowMessage(_T("WM_DlgFM_Refresh"));

	HWND hWnd = ::FindWindow(NULL, _T("WND_NAME_FM"));

	if(NULL != hWnd)
	{
		::PostMessage(hWnd, WM_DlgFM_Refresh, 0, 0);
	}
	else
	{
		CreateProcess(strFMPath, NULL, NULL, NULL, FALSE, 0, NULL, NULL, NULL, NULL);
	}
}
