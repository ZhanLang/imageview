#include "StdAfx.h"
#include "ImageView.h"


CImageView::CImageView(void)
{
	m_evtSet.addEvent(EVENTID(EventImageViewMouseWhell));
}


CImageView::~CImageView(void)
{
}

BOOL CImageView::OnMouseWheel(UINT nFlags, short zDelta, SOUI::CPoint pt)
{
	EventImageViewMouseWhell evtHover(this);
	evtHover.point = pt;
	evtHover.zDelta = zDelta;
	FireEvent(evtHover);
	return __super::OnMouseWheel(nFlags, zDelta, pt);
}
