#include "StdAfx.h"
#include "MultiframePanel.h"


CMultiframePanel::CMultiframePanel(void)
{
	m_bCustom = FALSE;
}


CMultiframePanel::~CMultiframePanel(void)
{
}

void CMultiframePanel::OnLButtonDown(UINT nFlags,SOUI::CPoint pt)
{
	__super::OnLButtonDown(nFlags, pt);
}

void CMultiframePanel::OnLButtonUp(UINT nFlags,SOUI::CPoint pt)
{
	__super::OnLButtonUp(nFlags, pt);
}

void CMultiframePanel::OnMouseMove(UINT nFlags,SOUI::CPoint pt)
{
	//__super::OnMouseMove(nFlags, pt);
	if ( !m_bDraging )
		return;
	
	m_bCustom = TRUE;

	SOUI::CPoint ptLT = pt - m_ptClick;
	SOUI::CRect rcWnd = GetWindowRect();
	rcWnd.MoveToXY(ptLT);

	SOUI::CRect rcParent = GetParent()->GetClientRect();
	BOOL bTopLeft = rcParent.PtInRect(rcWnd.TopLeft()) ;
	BOOL bBottomRight= rcParent.PtInRect(rcWnd.BottomRight());

	if ( bTopLeft && bBottomRight )
	{
		Move(rcWnd);

		SOUI::CPoint ptCenterWnd = rcWnd.CenterPoint();
		SOUI::CPoint ptCenterParent = rcParent.CenterPoint();

		if(ptCenterWnd.x<ptCenterParent.x)
			m_nFloatHAlign = HALIGN_LEFT;
		else
			m_nFloatHAlign = HALIGN_RIGHT;

		if(ptCenterWnd.y < ptCenterParent.y)
			m_nFloatVAlign = VALIGN_TOP;
		else
			m_nFloatVAlign =VALIGN_BOTTOM;

		if(m_nFloatHAlign & HALIGN_LEFT)
		{
			m_nDistX = rcWnd.left - rcParent.left;
		}else if(m_nFloatHAlign & HALIGN_RIGHT)
		{
			m_nDistX = rcParent.right - rcWnd.right;
		}
		if(m_nFloatVAlign & VALIGN_TOP)
		{
			m_nDistY = rcWnd.top -rcParent.top;
		}else if(m_nFloatVAlign & VALIGN_BOTTOM)
		{
			m_nDistY = rcParent.bottom - rcWnd.bottom;
		}
	}
}

int CMultiframePanel::OnCreate(LPVOID s)
{
	int nRet = __super::OnCreate(s);

	

	return nRet;
}

void CMultiframePanel::OnSize(UINT nType, SOUI::CSize size)
{
	__super::OnSize(nType, size);
	
}

void CMultiframePanel::OnUpdateFloatPosition(const SOUI::CRect & rcParent)
{
	SOUI::CSize sz(GetLayoutParam()->GetSpecifiedSize(Horz),GetLayoutParam()->GetSpecifiedSize(Vert));
	SOUI::CRect rcWnd(SOUI::CPoint(),sz);


	if ( !m_bCustom )
	{
		//¿¿ÉÏ¾ÓÖÐ
		rcWnd.MoveToX(rcParent.left+rcParent.Width() / 2 - rcWnd.Width() / 2);

		rcWnd.MoveToY(rcParent.top );
		Move(rcWnd);
		return ;
	}

	if(m_nFloatHAlign & HALIGN_LEFT)
	{
		rcWnd.MoveToX(rcParent.left+m_nDistX);
	}else if(m_nFloatHAlign & HALIGN_RIGHT)
	{
		rcWnd.MoveToX(rcParent.right - rcWnd.Width() - m_nDistX);
	}
	if(m_nFloatVAlign & VALIGN_TOP)
	{
		rcWnd.MoveToY(rcParent.top + m_nDistY);
	}else if(m_nFloatVAlign & VALIGN_BOTTOM)
	{
		rcWnd.MoveToY(rcParent.bottom - rcWnd.Height() -m_nDistY);
	}



	Move(rcWnd);

	BOOL bTopLeft = rcParent.PtInRect(rcWnd.TopLeft()) ;
	BOOL bBottomRight= rcParent.PtInRect(rcWnd.BottomRight());


	//if ( bTopLeft && bBottomRight )

	SStringT s;
	s.Format(_T("bTopLeft=%d  bBottomRight=%d \r\n"), bTopLeft, bBottomRight);
	OutputDebugString(s);
}
