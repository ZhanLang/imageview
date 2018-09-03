#pragma once
#include "SHostWndEx.h"
class SHostDialogEx : public SHostWndEx
{
public:
	SOUI_CLASS_NAME(SHostDialogEx,L"hostdlgex")
public:
	SHostDialogEx(LPCTSTR pszXmlName);
	~SHostDialogEx(void);

	virtual INT_PTR DoModal(HWND hParent=NULL);

	virtual void EndDialog(INT_PTR nResult);

protected:
	void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	void OnOK();
	void OnCancel();
	virtual SMessageLoop * GetMsgLoop(){return m_MsgLoop;}

	EVENT_MAP_BEGIN()
		EVENT_ID_COMMAND(IDOK,OnOK)
		EVENT_ID_COMMAND(IDCANCEL,OnCancel)
		EVENT_MAP_END()

		BEGIN_MSG_MAP_EX(SHostDialog)
		MSG_WM_CLOSE(OnCancel)
		MSG_WM_KEYDOWN(OnKeyDown)
		CHAIN_MSG_MAP(SHostWndEx)
		REFLECT_NOTIFICATIONS_EX()
		END_MSG_MAP()

		INT_PTR m_nRetCode;

		SMessageLoop * m_MsgLoop;
};

