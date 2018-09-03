// SettingTools.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "SettingTools.h"

#include <msapi/shellext.h>
#include <util/registry.h>
#include "msapi/mswinapi.h"

BOOL SetDefProgram(LPCTSTR lpszPord, LPCTSTR lpszProg, LPCTSTR lpszExt, LPCTSTR lpszDes, LPCTSTR lpszParme , LPCTSTR lpszIco)
{
	CRegistry().SetString(HKEY_CURRENT_USER, CString(_T("Software\\Classes\\")) + CString(lpszPord) + CString(lpszExt), _T(""), lpszDes);
	CRegistry().SetString(HKEY_CURRENT_USER, _T("Software\\Classes\\") +CString(lpszPord) + CString(lpszExt) +CString(_T("\\DefaultIcon")), _T(""), lpszIco);

	CString strCmd; strCmd.Format(_T("\"%s\" %s \"%%1\""), lpszProg, lpszParme);
	CRegistry().SetString(HKEY_CURRENT_USER, _T("Software\\Classes\\") +CString(lpszPord) + CString(lpszExt) +CString(_T("\\shell\\open\\command")), _T(""), strCmd);

	CRegistry().SetString(HKEY_CURRENT_USER, CString(_T("Software\\")) + CString(lpszExt), _T(""), CString(lpszPord) + CString(lpszExt));
	CRegistry().SetString(HKEY_CURRENT_USER, CString(_T("Software\\Classes\\"))+ CString(lpszExt), _T(""), CString(lpszPord) + CString(lpszExt));
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


int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	CString strImageViewPath;
	msapi::GetCurrentPath(strImageViewPath.GetBufferSetLength(MAX_PATH), MAX_PATH);
	strImageViewPath.ReleaseBuffer();
	strImageViewPath.Append(L"\\ImageView.exe");


	CString strCurPath;
	msapi::GetCurrentPath(strCurPath.GetBufferSetLength(MAX_PATH), MAX_PATH);
	strCurPath.ReleaseBuffer();

	for ( int n = 0 ; n <_countof(extIco) ; n++)
	{
		SetDefProgram(
			L"快眼看图王",
			strImageViewPath,
			extIco[n].lpszExt,
			L"快眼看图王",
			_T(""),
			strCurPath + _T("\\icon\\") + extIco[n].lpszIco);
	}


	CRegistry().SetString(HKEY_CURRENT_USER, L"Directory\\shell\\ImageView", L"", L"快眼看图王-穿透文件夹");
	CRegistry().SetString(HKEY_CURRENT_USER, L"Directory\\shell\\ImageView", L"Icon", strImageViewPath);

	CString strCmd;
	strCmd.Format(L"\"%s\" \"%%1\" ", strImageViewPath));
	CRegistry().SetString(HKEY_CURRENT_USER, L"Directory\\shell\\ImageView\\command", L"", strCmd);

	return 0;
}