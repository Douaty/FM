
#if !defined(AFX_CFMDlg_H__9406C023_F771_47FF_9864_AE314E89__INCLUDED_)
#define AFX_CFMDlg_H__9406C023_F771_47FF_9864_AE314E89__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LanguageSetDlg.h : header file
//
// #include "DialogBase.h"
/////////////////////////////////////////////////////////////////////////////
// CLanguageSetDlg dialog
#include "CFMFile.h"


#define  WM_FM_CLOSE		0x0000E1F3

#define	 DLG_WIDTH			800//480
#define	 DLG_HIGHT			480//272

typedef enum enBtnState
{
	BTN_STATE_NORMAL,
	BTN_STATE_DOWN,
	BTN_STATE_DISABLE,
}enBtnState;

class ItemInfo
{
public:
	CRect rect;
	CString chChar;
	enBtnState nState;
	char sdlstatus;
};


class CFMDlg : public CDialog
{
	enum { BTN_COUNT = 15 };
public:
	//变换语言和单位
	virtual BOOL OnLanguageChange();
	//变皮肤
	virtual BOOL OnSkinChange();

	CString			m_csState;			//搜索状态

	CFMFile			m_FM_LastPlay;		//保存最后一次播放的频率
	CFMFile			m_FM_Favorite;		//频率收藏
	CFMFile			m_FM_Set;					//FM设置
	int				m_nCurrFrequency;	//当前频率
	int				m_nFM_List_Index;	//当前频率在list中的索引

	UINT32 pFreq[100];

	//收藏栏
	int				m_nIndexFavoriteFreq;	//当前收藏频率的序号-1~FM_COUNT   -1为没有选中
	int				m_nIndexPage;			//当前收藏频率的页0~count-1
	int				m_nPageCount;			//收藏频率的页数count
	enum { FM_COUNT = 4 };					//每页显示收藏频率个数


	enum		enFM{
		FM1 = 0,
		FM2,
		FM3,
		FM4,
		NOTHING
	};

	enFM		enFMSel;
	int			m_nFMVolume;		//当前收音机音量
	

	void OnExit();
	void ReleaseGdi();

	void OnOpen();
	void OnClose();

	void OnRadioOn();
	void OnRadioOff();

	void	OnBtnItemClick(int nIndex);
	void	OnPreviousPage();
	void	OnNextPage();
	void	OnBtnAdd2Favorite();
	void	OnBtnDeleteFavorite();
	void	OnBtnLeft();	//向前搜索
	void	OnBtnRight();	//向后搜索
	void	OnBtnStopSearch();	//停止搜索
	void	OnBtnSetVolume(int nStep);	//调节音量0x0~0x1F
	
	void	Refreshpage();
	HBITMAP LoadImageFromFile(TCHAR * pFileImage);

protected:
	ItemInfo	m_Item[BTN_COUNT];
	HBITMAP		m_stBtnNormalBMP;
	HBITMAP     m_stBtnPressBMP;
	HBITMAP		m_stBtnDisableBMP;
	HBITMAP		m_stBtnBMP;

	HICON		m_hIcon;
	CDC*		m_pDC;
	CDC			memDC;
	CBitmap		bitmap;
	CFont		smallfont;
	CFont		bigfont;

	CString		m_csTitile;
	CRect		g_retTitile;

	CDC			bk_normal_dc;
	CDC			bk_press_dc;

	CBitmap*	m_pold_bk_normal_bmp;
	CBitmap*	m_pold_bk_press_bmp;

	void InitGdi();
	void InitControl();
	void DrawItems(CDC* pDC,const CRect &rt,enBtnState state);
	void DrawBtnText(CDC* pDC,CRect &rt,CString &str,CFont& font, COLORREF  col = RGB(255,255,255),UINT Format = DT_CENTER | DT_VCENTER,BOOL bDisable=FALSE);


public:
	CFMDlg(CWnd* pParent = NULL);   // standard constructor

	// Dialog Data
	//{{AFX_DATA(CFMDlg)
	enum { IDD = IDD_FM_DIALOG };
	//}}AFX_DATA

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFMDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CLanguageSetDlg)
	virtual BOOL OnInitDialog();
	//	afx_msg void OnBtnF();
	//	afx_msg void OnBtnB();
	// 	afx_msg void OnBtnCancel();
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LANGUAGESETDLG_H__9406C023_F771_47FF_9864_AEAE2E314E89__INCLUDED_)
