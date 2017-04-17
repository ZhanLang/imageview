// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             //  从 Windows 头文件中排除极少使用的信息

// Windows 头文件:
#include <windows.h>



#include <atlbase.h>	// 基本的ATL类
#include <atlstr.h>
#include <atltypes.h>
#include <atlapp.h>		// 基本的WTL类


#include <atlwin.h>		// ATL窗口类
#include <atlcrack.h>	// WTL增强的消息宏
//#include <atlsplit.h>
#include <atlframe.h>	// WTL主框架窗口类
//#include <atlgdi.h>
#include <atlctrls.h>
//#include <atlctrlx.h>
#include <atldlgs.h>
#include <atlmisc.h>	// WTL实用工具类


// TODO: 在此处引用程序需要的其他头文件
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