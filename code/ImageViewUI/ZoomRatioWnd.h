#pragma once
class CZoomRatioWnd : public SWindow
{
public:
	SOUI_CLASS_NAME(CZoomRatioWnd,L"zoomratio");

	SOUI_MSG_MAP_BEGIN()
		MSG_WM_TIMER(OnTimer)
	SOUI_MSG_MAP_END()
public:
	CZoomRatioWnd(void);
	~CZoomRatioWnd(void);

	VOID SetZoomRatio( DWORD dwRatio);

protected:
	void OnTimer(char cTimerID);
protected: //SWindow
	virtual void UpdateChildrenPosition();

private:
	SStatic* m_TxtZoomRatio;
};

