// FM.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "FM.h"
#include "FMDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CString			g_strPath;	//App目录
// CFMApp

BEGIN_MESSAGE_MAP(CFMApp, CWinApp)
END_MESSAGE_MAP()


// CFMApp construction
CFMApp::CFMApp()
	: CWinApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

void PlayKeySound()
{
	::PlaySound(g_strPath, NULL, SND_ASYNC | SND_NODEFAULT );
}

// The one and only CFMApp object
CFMApp theApp;
BOOL m_bSucceed;
// CFMApp initialization

BOOL CFMApp::InitInstance()
{
	//只启动一个进程
	HANDLE hOneInstance=CreateMutex(NULL,FALSE,_T("FM_ONE_INSTANCE"));
	if(GetLastError() == ERROR_ALREADY_EXISTS)
		return FALSE;

	TCHAR strModuleFileName[256];
	memset((void *)strModuleFileName,0,256);

	GetModuleFileName(NULL,strModuleFileName,256);
	g_strPath = strModuleFileName;

	g_strPath.ReleaseBuffer();
	g_strPath = g_strPath.Left(g_strPath.ReverseFind('\\'));
	g_strPath += _T("\\KeySound.wav\0");

	CFMDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
