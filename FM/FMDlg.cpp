
#include "stdafx.h"
#include "FM.h"
#include "FMDlg.h"
#include "CGDICommon.h"
#include <INITGUID.h> 
#include <imaging.h>
#include "Lib/midware.h"

#pragma comment(lib,"./Lib/midware.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


UINT WM_DlgFM_Refresh = RegisterWindowMessage(_T("WM_DlgFM_Refresh"));

// CFMDlg dialog

extern void PlayKeySound();

CFMDlg::CFMDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFMDlg::IDD, pParent)
{
	m_nCurrFrequency	= 9400;
	m_nIndexPage		= 0;
	enFMSel = NOTHING;
	m_nFMVolume				= 10;
	memset(pFreq, 0, sizeof(pFreq));
}
 
void CFMDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFMDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CFMDlg, CDialog)
	//{{AFX_MSG_MAP(CFMDlg)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
	ON_WM_TIMER()
END_MESSAGE_MAP()

BOOL CFMDlg::OnLanguageChange()
{
	m_csTitile = _T("收音机");
	
	return TRUE;
}
//变皮肤
BOOL CFMDlg::OnSkinChange()
{
	return TRUE;	
}

BOOL CFMDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	SetIcon(m_hIcon, TRUE);	
	SetIcon(m_hIcon, FALSE);

	SetWindowText(_T("WND_NAME_FM"));

	MoveWindow(0,0,800,480);
	CenterWindow(GetDesktopWindow());

	InitGdi();
	InitControl();

	OnLanguageChange();
	OnSkinChange();

	CString strTemp;
	GetModuleFileName(NULL,strTemp.GetBuffer(60),60);
	strTemp.ReleaseBuffer();
	strTemp = strTemp.Left(strTemp.ReverseFind('\\'));


	CString strLastPlayPath = strTemp + _T("\\FM_LastPlay.txt");
	CString strFavoritePath = strTemp + _T("\\FM_Favorite.txt");
	CString strConfigPath = strTemp + _T("\\config.ini");

	m_FM_Favorite.Load(strFavoritePath);		//收藏的列表
	m_FM_LastPlay.Load(strLastPlayPath);		//保存最后一次播放的频率

	if (m_FM_Set.Load(strConfigPath))
	{
		int itemp = 25;
		m_FM_Set.GetInt(_T("Set"), _T("VOL"), itemp);
		m_nFMVolume = itemp;

		m_FM_Set.GetInt(_T("Set"), _T("CurrFrequency"), itemp);
		m_nCurrFrequency = itemp;
	}

	SetTimer(0x1010, 1000, NULL);

	Refreshpage();
	
	return TRUE;
}

void CFMDlg::InitGdi()
{
	CRect rc;
	HDC hDC;
	hDC = ::GetDC(m_hWnd);
	m_pDC = CDC::FromHandle(hDC);
	GetClientRect(rc);
	memDC.CreateCompatibleDC(m_pDC);
	bitmap.CreateCompatibleBitmap(m_pDC,rc.Width(),rc.Height());

	bk_normal_dc.CreateCompatibleDC(m_pDC);
	bk_press_dc.CreateCompatibleDC(m_pDC);

	smallfont.CreateFont(
		25,					    // nHeight
		9,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_SEMIBOLD,                 // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		ANTIALIASED_QUALITY,       // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		_T("宋体\0")); 

	bigfont.CreateFont(
		40,					    // nHeight
		16,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_THIN,                 // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		ANTIALIASED_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		_T("宋体\0"));             // lpszFacename

	CString strTemp;
	GetModuleFileName(NULL,strTemp.GetBuffer(60),60);
	strTemp.ReleaseBuffer();
	strTemp = strTemp.Left(strTemp.ReverseFind('\\'));
	CString strPath = strTemp + _T("\\bgFM_N.jpg");
	m_stBtnNormalBMP	= LoadImageFromFile(strPath.GetBuffer(strPath.GetLength()));
	strPath = strTemp + _T("\\bgFM_P.jpg");
	m_stBtnPressBMP		= LoadImageFromFile(strPath.GetBuffer(strPath.GetLength()));

	m_pold_bk_normal_bmp	= bk_normal_dc.SelectObject(CBitmap::FromHandle(m_stBtnNormalBMP));
	m_pold_bk_press_bmp		= bk_press_dc.SelectObject(CBitmap::FromHandle(m_stBtnPressBMP));

	::ReleaseDC(m_hWnd, hDC);
}

void CFMDlg::ReleaseGdi()
{
	bk_press_dc.SelectObject(m_pold_bk_press_bmp);
	bk_normal_dc.SelectObject(m_pold_bk_normal_bmp);

	DeleteObject(m_stBtnPressBMP);
	DeleteObject(m_stBtnNormalBMP);

	bk_press_dc.DeleteDC();		
	bk_normal_dc.DeleteDC();

	bitmap.DeleteObject();
	memDC.DeleteDC();
}

void CFMDlg::OnExit()
{
	OnRadioOff();

	m_FM_LastPlay.AddFM(m_nCurrFrequency);
	m_FM_LastPlay.Flush();

	m_FM_Set.SetInt(_T("Set"), _T("CurrFrequency"), m_nCurrFrequency);
	m_FM_Set.SetInt(_T("Set"), _T("VOL"), m_nFMVolume);
	m_FM_Set.Flush();

	m_FM_Favorite.Flush();

	ReleaseGdi();
	CDialog::EndDialog(0);
}

void CFMDlg::InitControl()
{
	int i;
	for (i=0;  i < BTN_COUNT; i++)
	{
		m_Item[i].nState = BTN_STATE_NORMAL;
	}

	//频道列表
	for(i=0; i<4; i++)
	{
		m_Item[i].rect.left		= 0;
		m_Item[i].rect.top		= 65+83*i;
		m_Item[i].rect.right	= 223;
		m_Item[i].rect.bottom	= 65+83*i+82;
	}
	//上一页
	m_Item[4].rect.left		= 0;
	m_Item[4].rect.top		= 398;
	m_Item[4].rect.right	= 111;
	m_Item[4].rect.bottom	= 480;
	//下一页
	m_Item[5].rect.left		= 114;
	m_Item[5].rect.top		= 398;
	m_Item[5].rect.right	= 224;
	m_Item[5].rect.bottom	= 480;
	//保存
	m_Item[6].rect.left		= 226;
	m_Item[6].rect.top		= 398;
	m_Item[6].rect.right	= 338;
	m_Item[6].rect.bottom	= 480;
	//删除
	m_Item[7].rect.left		= 339;
	m_Item[7].rect.top		= 398;
	m_Item[7].rect.right	= 451;
	m_Item[7].rect.bottom	= 480;
	//向后搜索
	m_Item[8].rect.left		= 452;
	m_Item[8].rect.top		= 398;
	m_Item[8].rect.right	= 563;
	m_Item[8].rect.bottom	= 480;
	//向前搜索
	m_Item[9].rect.left		= 564;
	m_Item[9].rect.top		= 398;
	m_Item[9].rect.right	= 676;
	m_Item[9].rect.bottom	= 480;
	//停止搜索
	m_Item[10].rect.left	= 677;
	m_Item[10].rect.top		= 398;
	m_Item[10].rect.right	= 800;
	m_Item[10].rect.bottom	= 480;
	//音量-
	m_Item[11].rect.left	= 726;
	m_Item[11].rect.top		= 332;
	m_Item[11].rect.right	= 800;
	m_Item[11].rect.bottom	= 390;
	//音量+
	m_Item[12].rect.left	= 726;
	m_Item[12].rect.top		= 270;
	m_Item[12].rect.right	= 800;
	m_Item[12].rect.bottom	= 330;
	
	//Exit
	m_Item[13].rect.left	= 720;
	m_Item[13].rect.top		= 0;
	m_Item[13].rect.right	= 800;
	m_Item[13].rect.bottom	= 60;

	//Exit
	m_Item[14].rect.left	= 600;
	m_Item[14].rect.top		= 0;
	m_Item[14].rect.right	= 698;
	m_Item[14].rect.bottom	= 65;
	m_Item[14].chChar		= _T("最小化");
}

void CFMDlg::DrawItems(CDC* pDC,const CRect &rt,enBtnState state)
{
	ASSERT(pDC);
	int x = 0;
	int y = 0;
	switch(state)
	{
	case BTN_STATE_NORMAL:
		pDC->BitBlt(rt.left,rt.top,rt.Width(),rt.Height(),&bk_normal_dc,rt.left,rt.top,SRCCOPY);
		break;
	case BTN_STATE_DOWN:
		pDC->BitBlt(rt.left,rt.top,rt.Width(),rt.Height(),&bk_press_dc,rt.left,rt.top,SRCCOPY);
		break;
	case BTN_STATE_DISABLE:
		break;
	default:
		pDC->BitBlt(rt.left,rt.top,rt.Width(),rt.Height(),&bk_normal_dc,rt.left,rt.top,SRCCOPY);
		break;
	}
}

void CFMDlg::DrawBtnText(CDC* pDC,CRect &rt,CString &str,CFont& font,COLORREF col,UINT Format ,BOOL bDisable/* =FALSE */)
{
	COLORREF  nOldRgb;
	nOldRgb=pDC->SetTextColor( col );
	int      nOldMode = pDC->SetBkMode(TRANSPARENT);
	CFont* def_font = pDC->SelectObject(&font);

	pDC->DrawText(str, &rt, Format );

	pDC->SelectObject(def_font);
	pDC->SetBkMode(nOldMode);
	pDC->SetTextColor(nOldRgb);
}

void CFMDlg::OnPaint() 
{
	CPaintDC dc(this);
	CRect rc;
	GetClientRect(rc);
	CBitmap* pOldBmp = memDC.SelectObject(&bitmap);
	memDC.BitBlt(0,0, rc.Width(), rc.Height(), &bk_normal_dc, 0, 0, SRCCOPY);

	CString str;
	CRect Rect;
	enBtnState state;

	for(int i=0;i<BTN_COUNT;i++)
	{
		//绘制背景
		str = m_Item[i].chChar;
		Rect = m_Item[i].rect;
		state = m_Item[i].nState;
		DrawItems(&memDC,Rect,state);
		DrawBtnText(&memDC,Rect,str,CGDICommon::Instance()->bigfont(),
			RGB(255,255,255),DT_CENTER|DT_VCENTER);
	}
	//当前播放频率
	str.Format(_T("FM: %d.%dMHz"), m_nCurrFrequency/100, m_nCurrFrequency-m_nCurrFrequency/100*100);
	DrawBtnText(&memDC,CRect(333,146,690,242), str, CGDICommon::Instance()->bigfont(),
		RGB(255,255,255), DT_CENTER | DT_VCENTER);
	//标题
	DrawBtnText(&memDC,g_retTitile,m_csTitile,CGDICommon::Instance()->bigbigfont(),
		RGB(255,255,255), DT_CENTER | DT_VCENTER);
	//显示收藏页
	str.Format(_T("%d/%d"), m_nIndexPage+1, m_nPageCount);
	DrawBtnText(&memDC,CRect(227,352,319,388), str, CGDICommon::Instance()->smallfont(),
		RGB(255,255,255), DT_CENTER | DT_VCENTER);
	//显示搜索状态
	DrawBtnText(&memDC,CRect(334,346,692,388), m_csState, CGDICommon::Instance()->smallfont(),
		RGB(255,255,255), DT_CENTER | DT_VCENTER);

	BOOL b = dc.BitBlt(0, 0, rc.Width(), rc.Height(),&memDC,0,0,SRCCOPY);
	ASSERT(b);

	memDC.SelectObject(pOldBmp);
}

void CFMDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	int i = 0;
	BOOL bRc = false;
	bRc = PtInRect(&m_Item[7].rect,point);	//删除收藏
	for(i=0; i<4; i++)
	{
		if(!bRc)
		{
			if(m_Item[i].nState == BTN_STATE_DOWN)
			{
				m_Item[i].nState = BTN_STATE_NORMAL;
				enFMSel = NOTHING;	//Unselect.
			}
		}
	}
	
	for(i=0; i<BTN_COUNT; i++)
	{
		if(PtInRect(&m_Item[i].rect, point))
		{
			if(m_Item[i].nState == BTN_STATE_NORMAL)
			{
				m_Item[i].nState = BTN_STATE_DOWN;
				PlayKeySound();
			}
		}
	}
	
 	Invalidate(false);
	CDialog::OnLButtonDown(nFlags, point);
}

void CFMDlg::OnLButtonUp(UINT nFlags, CPoint point) 
{
	int i;
	//FM列表
	for(i=0; i<4; i++)
	{
		if(m_Item[i].nState == BTN_STATE_DOWN )
		{
			switch( i )
			{
			case 0:
			case 1:
			case 2:
			case 3:
				enFMSel = (enFM)i;
				OnBtnItemClick(i);
				break;
			}
		}
	}
	//其它按钮
	for(i=4; i<BTN_COUNT; i++)
	{
		if(m_Item[i].nState==BTN_STATE_DOWN)
		{
			m_Item[i].nState = BTN_STATE_NORMAL;			
			switch( i )
			{
			case 4://上一页
				OnPreviousPage();
				break;
			case 5://下一页
				OnNextPage();
				break;
			case 6://保存到列表
				OnBtnAdd2Favorite();
				break;
			case 7://删除选中的fm
				OnBtnDeleteFavorite();
				if(enFMSel != NOTHING)
				{
					m_Item[enFMSel].nState = BTN_STATE_NORMAL;
					enFMSel = NOTHING;	//Unselect.
				}
				break;
			case 8:		//搜台-
				OnBtnLeft();
				break;
			case 9:		//搜台+
				OnBtnRight();
				break;
			case 10:		//停止搜台
				OnBtnStopSearch();
				break;
			case 11:	//音量-
				OnBtnSetVolume(-1);
				break;
			case 12:	//音量+
				OnBtnSetVolume(1);
				break;
			case 13:
				OnExit();
				break;
			case 14:
				::SetWindowPos(m_hWnd, HWND_TOPMOST, 0, 0, DLG_WIDTH, DLG_HIGHT, SWP_HIDEWINDOW);	//隐藏窗口
				break;
			default:
				break;
			}
		}
	}

	Invalidate(false);
	CDialog::OnLButtonUp(nFlags, point);
}

void CFMDlg::OnRadioOn()
{

	TunerInit();							//收音机初始化 此接口运行时间比较久
	SPK_Channel(1);					//切换到收音机声道
	//AMP_EN(TRUE);				//打开功放

	SPK_ChannelVol(m_nFMVolume);   //设置收音机音量大小为上次保存的音量

	TunerSetFreq(m_nCurrFrequency);   //播放上次退出时保存的频段

}

void CFMDlg::OnRadioOff() 
{
	SPK_Channel(2);					//切换到系统声道
}
//点击收藏栏频率列表的项
void CFMDlg::OnBtnItemClick(int nIndex)
{
	//每选中则直接退出
	if(enFMSel == NOTHING)
		return;
	//计算选中的频率在收藏列表中的index
	int nTotalIndex = nIndex+m_nIndexPage*FM_COUNT;
	if(nTotalIndex < m_FM_Favorite.GetFMCount())
	{
		m_nCurrFrequency = m_FM_Favorite.GetFM(nTotalIndex);
		TunerSetFreq(m_nCurrFrequency);
	}
}
//收藏栏上一页
void CFMDlg::OnPreviousPage()
{
	if(m_nIndexPage > 0)
	{
		m_nIndexPage--;
		Refreshpage();
	}
}
//收藏栏下一页
void CFMDlg::OnNextPage()
{
	if(m_nIndexPage < m_nPageCount-1)
	{
		m_nIndexPage++;
		Refreshpage();
	}
}
//将当前播放频率添加到收藏栏
void CFMDlg::OnBtnAdd2Favorite()
{
// 	m_FM_Favorite.AddFM(m_nCurrFrequency);
// 	Refreshpage();

	if (pFreq[0])
	{
		m_FM_Favorite.AddFM(pFreq[0]);
		Refreshpage();
	}
}
//将选中的收藏栏频率删除
void CFMDlg::OnBtnDeleteFavorite()
{
	//每选中则直接退出
	if(enFMSel == NOTHING)
		return;
	//计算选中的频率在收藏列表中的index
	int nIndex = enFMSel+m_nIndexPage*FM_COUNT;
	if(nIndex < m_FM_Favorite.GetFMCount())
	{
		m_FM_Favorite.RemoveFM(nIndex);
		Refreshpage();
	}
}
//向前搜索频率
void CFMDlg::OnBtnLeft()
{//8750~10800
	TunerSeek(-1, pFreq);  //step 0.1M  将存放台的数组pFreq作为实参
	if (pFreq[0])
	{
		m_nCurrFrequency = pFreq[0];   //搜完台后默认播放搜到的第一个有效台
	}

	Invalidate(FALSE);
}
//向后搜索频率
void CFMDlg::OnBtnRight()
{//8750~10800
	TunerSeek(1, pFreq);  //step 0.1M  将存放台的数组pFreq作为实参	
	if (pFreq[0])
	{
		m_nCurrFrequency = pFreq[0];   //搜完台后默认播放搜到的第一个有效台
	}

	Invalidate(FALSE);
}
//停止搜索
void CFMDlg::OnBtnStopSearch()
{

}

//调节音量0x0~0x50
void CFMDlg::OnBtnSetVolume(int nStep)
{
	//在搜台时不能调节音量
	m_nFMVolume += nStep;
	if(m_nFMVolume <= 0)
		m_nFMVolume = 0;
	else if(m_nFMVolume >= 0x50)
		m_nFMVolume = 0x50;

	SPK_ChannelVol(m_nFMVolume);   //设置收音机音量大小
}

//刷新页面数据
void CFMDlg::Refreshpage()
{
	int nFMCount = m_FM_Favorite.GetFMCount();
	for(int i=0; i<FM_COUNT; i++)
	{
		int nIndex = i+m_nIndexPage*FM_COUNT;
		if(nIndex < nFMCount)
		{
			int nFreq = m_FM_Favorite.GetFM(nIndex);
			m_Item[i].chChar.Format(_T("%d.%dMHz"), nFreq/100, nFreq-nFreq/100*100);
		}
		else
		{
			m_Item[i].chChar = _T("");
		}
	}
	//计算收藏栏总页数
	(nFMCount==0) ? (m_nPageCount=1) : (m_nPageCount=(nFMCount-1)/4+1);
}

HBITMAP CFMDlg::LoadImageFromFile(TCHAR * pFileImage)
{ 
	IImagingFactory *pImgFactory = NULL;
	IImage *pImageBmp = NULL; 
	CoInitializeEx(NULL, COINIT_MULTITHREADED); 
	HBITMAP hResult = 0;

	if (SUCCEEDED(CoCreateInstance (CLSID_ImagingFactory, NULL,
		CLSCTX_INPROC_SERVER, IID_IImagingFactory, (void **)&pImgFactory)))
	{
		ImageInfo imageInfo;
		HRESULT hr = pImgFactory->CreateImageFromFile(pFileImage, &pImageBmp);
		if (SUCCEEDED(hr)&& SUCCEEDED(pImageBmp->GetImageInfo(&imageInfo)))
		{
			CWindowDC dc(0);
			CDC dcBitmap;

			dcBitmap.CreateCompatibleDC(&dc);
			hResult = ::CreateCompatibleBitmap(dc.GetSafeHdc(),imageInfo.Width, imageInfo.Height);
			HGDIOBJ hOldBitmap = SelectObject(dcBitmap, hResult);
			//内存不足的话，Draw操作可能会失败
			pImageBmp->Draw(dcBitmap, CRect(0, 0, imageInfo.Width, imageInfo.Height), NULL); 
			SelectObject(dcBitmap, hOldBitmap);
			DeleteDC(dcBitmap);
			pImageBmp->Release();
		}
		pImgFactory->Release();
	}
	CoUninitialize();

	return hResult;
}

LRESULT CFMDlg::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: Add your specialized code here and/or call the base class
	
	if (WM_DlgFM_Refresh == message)
	{
		::SetWindowPos(m_hWnd, HWND_TOPMOST, 0, 0, DLG_WIDTH, DLG_HIGHT, SWP_SHOWWINDOW);	//置顶显示
		UpdateWindow();
	}
	else if (WM_FM_CLOSE == message)
	{
		SetTimer(0x1009, 100, NULL);
	}

	return CDialog::DefWindowProc(message, wParam, lParam);
}

void CFMDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	if (0x1009 == nIDEvent)
	{
		OnExit();
	}
	else if (0x1010 == nIDEvent)
	{
		KillTimer(0x1010);
		OnRadioOn();
	}

	CDialog::OnTimer(nIDEvent);
}