// TestFMDlg.h : header file
//

#pragma once

// CTestFMDlg dialog
class CTestFMDlg : public CDialog
{
// Construction
public:
	CTestFMDlg(CWnd* pParent = NULL);	// standard constructor


	CString strTemp;

// Dialog Data
	enum { IDD = IDD_TESTFM_DIALOG };


	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
#if defined(_DEVICE_RESOLUTION_AWARE) && !defined(WIN32_PLATFORM_WFSP)
	afx_msg void OnSize(UINT /*nType*/, int /*cx*/, int /*cy*/);
#endif
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
};
