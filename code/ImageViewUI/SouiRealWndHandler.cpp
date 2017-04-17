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
	{//ֻʵ����button�Ĵ���
		//����һ��MFC CButton����
		CImageCanvaHost *pCanvasHost=new CImageCanvaHost;
		//����CButton����,ע��ʹ��pRealWnd->GetContainer()->GetHostHwnd()��ΪCButton�ĸ�����
		//��pRealWnd->GetID()��Ϊ�洰�ڵ�ID
		pCanvasHost->Create(pRealWnd->GetContainer()->GetHostHwnd(),(DWORD)WS_CHILD|WS_VISIBLE,(DWORD)0);
		//��pbtn��ָ��ŵ�SRealWnd��Data�б��棬�Ա��ڴ���destroyʱ�ͷ�pbtn����
		pRealWnd->SetData(pCanvasHost);
		//���سɹ�������Ĵ��ھ��
		return pCanvasHost->m_hWnd;
	}else
	{
		return 0;
	}
}

void CSouiRealWndHandler::OnRealWndDestroy( SRealWnd *pRealWnd )
{
	
}

//����������FALSE
BOOL CSouiRealWndHandler::OnRealWndSize( SRealWnd *pRealWnd )
{
	const SRealWndParam &param=pRealWnd->GetRealWndParam();
	CImageCanvaHost *pCanvasHost = static_cast<CImageCanvaHost *>(pRealWnd->GetData());
	SOUI::CRect rcClent = pRealWnd->GetParent()->GetClientRect();
	pCanvasHost->Move(rcClent);
	return FALSE;
}

//����������FALSE
BOOL CSouiRealWndHandler::OnRealWndInit( SRealWnd *pRealWnd )
{
	return FALSE;
}