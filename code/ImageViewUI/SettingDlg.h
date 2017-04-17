#pragma once
#include <core/shostdialog.h>
class CSettingDlg :
	public SHostDialog,public CMsComBase<CSettingDlg>
{
public:
	CSettingDlg(IMscomRunningObjectTable* pRot);
	~CSettingDlg(void);
	EVENT_MAP_BEGIN()
		EVENT_NAME_COMMAND(L"select_all", OnSelectAll)
		EVENT_NAME_COMMAND(L"unselect_all", OnUnSelectAll)
		EVENT_NAME_COMMAND(L"ok", OnOk)
		EVENT_NAME_COMMAND(L"cancel", OnCancel)
		EVENT_NAME_COMMAND(L"apply", OnApply)
		EVENT_NAME_COMMAND(L"check_update", OnUpdate)
	EVENT_MAP_END()


	BEGIN_MSG_MAP_EX(CSettingDlg)
		MSG_WM_INITDIALOG(OnInitDialog)
		MSG_WM_KEYDOWN(OnKeyDown)
		CHAIN_MSG_MAP(SHostWnd)
		REFLECT_NOTIFICATIONS_EX()
	END_MSG_MAP()

	VOID OnSelectAll();
	VOID OnUnSelectAll();
	VOID OnOk();
	VOID OnCancel();
	VOID OnApply();
	VOID OnUpdate();

protected:
	BOOL OnInitDialog(HWND wndFocus, LPARAM lInitParam);

private:
};

