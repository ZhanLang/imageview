#pragma once

class  EventImageViewMouseWhell : public TplEventArgs<EventImageViewMouseWhell>
{
	SOUI_CLASS_NAME(EventImageViewMouseWhell,L"on_eventimageview_mousewhell")
public:
	EventImageViewMouseWhell(SObject *pSender):TplEventArgs<EventImageViewMouseWhell>(pSender){}
	enum{EventID=EVT_EXTERNAL_BEGIN + 200};
	SOUI::CPoint point;
	short zDelta;
};


class CImageView :
	public SWindow
{
public:
	CImageView(void);
	~CImageView(void);

	SOUI_CLASS_NAME(CImageView,L"imageview")
	SOUI_MSG_MAP_BEGIN()
		MSG_WM_MOUSEWHEEL(OnMouseWheel)
		
	SOUI_MSG_MAP_END()

protected:
	 BOOL OnMouseWheel(UINT nFlags, short zDelta, SOUI::CPoint pt);
};

