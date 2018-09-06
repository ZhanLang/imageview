#pragma once
#include "SFreeMoveWindow1.h"
#include "ImageCanvas.h"
class CMultiframePanel : public SFreeMoveWindow
{
public:
	CMultiframePanel(void);
	~CMultiframePanel(void);

	SOUI_CLASS_NAME(CImageCanvas,L"multiframepanel");

	SOUI_MSG_MAP_BEGIN()
		MSG_WM_LBUTTONDOWN(OnLButtonDown)
		MSG_WM_LBUTTONUP(OnLButtonUp)
		MSG_WM_MOUSEMOVE(OnMouseMove)
		MSG_WM_CREATE(OnCreate)
		MSG_WM_SIZE(OnSize)
	SOUI_MSG_MAP_END()

	void OnLButtonDown(UINT nFlags,SOUI::CPoint pt);
	void OnLButtonUp(UINT nFlags,SOUI::CPoint pt);
	void OnMouseMove(UINT nFlags,SOUI::CPoint pt);
	int OnCreate(LPVOID);
	void OnSize( UINT nType, SOUI::CSize size );

protected: //SFreeMoveWindow
	virtual void OnUpdateFloatPosition(const SOUI::CRect & rcParent);

private:
	BOOL m_bCustom;
};

