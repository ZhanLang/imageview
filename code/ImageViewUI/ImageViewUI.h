#pragma once

#include <iimageviewui.h>
#include <mscom/SrvBaseImp.h>

#include "MainDlg.h"

enum SKIN_MODE
{
	SKIN_SIMPLE,
	SKIN_DEFAULT,
};

class CImageViewUI: public IImageViewUI, public IMsPlugin, public IMsPluginRun, public CMsComBase<CImageViewUI>
{
public:
	UNKNOWN_IMP3_( IMsPlugin , IMsPluginRun,IImageViewUI);
	CImageViewUI(void);
	~CImageViewUI(void);

	STDMETHOD(Init)(void*);
	STDMETHOD(Uninit)();
	STDMETHOD(Start)();
	STDMETHOD(Stop)();
	
private:
	VOID DoReportActive();
private:
	SComMgr *m_pComMgr;
	SApplication *m_pTheApp;

	CMainDlg* m_pDlgMain;
	int m_nSkin;
};

