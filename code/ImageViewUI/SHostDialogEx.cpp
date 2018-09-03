#include "StdAfx.h"
#include "SHostDialogEx.h"


#define RC_INIT -1

SHostDialogEx::SHostDialogEx(LPCTSTR pszXmlName):SHostWndEx(pszXmlName),m_nRetCode(RC_INIT)
{
	m_MsgLoop = SApplication::getSingleton().GetMsgLoopFactory()->CreateMsgLoop();
}

SHostDialogEx::~SHostDialogEx(void)
{
	SApplication::getSingleton().GetMsgLoopFactory()->DestoryMsgLoop(m_MsgLoop);
}

INT_PTR SHostDialogEx::DoModal(HWND hParent/*=NULL*/)
{
	if(!hParent)
	{
		hParent = ::GetActiveWindow();
	}

	BOOL bEnableParent = FALSE;
	if (hParent && hParent != ::GetDesktopWindow() && ::IsWindowEnabled(hParent))
	{
		::EnableWindow(hParent, FALSE);
		bEnableParent = TRUE;
	}

	if(!Create(hParent,WS_POPUP | WS_CLIPCHILDREN,0, 0,0,0,0))
		return 0;

	CSimpleWnd::SendMessage(WM_INITDIALOG, (WPARAM)m_hWnd);

	if(m_nRetCode == RC_INIT)
	{
		HWND hWndLastActive = ::SetActiveWindow(m_hWnd);

		if(GetExStyle()&WS_EX_TOOLWINDOW)
			::ShowWindow(m_hWnd,SW_SHOWNOACTIVATE);
		else
			::ShowWindow(m_hWnd,SW_SHOWNORMAL);


		int nRet = m_MsgLoop->Run();

		if(m_nRetCode == RC_INIT)
		{//不是自己主动使用EndDialog关闭窗口，重新把WM_QUIT放回消息队列。
			PostQuitMessage(nRet);
		}
		// From MFC
		// hide the window before enabling the parent, etc.
		if ( IsWindow() )
		{
			CSimpleWnd::SetWindowPos(
				NULL, 0, 0, 0, 0,
				SWP_HIDEWINDOW | SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER );
		}

		if (bEnableParent)
		{
			::EnableWindow(hParent, TRUE);
		}

		::SetActiveWindow(hWndLastActive);
	}

	if ( IsWindow() )
		CSimpleWnd::DestroyWindow();

	return m_nRetCode;
}

void SHostDialogEx::EndDialog( INT_PTR nResult )
{
	if (m_nRetCode == RC_INIT)
	{
		m_nRetCode = nResult;
		m_MsgLoop->Quit();
		CSimpleWnd::SetWindowPos(
			NULL, 0, 0, 0, 0,
			SWP_HIDEWINDOW | SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER );
		PostMessage(WM_NULL);
	}
}

void SHostDialogEx::OnOK()
{
	EndDialog(IDOK);
}

void SHostDialogEx::OnCancel()
{
	EndDialog(IDCANCEL);
}

void SHostDialogEx::OnKeyDown( UINT nChar, UINT nRepCnt, UINT nFlags )
{
	SHostWnd::OnKeyEvent(WM_KEYDOWN,(WPARAM)nChar,MAKELPARAM(nRepCnt,nFlags));
	if(SHostWnd::IsMsgHandled()) return;
	if(nChar == VK_ESCAPE || nChar == VK_RETURN)
	{
		SWindow *pBtnExit = FindChildByID(nChar==VK_ESCAPE?IDCANCEL:IDOK);
		if(pBtnExit)
		{
			pBtnExit->FireCommand();
		}
	}
}