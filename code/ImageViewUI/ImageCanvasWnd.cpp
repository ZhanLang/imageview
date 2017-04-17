#include "StdAfx.h"
#include "ImageCanvasWnd.h"


CImageCanvasWnd::CImageCanvasWnd(void) : SHostWnd(L"LAYOUT:XML_IMAGE_CANVAS")
{

}


CImageCanvasWnd::~CImageCanvasWnd(void)
{
}

int CImageCanvasWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
#ifdef DWMBLUR	//win7Ã«²£Á§¿ª¹Ø
	MARGINS mar = {5,5,30,5};
	DwmExtendFrameIntoClientArea ( m_hWnd, &mar );
#endif

	//SetIcon(IDI_ICON_MAIN);

	SetMsgHandled(FALSE);

	return 0;
}

BOOL CImageCanvasWnd::OnInitDialog(HWND hWnd, LPARAM lParam)
{
	m_bLayoutInited = TRUE;
	


	return TRUE;
}


void CImageCanvasWnd::OnDestory()
{
	MSG msg;
	while (PeekMessage(&msg, m_hWnd, WM_EXECUTE_MSG, WM_EXECUTE_MSG, PM_REMOVE)) 
	{
		DispatchMessage(&msg);
	}

	
	SetMsgHandled(FALSE);
}


void CImageCanvasWnd::Post(const std::function<void()> &f)
{
	auto func = new std::function<void()>(f);
	PostMessage( WM_EXECUTE_MSG, 0, (LPARAM)func);
}


void CImageCanvasWnd::OnSize(UINT nType, SOUI::CSize size)
{
	SetMsgHandled(FALSE);
	if (!m_bLayoutInited) return;

	::PostMessage(::GetParent(m_hWnd), WM_CANVAS_SIZE, 0,0);
}

LRESULT CImageCanvasWnd::OnExecute(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL/* bHandled*/)
{
	auto func = (std::function<void()> *)lParam;
	(*func)();
	delete func;
	return 0;
}