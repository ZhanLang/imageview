// dllmain.cpp : ���� DLL Ӧ�ó������ڵ㡣
#include "stdafx.h"
#include "ImageViewUI.h"
USE_DEFAULT_DLL_MAIN;//��Ҫ�滻ԭ����DllMain
BEGIN_CLIDMAP
	CLIDMAPENTRY_BEGIN
	CLIDMAPENTRY(CLSID_ImageViewUI,CImageViewUI)
	CLIDMAPENTRY_END
	END_CLIDMAP_AND_EXPORTFUN;
