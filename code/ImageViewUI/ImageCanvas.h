#pragma once

#include "ImageCanvasWnd.h"

#include <functional>
#include <ximage.h>
#include <imagestone/ImageStone.h>
#include <Gdiplus.h>
#include <GdiPlusImaging.h>
#include <map>
#include "SFreeMoveWindow1.h"


struct DocType
{
public:
	int nID;
	BOOL bRead;
	BOOL bWrite;
	const TCHAR* description;
	const TCHAR* ext;
};

static DocType doctypes[CMAX_IMAGE_FORMATS] =
{
	{ -1, TRUE, TRUE, _T("Supported files"), _T("*.bmp;*.gif;*.jpg;*.jpeg;*.png;*.ico;*.tif;*.tiff;*.tga;*.pcx;*.wbmp;*.wmf;*.emf;*.j2k;*.jp2;*.jbg;*.j2c;*.jpc;*.pgx;*.pnm;*.pgm;*.ppm;*.ras;*.mng;*.jng;*.ska;*.nef;*.crw;*.cr2;*.mrw;*.raf;*.erf;*.3fr;*.dcr;*.raw;*.dng;*.pef;*.x3f;*.arw;*.sr2;*.mef;*.orf;*.psd") },
#if CXIMAGE_SUPPORT_BMP
	{ CXIMAGE_FORMAT_BMP, TRUE, TRUE, _T("BMP files"), _T("*.bmp") },
#endif
#if CXIMAGE_SUPPORT_GIF
	{ CXIMAGE_FORMAT_GIF, TRUE, FALSE, _T("GIF files"), _T("*.gif") },
#endif
#if CXIMAGE_SUPPORT_JPG
	{ CXIMAGE_FORMAT_JPG, TRUE, TRUE, _T("JPG files"), _T("*.jpg;*.jpeg") },
#endif
#if CXIMAGE_SUPPORT_PNG
	{ CXIMAGE_FORMAT_PNG, TRUE, TRUE, _T("PNG files"), _T("*.png") },
#endif
#if CXIMAGE_SUPPORT_MNG
	{ CXIMAGE_FORMAT_MNG, TRUE, TRUE, _T("MNG files"), _T("*.mng;*.jng;*.png") },
#endif
#if CXIMAGE_SUPPORT_ICO
	{ CXIMAGE_FORMAT_ICO, TRUE, FALSE, _T("ICO CUR files"), _T("*.ico;*.cur") },
#endif
#if CXIMAGE_SUPPORT_TIF
	{ CXIMAGE_FORMAT_TIF, TRUE, FALSE, _T("TIF files"), _T("*.tif;*.tiff") },
#endif
#if CXIMAGE_SUPPORT_TGA
	{ CXIMAGE_FORMAT_TGA, TRUE, TRUE, _T("TGA files"), _T("*.tga") },
#endif
#if CXIMAGE_SUPPORT_PCX
	{ CXIMAGE_FORMAT_PCX, TRUE, TRUE, _T("PCX files"), _T("*.pcx") },
#endif
#if CXIMAGE_SUPPORT_WBMP
	{ CXIMAGE_FORMAT_WBMP, TRUE, TRUE, _T("WBMP files"), _T("*.wbmp") },
#endif
#if CXIMAGE_SUPPORT_WMF
	{ CXIMAGE_FORMAT_WMF, TRUE, FALSE, _T("WMF EMF files"), _T("*.wmf;*.emf") },
#endif
#if CXIMAGE_SUPPORT_JBG
	{ CXIMAGE_FORMAT_JBG, TRUE, TRUE, _T("JBG files"), _T("*.jbg") },
#endif
#if CXIMAGE_SUPPORT_JP2
	{ CXIMAGE_FORMAT_JP2, TRUE, TRUE, _T("JP2 files"), _T("*.j2k;*.jp2") },
#endif
#if CXIMAGE_SUPPORT_JPC
	{ CXIMAGE_FORMAT_JPC, TRUE, TRUE, _T("JPC files"), _T("*.j2c;*.jpc") },
#endif
#if CXIMAGE_SUPPORT_PGX
	{ CXIMAGE_FORMAT_PGX, TRUE, TRUE, _T("PGX files"), _T("*.pgx") },
#endif
#if CXIMAGE_SUPPORT_RAS
	{ CXIMAGE_FORMAT_RAS, TRUE, TRUE, _T("RAS files"), _T("*.ras") },
#endif
#if CXIMAGE_SUPPORT_PNM
	{ CXIMAGE_FORMAT_PNM, TRUE, TRUE, _T("PNM files"), _T("*.pnm;*.pgm;*.ppm") },
#endif
#if CXIMAGE_SUPPORT_SKA
	{ CXIMAGE_FORMAT_SKA, TRUE, TRUE, _T("SKA files"), _T("*.ska") },
#endif
#if CXIMAGE_SUPPORT_RAW
	{ CXIMAGE_FORMAT_RAW, TRUE, FALSE, _T("RAW files"), _T("*.nef;*.crw;*.cr2;*.mrw;*.raf;*.erf;*.3fr;*.dcr;*.raw;*.dng;*.pef;*.x3f;*.arw;*.sr2;*.mef;*.orf") },
#endif
#if CXIMAGE_SUPPORT_PSD
	{ CXIMAGE_FORMAT_PSD, TRUE, FALSE, _T("PSD files"), _T("*.psd") }
#endif
};


class  EventCanvasMouseMove : public TplEventArgs<EventCanvasMouseMove>
{
	SOUI_CLASS_NAME(EventCanvasMouseMove,L"on_canvas_mousemove")
public:
	EventCanvasMouseMove(SObject *pSender):TplEventArgs<EventCanvasMouseMove>(pSender){}
	enum{EventID=EVT_EXTERNAL_BEGIN + 100};
	SOUI::CPoint point;
};

class  EventCanvasWndMove : public TplEventArgs<EventCanvasWndMove>
{
	SOUI_CLASS_NAME(EventCanvasWndMove,L"on_canvas_wndmove")
public:
	EventCanvasWndMove(SObject *pSender):TplEventArgs<EventCanvasWndMove>(pSender){}
	enum{EventID=EVT_EXTERNAL_BEGIN + 110};
};

class EventCanvasFrameChange : public TplEventArgs<EventCanvasFrameChange>
{
	SOUI_CLASS_NAME(EventCanvasFrameChange,L"on_canvas_framechange")
public:
	EventCanvasFrameChange(SObject *pSender):TplEventArgs<EventCanvasFrameChange>(pSender){}
	enum{EventID=EVT_EXTERNAL_BEGIN + 120};
};

class EventCanvasImageLoad : public TplEventArgs<EventCanvasImageLoad>
{
	SOUI_CLASS_NAME(EventCanvasImageLoad,L"on_canvas_imageload")
public:
	EventCanvasImageLoad(SObject *pSender):TplEventArgs<EventCanvasImageLoad>(pSender){
		bResult = FALSE;
	}

	enum{EventID=EVT_EXTERNAL_BEGIN + 130};
	BOOL bResult;
};

class CImageCanvas: public SFreeMoveWindow
{
public:
	CImageCanvas();
	~CImageCanvas(void);
	SOUI_CLASS_NAME(CImageCanvas,L"imagecanvas")

	SOUI_MSG_MAP_BEGIN()
		MSG_WM_PAINT_EX(OnPaint)
		MSG_WM_MOUSEMOVE(OnMouseMove)
		MSG_WM_SIZE(OnSize)
		MSG_WM_LBUTTONDOWN(OnLButtonDown)
		MSG_WM_LBUTTONUP(OnLButtonUp)
		MSG_WM_TIMER(OnTimer)
	SOUI_MSG_MAP_END()

public:
	void Post(const std::function<void()> &f);
	BOOL SetImageFile( LPCTSTR lpszImageFile);
	DWORD GetImageWidth();
	DWORD GetImageHeight();
	BOOL  IsImageValide();
	BOOL IsAnimation();
	BOOL IsMutiframe();
	BOOL IsAniming(); //是否正在进行播放

	BOOL NextFrame();
	BOOL PrevFrame();
	UINT GetCurFrame();
	UINT GetFrameCount();

	BOOL StartAnim();
	BOOL PauseAnim();

	//是否可编辑
	BOOL	CanEdition();


	BOOL RotateLeft();
	BOOL RotateRight();
	LPCTSTR GetImageFile();
	VOID DestoryImage();
protected:
	void OnSize(UINT nType, SOUI::CSize size);
	int OnCreate(LPCREATESTRUCT lpCreateStruct);
	void OnShowWindow(BOOL bShow, UINT nStatus);
	BOOL OnInitDialog(HWND wndFocus, LPARAM lInitParam);
	void OnDestory();
	void OnClose();

protected: //SFreeMoveWindow
	virtual void OnUpdateFloatPosition(const SOUI::CRect & rcParent);
protected:
	LRESULT OnExecute(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL/* bHandled*/);
	void OnPaint(IRenderTarget *pRT);
	void OnPaint(HDC hDC);
	void OnNcPaint(IRenderTarget *pRT);
	BOOL OnEraseBkgnd(IRenderTarget *pRT);
	VOID OnMouseMove(UINT nFlags,SOUI::CPoint pt);
	void OnLButtonDown(UINT nFlags,SOUI::CPoint pt);
	void OnLButtonUp(UINT nFlags,SOUI::CPoint pt);
	void OnTimer(char cTimerID);

private:
	
	BOOL Load( );
private:
	BOOL			m_bLayoutInited;

	SStringT m_strImageFile;


	typedef struct _BITMAP_FRAME_
	{
		Gdiplus::Image* bitmap;
		INT TickClock;
	}BITMAP_FRAME;

	std::vector<Gdiplus::Image*>		m_BitmapFrame;
	BOOL	m_bAnime;					//是否为动画
	BOOL	m_bAniming;					//是否正在播放
	Gdiplus::Image *					m_pImage;

	DWORD m_dwImageWidth;
	DWORD m_dwImageHeight;
	INT		m_nImageType;
	BOOL	m_bMulitFrame;
	UINT	m_nFrameCount;
	Gdiplus::PropertyItem*	m_pPropertyItem;	// 帧与帧之间间隔时间
	UINT			m_nFramePosition;			// 当前放到第几帧
};

