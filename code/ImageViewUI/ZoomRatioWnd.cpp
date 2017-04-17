#include "StdAfx.h"
#include "ZoomRatioWnd.h"

#define EVENT_ZOOM_RATIO_TIMER 10
CZoomRatioWnd::CZoomRatioWnd(void)
{
	m_TxtZoomRatio = NULL;
}


CZoomRatioWnd::~CZoomRatioWnd(void)
{
}


VOID CZoomRatioWnd::SetZoomRatio(DWORD dwRatio)
{
	SStringT sText;
		if ( dwRatio == -1 )
		sText.Format(GETSTRING(L"@string/zoom_max"));
	else if ( dwRatio == 0 )
		sText.Format(GETSTRING(L"@string/zoom_min"));
	else
	sText.Format(_T("%d%%"), dwRatio);



	if ( m_TxtZoomRatio )
		m_TxtZoomRatio->SetWindowText(sText);

	this->SetVisible(TRUE, TRUE);
	KillTimer( EVENT_ZOOM_RATIO_TIMER );
	SetTimer(EVENT_ZOOM_RATIO_TIMER, 500 );
}

void CZoomRatioWnd::OnTimer(char cTimerID)
{
	this->SetVisible(FALSE, TRUE);
}

void CZoomRatioWnd::UpdateChildrenPosition()
{
	if ( !m_TxtZoomRatio )
		m_TxtZoomRatio = FindChildByName2<SStatic>(L"txt_zoomratio");
	
	return __super::UpdateChildrenPosition();
}
