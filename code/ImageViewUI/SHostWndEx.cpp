#include "StdAfx.h"
#include "SHostWndEx.h"


SHostWndEx::SHostWndEx(LPCTSTR pszResName):SHostWnd(pszResName)
{
}


SHostWndEx::~SHostWndEx(void)
{
}

void SHostWndEx::Post(const std::function<void()> &f)
{
	auto func = new std::function<void()>(f);
	PostMessage( WM_EXECUTE_MSG, 0, (LPARAM)func);
}

BOOL SHostWndEx::SetWindowText(LPCTSTR lpszCtrl, LPCTSTR lpszText)
{
	SWindow* pCtrl = FindChildByName(lpszCtrl);
	if ( !pCtrl )
		return FALSE;

	 pCtrl->SetWindowText(lpszText);
	 return TRUE;
}

BOOL SHostWndEx::EnableWindow(LPCTSTR lpszCtrl, BOOL bEnable)
{
	SWindow* pCtrl = FindChildByName(lpszCtrl);
	if ( !pCtrl )
		return FALSE;

	pCtrl->EnableWindow(bEnable, TRUE);
	return TRUE;
}

BOOL SHostWndEx::SetVisible(LPCTSTR lpszCtrl, BOOL bShow)
{
	SWindow* pCtrl = FindChildByName(lpszCtrl);
	if ( !pCtrl )
		return FALSE;

	pCtrl->SetVisible(bShow, TRUE);

	return TRUE;
}

void SHostWndEx::OnDestory()
{
	MSG msg;
	while (PeekMessage(&msg, m_hWnd, WM_EXECUTE_MSG, WM_EXECUTE_MSG, PM_REMOVE)) 
	{
		DispatchMessage(&msg);
	}
	SetMsgHandled(FALSE);
}

LRESULT SHostWndEx::OnExecute(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL/* bHandled*/)
{
	auto func = (std::function<void()> *)lParam;
	(*func)();
	delete func;
	return 0;
}
