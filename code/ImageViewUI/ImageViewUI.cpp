// JBktUI.cpp : ���� DLL Ӧ�ó���ĵ���������
//

#include "stdafx.h"
#include "ImageViewUI.h"

#include <userguid/userguid.h>
#include <network/HttpImplement.h>
#include <resprovider-zip/zipresprovider-param.h>
#include "..\controls.extend\gif\SGifPlayer.h"
#include "SRotateWindow.h"
#include "SShowGifWnd.h"
#include "..\controls.extend\SFreeMoveWindow.h"

#include<ximage.h>
#include "ImageView.h"
#include "MultiframePanel.h"
#include "ZoomRatioWnd.h"
#include "SouiRealWndHandler.h"
#include "msapi/mstime.h"
#include "msapi/msapp.h"
#include <msapi/EnumAntivirus.h>
CImageViewUI::CImageViewUI(void)
{
	m_nSkin = SKIN_DEFAULT;
}


CImageViewUI::~CImageViewUI(void)
{
}

VOID CImageViewUI::DoReportActive()
{
	msdk::network::CUrlParamValueMap ParamValueMap;
	msapi::CApp m_App(CMsComBase::GetEnvParamString("productname"));
	ParamValueMap["apiver"]			=		1;					//��ǰAPI�汾
	ParamValueMap["hdd"]			=		m_App.GetDiskSerial();
	ParamValueMap["uguid"]			=		m_App.GetUserGuid();
	ParamValueMap["mac"]			=		CGetNetAdapterInfo::GetMac();
	ParamValueMap["osver"]			=		msapi::GetMicroVersion();
	ParamValueMap["wow64"]			=		msapi::IsWindowsX64();
	ParamValueMap["ant"]			=		msapi::CEnumAntivirus().Enum();
	ParamValueMap["prod"]			=		m_App.GetSetting(_T("prod"),0);
	ParamValueMap["packet"]			=		m_App.GetSetting(_T("packet"),0);
	ParamValueMap["subprod"]		=		0;	//��ǰģ����Ӳ�Ʒ
	ParamValueMap["idate"]			=		m_App.GetSetting(_T("idate"), msapi::GetCurrentDateStr(FALSE));
	ParamValueMap["itime"]			=		m_App.GetSetting(_T("itime"), msapi::GetCurrentTimeStr(FALSE));
	ParamValueMap["prodver"]		=		m_App.GetVersion();
	ParamValueMap["oper"]			=		m_App.GetSetting(_T("oper"), 0);
	ParamValueMap["random"]			=		GetTickCount();
	msdk::network::CHttpImplement HttpImpl;
	HttpImpl.GetRequest(UPDATE_URL, 80, "active" , ParamValueMap);
}


HRESULT CImageViewUI::Init(void*)
{
	m_pComMgr = new SComMgr;
	CxImage* newImage = new CxImage();
	BOOL bLoaded=FALSE;
	CAutoRefPtr<SOUI::IImgDecoderFactory> pImgDecoderFactory;
	CAutoRefPtr<SOUI::IRenderFactory> pRenderFactory;
	bLoaded = m_pComMgr->CreateRender_GDI((IObjRef**)&pRenderFactory);
	SASSERT_FMT(bLoaded,_T("load interface [render] failed!"));
	bLoaded=m_pComMgr->CreateImgDecoder((IObjRef**)&pImgDecoderFactory);
	SASSERT_FMT(bLoaded,_T("load interface [%s] failed!"),_T("imgdecoder"));

	pRenderFactory->SetImgDecoderFactory(pImgDecoderFactory);
	m_pTheApp = new SApplication(pRenderFactory, GetModuleInstance());
	{
		CAutoRefPtr<IResProvider> sysResProvider;
		CreateResProvider(RES_PE, (IObjRef**)&sysResProvider);
		sysResProvider->Init((WPARAM)GetModuleInstance(), 0);
		m_pTheApp->LoadSystemNamedResource(sysResProvider);
	}


	CSouiRealWndHandler* pRealWndHander = new CSouiRealWndHandler;
	m_pTheApp->SetRealWndHandler(pRealWndHander);
	pRealWndHander->Release();

	m_pTheApp->RegisterWindowClass<SRotateWindow>();
	m_pTheApp->RegisterWindowClass<CImageCanvas>();
	m_pTheApp->RegisterWindowClass<CMultiframePanel>();
	m_pTheApp->RegisterWindowClass<SShowGifWnd>();
	m_pTheApp->RegisterWindowClass<SFreeMoveWindow>();
	m_pTheApp->RegisterWindowClass<CImageView>();
	m_pTheApp->RegisterWindowClass<CZoomRatioWnd>();
	m_pTheApp->RegisterWindowClass<SGifPlayer>();//theApp�����ӷ�����RegisterWindowClass,�滻RegisterWndFactory(TplSWindowFactory<SGifPlayer>())
	m_pTheApp->RegisterSkinClass<SSkinGif>();//ע��SkinGif

	CAutoRefPtr<IResProvider>   pResProvider;
#ifdef DEBUG
	CreateResProvider(RES_FILE, (IObjRef**)&pResProvider);

	SStringT sSkin;
	msapi::GetCurrentPath(sSkin.GetBufferSetLength(MAX_PATH), MAX_PATH);
	sSkin.ReleaseBuffer();
	sSkin.Append(_T("\\skin\\imageview"));


	if (!pResProvider->Init((LPARAM)sSkin.GetBuffer(0), 0))
	{
		SASSERT(0);
		return E_FAIL;
	}
#else
	ZIPRES_PARAM zipResParam;
	zipResParam.pRenderFac = pRenderFactory;
	zipResParam.type = ZIPRES_PARAM::ZIPFILE;

	SStringT sSkin;
	msapi::GetCurrentPath(sSkin.GetBufferSetLength(MAX_PATH), MAX_PATH);
	sSkin.ReleaseBuffer();
	sSkin.Append(_T("\\skin\\imageview.zip"));
	zipResParam.pszZipFile =sSkin ;
	zipResParam.pszPsw = ("CreateFile");
	m_pComMgr->CreateResProvider_ZIP((IObjRef**)&pResProvider);
	if (!pResProvider->Init((LPARAM)&zipResParam, 0))
	{
		SASSERT(0);
		return E_FAIL;
	}
#endif



	m_pTheApp->AddResProvider(pResProvider);


	m_pDlgMain = new CMainDlg( m_pRot );
	m_pDlgMain->Create(NULL, (DWORD)WS_POPUP, (DWORD)00);
	BOOL bRet = m_pDlgMain->SendMessage(WM_INITDIALOG);
	m_pDlgMain->CenterWindow(m_pDlgMain->m_hWnd);
	m_pDlgMain->ShowWindow(SW_SHOWNORMAL);



	//��ʼ��
	if ( m_pDlgMain->IsWindow() )
	{

	}

	CThreadEx().Do([this]{DoReportActive();});
	return S_OK;
}

HRESULT CImageViewUI::Uninit()
{
	if ( m_pDlgMain )
	{
		delete m_pDlgMain;
		m_pDlgMain = NULL;
	}
	if ( m_pTheApp )
	{
		delete m_pTheApp;
		m_pTheApp = NULL;
	}

	if ( m_pComMgr )
	{
		delete m_pComMgr;
		m_pComMgr = NULL;
	}

	return S_OK;
}

HRESULT CImageViewUI::Start()
{
	m_pTheApp->Run(m_pDlgMain->m_hWnd);
	return S_OK;
}

HRESULT CImageViewUI::Stop()
{
	return S_OK;
}

