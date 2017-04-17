// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             //  �� Windows ͷ�ļ����ų�����ʹ�õ���Ϣ
// Windows ͷ�ļ�:
#include <windows.h>

#include <mscom/mscominc.h>
using namespace mscom;

#include <mslog/stdlog_dll.h>
#define GroupName _T("VCDRomUI")

#include <DuiLib/UIlib.h>
using namespace DuiLib;

#include "resource.h"

#include <atlstr.h>
// TODO: �ڴ˴����ó�����Ҫ������ͷ�ļ�

inline DuiLib::CControlUI* CreateControlElement(LPCTSTR lpXmlText, DuiLib::IDialogBuilderCallback* pCallback, DuiLib::CPaintManagerUI* pManager)
{
	CDuiString strHeader, strEnd;
	strHeader.Format(_T("%s"), _T("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n<Window>\r\n"));
	strEnd.Format(_T("%s"), _T("</Window>"));
	DuiLib::CDialogBuilder builder;

	CDuiString strTemp = lpXmlText;
	strTemp = strHeader+strTemp+strEnd;
	lpXmlText = (LPCTSTR)strTemp;
	return builder.Create(lpXmlText, (UINT)0,  pCallback, pManager);
}


// TODO: �ڴ˴����ó�����Ҫ������ͷ�ļ�
