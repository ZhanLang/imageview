#include "StdAfx.h"
#include "SettingDlg.h"
#include <msapi/shellext.h>
#include <util/registry.h>
#include "msapi/mswinapi.h"

BOOL SetDefProgram(LPCTSTR lpszPord, LPCTSTR lpszProg, LPCTSTR lpszExt, LPCTSTR lpszDes, LPCTSTR lpszParme , LPCTSTR lpszIco)
{
	CRegistry().SetString(HKEY_CURRENT_USER, SStringT(_T("Software\\Classes\\")) + SStringT(lpszPord) + SStringT(lpszExt), _T(""), lpszDes);
	CRegistry().SetString(HKEY_CURRENT_USER, _T("Software\\Classes\\") +SStringT(lpszPord) + SStringT(lpszExt) +SStringT(_T("\\DefaultIcon")), _T(""), lpszIco);

	SStringT strCmd; strCmd.Format(_T("\"%s\" %s \"%%1\""), lpszProg, lpszParme);
	CRegistry().SetString(HKEY_CURRENT_USER, _T("Software\\Classes\\") +SStringT(lpszPord) + SStringT(lpszExt) +SStringT(_T("\\shell\\open\\command")), _T(""), strCmd);

	CRegistry().SetString(HKEY_CURRENT_USER, SStringT(_T("Software\\")) + SStringT(lpszExt), _T(""), SStringT(lpszPord) + SStringT(lpszExt));
	CRegistry().SetString(HKEY_CURRENT_USER, SStringT(_T("Software\\Classes\\"))+ SStringT(lpszExt), _T(""), SStringT(lpszPord) + SStringT(lpszExt));
	return TRUE;
}


struct ExtIco
{
	LPCTSTR lpszExt;
	LPCTSTR lpszIco;
	BOOL bCheck;
};


ExtIco extIco[] = 
{
	{_T(".BMP"),	_T("BMP.ico"), TRUE},
	{_T(".WBMP"), _T("COMMON.ico"), TRUE},
	{_T(".JPG"), _T("JPG.ico"), TRUE},
	{_T(".JPEG"), _T("JPG.ico"), TRUE},
	{_T(".JPE"), _T("JPG.ico"), TRUE},
	{_T(".PNG"), _T("PNG.ico"), TRUE},
	{_T(".JP2"), _T("JPG.ico"), TRUE},
	{_T(".PSD"), _T("PSD.ico"), TRUE},
	{_T(".GIF"), _T("GIF.ico"), TRUE},
	{_T(".DDS"), _T("COMMON.ico"), TRUE},
	{_T(".PCX"), _T("COMMON.ico"), TRUE},
	{_T(".XPM"), _T("COMMON.ico"), TRUE},
	{_T(".TGA"), _T("COMMON.ico"), TRUE},
	{_T(".XBM"), _T("COMMON.ico"), TRUE},
	{_T(".TIF"), _T("TIF.ico"), TRUE},
	{_T(".PNM"), _T("COMMON.ico"), TRUE},
	{_T(".SKA"), _T("COMMON.ico"), TRUE},
	{_T(".WEBP"), _T("COMMON.ico"), TRUE},
	{_T(".CUT"), _T("COMMON.ico"), TRUE},
	{_T(".ICO"), _T("COMMON.ico"), TRUE},
	{_T(".HDR"), _T("COMMON.ico"), TRUE},
	{_T(".JNG"), _T("COMMON.ico"), TRUE},
	{_T(".ARW"), _T("COMMON.ico"), TRUE},
	{_T(".TIFF"), _T("COMMON.ico"), TRUE},
	{_T(".KOA"), _T("COMMON.ico"), TRUE},
	{_T(".MNG"), _T("COMMON.ico"), TRUE},
	{_T(".PCD"), _T("COMMON.ico"), TRUE},
	{_T(".RAS"), _T("COMMON.ico"), TRUE},
	{_T(".J2K"), _T("COMMON.ico"), TRUE},
	{_T(".JPC"), _T("COMMON.ico"), TRUE},
	{_T(".IFF"), _T("COMMON.ico"), TRUE},
	{_T(".G3"), _T("COMMON.ico"), TRUE},
	{_T(".SGI"), _T("COMMON.ico"), TRUE},
	{_T(".EXR"), _T("COMMON.ico"), TRUE},
	{_T(".WDP"), _T("COMMON.ico"), TRUE},
	{_T(".PFM"), _T("COMMON.ico"), TRUE},
	{_T(".PCT"), _T("COMMON.ico"), TRUE},
	{_T(".RAW"), _T("COMMON.ico"), TRUE},
};

CSettingDlg::CSettingDlg(IMscomRunningObjectTable* pRot):SHostDialog(_T("XML_DLG_SETTING"))
{
	m_pRot = pRot;
}


CSettingDlg::~CSettingDlg(void)
{
}

VOID CSettingDlg::OnSelectAll()
{
	for ( INT nLoop = 0 ; nLoop < _countof(extIco) ; nLoop++)
	{
		LPCTSTR lpszName = extIco[nLoop].lpszExt;
		SCheckBox* checkBox = FindChildByName2<SCheckBox>(lpszName);
		extIco[nLoop].bCheck = TRUE;
		checkBox->SetCheck(TRUE);
	}
}

VOID CSettingDlg::OnUnSelectAll()
{
	for ( INT nLoop = 0 ; nLoop < _countof(extIco) ; nLoop++)
	{
		LPCTSTR lpszName = extIco[nLoop].lpszExt;
		SCheckBox* checkBox = FindChildByName2<SCheckBox>(lpszName);
		if( checkBox )
		{
			extIco[nLoop].bCheck = FALSE;
			checkBox->SetCheck(FALSE);
		}
		
	}
}

VOID CSettingDlg::OnOk()
{
	OnApply();
	__super::OnOK();
}

VOID CSettingDlg::OnCancel()
{
	__super::OnCancel();
}

bool ImproveCurrentPurview()
{
	HANDLE hToken;
	if(OpenProcessToken(GetCurrentProcess(),TOKEN_ADJUST_PRIVILEGES,&hToken))
	{
		LUID myLUID;
		LookupPrivilegeValue(NULL,SE_DEBUG_NAME, &myLUID);
		TOKEN_PRIVILEGES tp={sizeof(tp)};
		tp.PrivilegeCount=1;
		tp.Privileges[0].Luid=myLUID;
		tp.Privileges[0].Attributes=SE_PRIVILEGE_ENABLED;
		return AdjustTokenPrivileges(hToken,FALSE,&tp,sizeof(tp),NULL,NULL) != FALSE;
	}
	return false;
}
VOID CSettingDlg::OnApply()
{
	ImproveCurrentPurview();
	
	TCHAR szModulePath[ MAX_PATH ] = { 0 };
	GetModuleFileName( NULL, szModulePath, MAX_PATH);

	SStringT strCurPath;
	msapi::GetCurrentPath(strCurPath.GetBufferSetLength(MAX_PATH), MAX_PATH);
	strCurPath.ReleaseBuffer();

	for ( INT nLoop = 0 ; nLoop < _countof(extIco) ; nLoop++)
	{
		LPCTSTR lpszName = extIco[nLoop].lpszExt;
		SCheckBox* checkBox = FindChildByName2<SCheckBox>(lpszName);
		if (checkBox && checkBox->IsChecked() )
		{
			SetDefProgram(
				GetEnvParamString("productname"),
				szModulePath,
				lpszName,
				GetEnvParamString("friendprodname"),
				_T(""),
				strCurPath + _T("\\icon\\") + extIco[nLoop].lpszIco);
		}
	}

	SHChangeNotify(SHCNE_ASSOCCHANGED, SHCNF_DWORD | SHCNF_FLUSH, NULL, NULL);
	
}

VOID CSettingDlg::OnUpdate()
{
	/*
	msapi::ShellProcess(_T("update.exe"), NULL, TRUE);
	*/
}

BOOL CSettingDlg::OnInitDialog(HWND wndFocus, LPARAM lInitParam)
{
	SStatic* sVer = FindChildByName2<SStatic>(L"version");
	if(sVer )
	{
		sVer->SetWindowText(GetEnvParamString("version"));
	}

	return TRUE;
}
