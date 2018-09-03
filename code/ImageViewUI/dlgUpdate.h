#pragma once

#include "SHostDialogEx.h"
#include "thread\threadex.h"

struct UPDATE_INFO
{
	UPDATE_INFO():bForce(false)
	{

	}

	SStringT strVersion;
	SStringT strUrl;
	SStringT strParam;
	bool	 bForce;
	SStringT strContent;
	SStringT strWebHost;
	SStringT strLocVer;
	SStringT UpdateHost;
};

class dlgUpdate : public SHostDialogEx
{
public:
	dlgUpdate(UPDATE_INFO updateInfo);
	~dlgUpdate(void);
	EVENT_MAP_BEGIN()
		EVENT_NAME_COMMAND(L"btnUpdate", OnBtnUpdate)
		EVENT_NAME_COMMAND(L"btnOk", OnBtnOk)
		EVENT_NAME_COMMAND(L"btnCancel", OnBtnCancel)
		EVENT_NAME_COMMAND(L"skipCheck", OnBtnSkip)
	EVENT_MAP_END()

	BEGIN_MSG_MAP_EX(CMainDlg)
		MSG_WM_INITDIALOG(OnInitDialog)
	CHAIN_MSG_MAP(SHostDialogEx)
	REFLECT_NOTIFICATIONS_EX()
	END_MSG_MAP()

	BOOL OnInitDialog(HWND wndFocus, LPARAM lInitParam);
	static VOID SkinUpdate( LPCTSTR lpszVer );
	static BOOL IsSkip(LPCTSTR lpszVer);

private:
	VOID OnBtnUpdate();
	VOID OnBtnOk();
	VOID OnBtnCancel();
	VOID OnBtnSkip();
	public:
	VOID OnDownload( INT Percent);
private:
	BOOL DoUpdate();
private:
	UPDATE_INFO m_updateInfo;
	CThreadEx m_downThread;
};

