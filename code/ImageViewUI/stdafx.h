// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             //  �� Windows ͷ�ļ����ų�����ʹ�õ���Ϣ

// Windows ͷ�ļ�:
#include <windows.h>



#include <atlbase.h>	// ������ATL��
#include <atlstr.h>
#include <atltypes.h>
#include <atlapp.h>		// ������WTL��


#include <atlwin.h>		// ATL������
#include <atlcrack.h>	// WTL��ǿ����Ϣ��
//#include <atlsplit.h>
#include <atlframe.h>	// WTL����ܴ�����
//#include <atlgdi.h>
#include <atlctrls.h>
//#include <atlctrlx.h>
#include <atldlgs.h>
#include <atlmisc.h>	// WTLʵ�ù�����


// TODO: �ڴ˴����ó�����Ҫ������ͷ�ļ�
#define  _CRT_SECURE_NO_WARNINGS
#define	 DLL_SOUI
#include <souistd.h>
#include <core/SHostDialog.h>
#include <control/SMessageBox.h>
#include <control/souictrls.h>
#include <res.mgr/sobjdefattr.h>
#include <com-cfg.h>

#include <atlconv.h>



#include "resource.h"
#include "res\resource.h"
using namespace SOUI;

#include <mscom/mscominc.h>
using namespace mscom;

#define GroupName _T("ImageViewUI")
#define MODULE_NAME GroupName
#include <mslog/stdlog_dll.h>
#include <mslog/msdkoutput.h>
#include <shlobj.h>

#define AppName _T("ImageView")