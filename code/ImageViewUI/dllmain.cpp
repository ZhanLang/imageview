// dllmain.cpp : ���� DLL Ӧ�ó������ڵ㡣
#include "stdafx.h"
#include "ImageViewUI.h"

HINSTANCE g_hinstance = NULL;
int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPTSTR lpstrCmdLine, int /*nCmdShow*/)
{
	g_hinstance = hInstance;
	CImageViewUI ImageViewUI;
	if ( ImageViewUI.Init(NULL) == S_OK)
		ImageViewUI.Start();
	
	ImageViewUI.Stop();
	ImageViewUI.Uninit();

	return 0;
}
