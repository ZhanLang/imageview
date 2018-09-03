#pragma once
#include <functional>
#define WM_EXECUTE_MSG (WM_USER + 55648)
class SHostWndEx :
	public SHostWnd
{
public:
	SHostWndEx(LPCTSTR pszResName = NULL);
	~SHostWndEx(void);

	void Post(const std::function<void()> &f);

	BOOL SetWindowText( LPCTSTR lpszCtrl, LPCTSTR lpszText);
	BOOL EnableWindow( LPCTSTR lpszCtrl, BOOL bEnable );
	BOOL SetVisible(LPCTSTR lpszCtrl, BOOL bShow);

	
	BEGIN_MSG_MAP_EX(SHostWndEx)
		MESSAGE_HANDLER(WM_EXECUTE_MSG, OnExecute)
		MSG_WM_DESTROY(OnDestory)
		CHAIN_MSG_MAP(SHostWnd)
		REFLECT_NOTIFICATIONS_EX()
	END_MSG_MAP()

protected:
	void OnDestory();
	LRESULT OnExecute(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL/* bHandled*/);
};

