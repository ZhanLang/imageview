#pragma once

#include <functional>

#define WM_EXECUTE_MSG (WM_USER + 55648)
#define WM_CANVAS_SIZE ( WM_USER + 55649)



class CImageCanvasWnd : public SHostWnd
{
public:
	CImageCanvasWnd(void);
	~CImageCanvasWnd(void);

	BEGIN_MSG_MAP_EX(CMainDlg)
		MSG_WM_CREATE(OnCreate)
		MSG_WM_INITDIALOG(OnInitDialog)
		MSG_WM_SIZE(OnSize)
		MSG_WM_DESTROY(OnDestory)
		MESSAGE_HANDLER(WM_EXECUTE_MSG, OnExecute)
		CHAIN_MSG_MAP(SHostWnd)
		REFLECT_NOTIFICATIONS_EX()
		END_MSG_MAP()

public:
	void Post(const std::function<void()> &f);

protected:
	void OnSize(UINT nType, SOUI::CSize size);
	int OnCreate(LPCREATESTRUCT lpCreateStruct);
	BOOL OnInitDialog(HWND wndFocus, LPARAM lInitParam);
	void OnDestory();

	LRESULT OnExecute(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL/* bHandled*/);

private:
	BOOL			m_bLayoutInited;
};

