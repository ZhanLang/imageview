#include "StdAfx.h"
#include "SouiRealWndHandler.h"

#include "ImageCanvaHost.h"

CSouiRealWndHandler::CSouiRealWndHandler(void)
{
}


CSouiRealWndHandler::~CSouiRealWndHandler(void)
{
}

HWND CSouiRealWndHandler::OnRealWndCreate( SRealWnd *pRealWnd )
{
	const SRealWndParam &param=pRealWnd->GetRealWndParam();
	if(param.m_strClassName==_T("imageview"))
	{//只实现了button的创建
		//分配一个MFC CButton对象
		CImageCanvaHost *pCanvasHost=new CImageCanvaHost;
		//创建CButton窗口,注意使用pRealWnd->GetContainer()->GetHostHwnd()作为CButton的父窗口
		//把pRealWnd->GetID()作为真窗口的ID
		pCanvasHost->Create(pRealWnd->GetContainer()->GetHostHwnd(),(DWORD)WS_CHILD|WS_VISIBLE,(DWORD)0);
		//把pbtn的指针放到SRealWnd的Data中保存，以便在窗口destroy时释放pbtn对象。
		pRealWnd->SetData(pCanvasHost);
		//返回成功创建后的窗口句柄
		return pCanvasHost->m_hWnd;
	}else
	{
		return 0;
	}
}

void CSouiRealWndHandler::OnRealWndDestroy( SRealWnd *pRealWnd )
{
	
}

//不处理，返回FALSE
BOOL CSouiRealWndHandler::OnRealWndSize( SRealWnd *pRealWnd )
{
	const SRealWndParam &param=pRealWnd->GetRealWndParam();
	CImageCanvaHost *pCanvasHost = static_cast<CImageCanvaHost *>(pRealWnd->GetData());
	SOUI::CRect rcClent = pRealWnd->GetParent()->GetClientRect();
	pCanvasHost->Move(rcClent);
	return FALSE;
}

//不处理，返回FALSE
BOOL CSouiRealWndHandler::OnRealWndInit( SRealWnd *pRealWnd )
{
	return FALSE;
}