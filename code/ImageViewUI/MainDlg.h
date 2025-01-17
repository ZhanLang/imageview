// MainDlg.h : interface of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////
#pragma once

#include "ImageCanvas.h"
#include <functional>

#include "ImageView.h"
#include "helper/SMenu.h"
#include "helper/SMenuEx.h"
#include "MultiframePanel.h"
#include "ZoomRatioWnd.h"
#include <syncobject/criticalsection.h>
#include <msapi/EnumFile.h>
#include <thread/threadex.h>
#include "dlgUpdate.h"


enum 
{
	MENU_CMD_OPEN_FILE = 2001,
	MENU_CMD_OPEN_PATH,
	MENU_CMD_TOP_MOUST,
	MENU_CMD_SETTING,
	MENU_CMD_UPDATE,


	MENU_CMD_OPEN_EXPLORER,

	MENU_CMD_COPY,
	MENU_CMD_PASTE,

	MENU_CMD_PRINT,

	MENU_CMD_CUT_SCREEN,

};

class CMainDlg : public SHostWnd,public CMsComBase<CMainDlg>
{
public:
	CMainDlg( IMscomRunningObjectTable* pRot);
	~CMainDlg();

	void OnClose();
	void OnMaximize();
	void OnRestore();
	void OnMinimize();
	void OnFullScreen();
	void OnMenu();
	void OnOpen();
	void OnSkin();

	void OnSize(UINT nType, SOUI::CSize size);
	int OnCreate(LPCREATESTRUCT lpCreateStruct);
	BOOL OnInitDialog(HWND wndFocus, LPARAM lInitParam);
	VOID OnCommand(UINT wNotifyCode , int wID , HWND hwndCtl);
	void OnDestory();
public:
	void Post(const std::function<void()> &f);
	VOID SetTabIndex( int nIndex );
	void SetIcon(UINT nRes);
	VOID OpenFile(LPCTSTR lpszPath );
	VOID OpenPath(LPCTSTR lpszPath );
	BOOL IsImageValide();
	BOOL CanBeEdit();
	BOOL IsImageAnim();
	VOID SetSkin( INT nSkin );
protected:
	//soui消息
	EVENT_MAP_BEGIN()
		EVENT_NAME_COMMAND(L"btn_close", OnClose)
		EVENT_NAME_COMMAND(L"btn_min", OnMinimize)
		EVENT_NAME_COMMAND(L"btn_max", OnMaximize)
		EVENT_NAME_COMMAND(L"btn_restore", OnRestore)
		EVENT_NAME_COMMAND(L"btn_screen", OnFullScreen)
		EVENT_NAME_COMMAND(L"btn_menu", OnMenu)
		EVENT_NAME_COMMAND(L"btn_open", OnOpen)
		EVENT_NAME_COMMAND(L"btn_skin", OnSkin)

		EVENT_NAME_COMMAND(L"bar_orignalsize", OnBarOrignalsize)
		EVENT_NAME_COMMAND(L"bar_fitsize", OnBarFitsize)
		EVENT_NAME_COMMAND(L"bar_zoomin", OnBarZoomin)
		EVENT_NAME_COMMAND(L"bar_zoomout", OnBarZoomout)
		EVENT_NAME_COMMAND(L"bar_prev", OnBarPrev)
		EVENT_NAME_COMMAND(L"bar_next", OnBarNext)
		EVENT_NAME_COMMAND(L"btn_bar_prev", OnBarPrev)
		EVENT_NAME_COMMAND(L"btn_bar_next", OnBarNext)
		EVENT_NAME_COMMAND(L"bar_rotateleft", OnBarRotateleft)
		EVENT_NAME_COMMAND(L"bar_rotateright", OnBarRotateright)
		EVENT_NAME_COMMAND(L"bar_delete", OnBarDelete)
		EVENT_NAME_COMMAND(L"bar_more", OnBarMore)

		//multiframe
		EVENT_NAME_COMMAND(L"btn_multiframe_prev", OnMutiframPrev)
		EVENT_NAME_COMMAND(L"btn_multiframe_play", OnMutiframPlay)
		EVENT_NAME_COMMAND(L"btn_multiframe_pause", OnMutiframPause)
		EVENT_NAME_COMMAND(L"btn_multiframe_next", OnMutiframNext)
		EVENT_NAME_COMMAND(L"btn_multiframe_save", OnMutiframSave)
		EVENT_NAME_COMMAND(L"btn_multiframe_saveall", OnMutiframSaveAll)
		//EVENT_NAME_HANDLER(L"page_image", EventCtxMenu::EventID, OnImageMenu)
	EVENT_MAP_END()
	//HostWnd真实窗口消息处理
	BEGIN_MSG_MAP_EX(CMainDlg)
		MSG_WM_CREATE(OnCreate)
		MSG_WM_INITDIALOG(OnInitDialog)
		MSG_WM_CLOSE(OnClose)
		MSG_WM_SIZE(OnSize)
		MSG_WM_DESTROY(OnDestory)
		MSG_WM_KEYDOWN(OnKeyDown)
		MSG_WM_MOUSEMOVE(OnMouseMove)
		MSG_WM_COMMAND(OnCommand)
		MESSAGE_HANDLER(WM_EXECUTE_MSG, OnExecute)
		CHAIN_MSG_MAP(SHostWnd)
		REFLECT_NOTIFICATIONS_EX()
	END_MSG_MAP()


protected:

	VOID OnBarOrignalsize();
	VOID OnBarFitsize();
	VOID OnBarZoomin();
	VOID OnBarZoomout();
	VOID OnBarPrev();
	VOID OnBarNext();
	VOID OnBarRotateleft();
	VOID OnBarRotateright();
	VOID OnBarDelete();
	VOID OnBarMore();

	

protected://multiframe
	VOID OnMutiframPrev();
	VOID OnMutiframPlay();
	VOID OnMutiframPause();
	VOID OnMutiframNext();
	VOID OnMutiframSave();
	VOID OnMutiframSaveAll();


protected:
	LRESULT OnExecute(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL/* bHandled*/);
	void OnMouseMove(UINT nFlags, SOUI::CPoint point);
	void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
private:
	VOID OnCheckUpdata(EventArgs *pEvt = NULL);
	VOID OpenImageFile( LPCTSTR lpszImageFile);

private:
	bool OnCanvasSize(EventSwndSize *pEvt);
	bool OnCanvasMouseMove( EventCanvasMouseMove* pEvt);
	bool OnCanvasMouseLeave(EventSwndMouseLeave* pEvt);
	bool OnCanvasWndMove(EventCanvasWndMove* pEvt);
	bool OnImageViewMouseWhell( EventImageViewMouseWhell* pEvt);
	bool OnCanvasFrameChange( EventCanvasFrameChange* pEvt);
	bool OnCanvasImageLoad(EventCanvasImageLoad* pEvt);
	bool OnImageMenu(EventCtxMenu* pEvt);
private:
	VOID OnWorkCheckUpdata();
	VOID OnCheckUpdateUI();
	//更新图片显示控件大小
	BOOL SetFitSize( BOOL bFitSize);

	//设置缩放比例
	BOOL SetScale( float fScale);

	VOID SetMutiframState();
	VOID SetBarPaneState();
	VOID SetTitleText();
	VOID SetCanvasFitsize();		//设置为1：1大小
	VOID SetCanvasOrignalsize();	//设置为原始大小

	VOID EnumImageFile(LPCTSTR lpstrPath, BOOL bStrike = FALSE);
	
	INT GetCurImageIndex();
	INT GetImageCount();
public:
	VOID OneImageFileFind( LPCTSTR lpszFile);

	BOOL m_bStopEnumFile; //是否退出

private:
	VOID Cf_Hdrop( LPCTSTR lpszFile);
	VOID Cf_Html( LPCTSTR lpszFile);
	VOID Cf_QQEdit( LPCTSTR lpszFile);

private:
	VOID DoReportActive();
	BOOL CheckUpdate();
	VOID DoUpdate();
	DWORD GetOper();
	UPDATE_INFO		m_updateInfo;
	SStringT	m_strLocalVer;
private:
	BOOL			m_bLayoutInited;
	STabCtrl*		m_tableCtrl;
	CImageCanvas*	m_ImageCanvas;
	SCaption*		m_Caption;
	SWindow*		m_ImageView;

	SWindow*		m_PanelLeft;
	SWindow*		m_PanelRight;
	SWindow*		m_PanelBar;

	SWindow*		m_CtrlBar;
	SImageButton*	m_BtnBarNext;
	SImageButton*	m_BtnBarPrev;

	BOOL m_bFitSize; //自适应窗口

	float m_fScale;		//设置缩放比例
	SOUI::CPoint m_ScalPoint; //按比例缩放时的起点位置
	BOOL	m_bOpenFile;


	//
	SImageButton* m_MultiframePrev;
	SImageButton* m_MultiframePlay;	
	SImageButton* m_MultiframePause;
	SImageButton* m_MultiframeNext;
	SImageButton* btn_multiframe_save;
	SImageButton* btn_multiframe_saveall;
	SStatic*	  m_TextFrameCount;
	SStatic*	  m_TextTitle;
	CZoomRatioWnd* m_ZoomRatioWnd;

	CMultiframePanel* m_MutiframPanel;

private:
	SMenuEx m_MenuMain;
	SMenu m_MenuSkin;
	SMenuEx m_MenuImage;
	SStringT m_strFile;

	msdk::CThreadEx m_EnumThreadEx;
	

	std::vector<SStringT> m_FileList;
	DECLARE_AUTOLOCK_CS(m_FileList);
};
