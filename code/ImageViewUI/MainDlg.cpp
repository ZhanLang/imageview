// MainDlg.cpp : implementation of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MainDlg.h"
#include <msapi/mspath.h>
#include <UTIL/cconv.h>
#include <arith/AES.h>
#ifdef DWMBLUR	//win7毛玻璃开关
#include <dwmapi.h>
#pragma comment(lib,"dwmapi.lib")
#endif

#include "resource.h"
#include <util/vercmp.h>
#include <shellapi.h>
#include <thread/threadex.h>
#include <network/HttpImplement.h>
#include <msapi/msapp.h>
#include <json/json.h>
#include <msapi/msClipboard.h>
#include <msapi/EnumAntivirus.h>
#include <msapi/mssrv.h>
#include <msapi/msapp.h>
#include <process/ParseCommand.h>

#include <Commctrl.h>
#include "ImageCanvas.h"
#include <commdlg.h>
#include "DropTargetImage.h"
#include "../../include/iimageviewui.h"
#include "ImageViewUI.h"
#include "SettingDlg.h"
#include <atlconv.h>
#include "msapi/mstime.h"





SStringT GetFileTypes(BOOL bOpenFileDialog)
{
	SStringT str;
	for (int i=0;i<CMAX_IMAGE_FORMATS;i++){
		if (bOpenFileDialog && doctypes[i].bRead){
			str += doctypes[i].description ;
			str+= L"(" ;
			str += doctypes[i].ext;
			str += L")";
			str += (TCHAR)'\0';
			str += doctypes[i].ext;
			str += (TCHAR)'\0';
		} else if (!bOpenFileDialog && doctypes[i].bWrite) {
			str += doctypes[i].description;
			str += (TCHAR)'\0';
			str += doctypes[i].ext;
			str += (TCHAR)'\0';
		}
	}
	return str;
}

CMainDlg::CMainDlg(IMscomRunningObjectTable* pRot) : SHostWnd(_T("LAYOUT:XML_MAINWND"))
{
	m_bLayoutInited = FALSE;
	m_pRot = pRot;
	m_tableCtrl = NULL;
	m_Caption = NULL;
	m_bFitSize = TRUE;
	m_fScale = 1.0f;
	m_PanelLeft = NULL;
	m_PanelRight = NULL;
	m_PanelBar = NULL;

	m_CtrlBar = NULL;
	m_BtnBarNext = NULL;
	m_BtnBarPrev = NULL;
	m_bOpenFile = FALSE;

	m_MultiframePrev = NULL;
	m_MultiframePlay = NULL;
	m_MultiframePause = NULL;
	m_MultiframeNext = NULL;
	btn_multiframe_save = NULL;
	btn_multiframe_saveall = NULL;
	m_bStopEnumFile = FALSE;

}

CMainDlg::~CMainDlg()
{
	
}

int CMainDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	#ifdef DWMBLUR	//win7毛玻璃开关
	MARGINS mar = {5,5,30,5};
	DwmExtendFrameIntoClientArea ( m_hWnd, &mar );
	#endif

	//SetIcon(IDI_ICON_MAIN);
	
	SetMsgHandled(FALSE);

	return 0;
}

void CMainDlg::SetIcon(UINT nRes)
{
	HICON hIcon = (HICON)::LoadImage(NULL, MAKEINTRESOURCE(nRes), IMAGE_ICON,
		(::GetSystemMetrics(SM_CXICON) + 15) & ~15, (::GetSystemMetrics(SM_CYICON) + 15) & ~15,	// 防止高DPI下图标模糊
		LR_DEFAULTCOLOR);
	
	::SendMessage(m_hWnd, WM_SETICON, (WPARAM) TRUE, (LPARAM) hIcon);


	hIcon = (HICON)::LoadImage(g_hinstance, MAKEINTRESOURCE(nRes), IMAGE_ICON,
		(::GetSystemMetrics(SM_CXICON) + 15) & ~15, (::GetSystemMetrics(SM_CYICON) + 15) & ~15,	// 防止高DPI下图标模糊
		LR_DEFAULTCOLOR);

	::SendMessage(m_hWnd, WM_SETICON, (WPARAM) FALSE, (LPARAM) hIcon);
}

VOID CMainDlg::OpenFile(LPCTSTR lpszPath)
{
	if ( GetFileAttributes(lpszPath) & FILE_ATTRIBUTE_DIRECTORY)
		return ;

	LPCTSTR lpszExt = PathFindExtension( lpszPath );
	if ( !(lpszExt && _tcslen( lpszExt)) )
		return ;

	int nImageType = CxImage::GetTypeIdFromName(lpszExt + 1);
	if ( nImageType == CXIMAGE_FORMAT_UNKNOWN )
		return ;

	OpenImageFile( lpszPath );
	TCHAR szPath[MAX_PATH] = { 0 };
	EnumImageFile(msapi::GetPathFilePath(lpszPath, szPath, MAX_PATH),FALSE);
}

BOOL CMainDlg::OnInitDialog(HWND hWnd, LPARAM lParam)
{
	//SetIcon(IDI_ICON1);
	m_bLayoutInited = TRUE;

	TCHAR strFile[ MAX_PATH ] = { 0 };
	GetModuleFileName( NULL, strFile, MAX_PATH );
	TCHAR szVer[ MAX_PATH ] = { 0 };
	msapi::GetPeFileDetail( strFile, L"080404b0", szVer, MAX_PATH );


	SStringT strSetupIni;
	msapi::GetCurrentPath(strSetupIni.GetBufferSetLength(MAX_PATH), MAX_PATH);
	strSetupIni.ReleaseBuffer();
	strSetupIni.Append(L"\\setting.ini");

	GetPrivateProfileString(L"setting", L"version", szVer, m_strLocalVer.GetBufferSetLength(MAX_PATH), MAX_PATH, strSetupIni);
	m_strLocalVer.ReleaseBuffer();
	m_updateInfo.strLocVer = m_strLocalVer;


	SetSkin(msapi::CApp(AppName).GetSetting(L"skin_id", SKIN_SIMPLE));

	BOOL bTopMoust = msapi::CApp(AppName).GetSetting(L"top_moust", FALSE);
	if ( bTopMoust )
	{
		::SetWindowPos(m_hWnd,HWND_TOPMOST,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
	}

	m_tableCtrl = FindChildByName2<STabCtrl>(L"table");
// 	if( !m_tableCtrl )
// 		return FALSE;

	m_ImageCanvas = FindChildByName2<CImageCanvas>(L"imagecanvas");
	if ( m_ImageCanvas )
	{
		//m_ImageCanvas->SetImageFile(_T("C:\\Users\\endprot\\Desktop\\IMG_5017.jpg"));
		//m_ImageCanvas->SetImageFile(_T("C:\\Users\\endprot\\Desktop\\open_BG.png"));
	}

	m_Caption = FindChildByName2<SCaption>(L"caption");

	m_PanelLeft = FindChildByName2<SWindow>(L"pane_left");
	m_PanelRight = FindChildByName2<SWindow>(L"pane_right");
	m_PanelBar = FindChildByName2<SWindow>(L"pane_bar");

	m_CtrlBar = FindChildByName2<SWindow>(L"ctrl_bar");
	m_BtnBarPrev = FindChildByName2<SImageButton>(L"btn_bar_prev");
	m_BtnBarNext = FindChildByName2<SImageButton>(L"btn_bar_next");


	if ( m_ImageCanvas )
	{
		m_ImageCanvas->GetEventSet()->subscribeEvent(&CMainDlg::OnCanvasSize, this);
		m_ImageCanvas->GetEventSet()->subscribeEvent(&CMainDlg::OnCanvasMouseMove, this);
		m_ImageCanvas->GetEventSet()->subscribeEvent(&CMainDlg::OnCanvasMouseLeave, this);
		m_ImageCanvas->GetEventSet()->subscribeEvent(&CMainDlg::OnCanvasWndMove, this);
		m_ImageCanvas->GetEventSet()->subscribeEvent(&CMainDlg::OnCanvasFrameChange, this);
		m_ImageCanvas->GetEventSet()->subscribeEvent(&CMainDlg::OnCanvasImageLoad, this);
		m_ImageCanvas->GetEventSet()->subscribeEvent(&CMainDlg::OnImageMenu,this);
	}
	
	m_ImageView = FindChildByName2<SWindow>(L"image_view");
	if ( m_ImageView )
	{
		m_ImageView->GetEventSet()->subscribeEvent(&CMainDlg::OnImageViewMouseWhell, this);
		m_ImageView->GetEventSet()->subscribeEvent(&CMainDlg::OnImageMenu, this);
	}


	m_MultiframePrev		= FindChildByName2<SImageButton>(L"btn_multiframe_prev");
	m_MultiframePlay		= FindChildByName2<SImageButton>(L"btn_multiframe_play");
	m_MultiframePause		= FindChildByName2<SImageButton>(L"btn_multiframe_pause");
	m_MultiframeNext		= FindChildByName2<SImageButton>(L"btn_multiframe_next");
	
	m_TextFrameCount = FindChildByName2<SStatic>(L"txt_frame_count");

	btn_multiframe_save		= FindChildByName2<SImageButton>(L"btn_multiframe_save");
	btn_multiframe_saveall	= FindChildByName2<SImageButton>(L"btn_multiframe_saveall");

	m_MutiframPanel = FindChildByName2<CMultiframePanel>(L"multiframe");
	m_ZoomRatioWnd = FindChildByName2<CZoomRatioWnd>(L"zoomratio");
	m_TextTitle	= FindChildByName2<SStatic>(L"text_title");
	BOOL bRet = m_MenuMain.LoadMenu(_T("LAYOUT:menu_main"));					//icon按钮的弹出菜单
	m_MenuSkin.LoadMenu(_T("menu_skin"),_T("LAYOUT"));							//icon按钮的弹出菜单

	m_MenuImage.LoadMenu(_T("LAYOUT:menu_image"));

	OleInitialize(NULL );
	HRESULT hr=::RegisterDragDrop(m_hWnd,new CDropTargetImage(this,m_hWnd));


	int nArgc = 0;
	LPWSTR * cArg = CommandLineToArgvW(GetCommandLineW(), &nArgc);
	if ( nArgc = 2)
	{
		LPCTSTR lpszPath = cArg[1];
		DWORD dwFileAttr = GetFileAttributes(lpszPath);
		if ( dwFileAttr != INVALID_FILE_ATTRIBUTES  )
		{
			if ( dwFileAttr & FILE_ATTRIBUTE_DIRECTORY )
				OpenPath(lpszPath);
			else
				OpenFile(lpszPath);
		}
		
	}

	if ( cArg )
		LocalFree(cArg);
	


	SetBarPaneState();

	DoUpdate();
	CThreadEx().Do([this]{DoReportActive();});
	//CThreadEx().Do([this]{ OnWorkCheckUpdata();});
	return TRUE;
}

SStringT BrowseForFolder( HWND hwnd)
{
	SStringT strFolder;
	if ( !msapi::IsWindowVistaLatter())
	{
		TCHAR szBuffer[MAX_PATH] = {0}; 
		BROWSEINFO bi; 
		ZeroMemory(&bi,sizeof(BROWSEINFO)); 
		bi.hwndOwner = hwnd; 
		bi.pszDisplayName = szBuffer; 
		bi.lpszTitle = _T("从下面选择文件或文件夹:"); 
		bi.ulFlags = BIF_BROWSEINCLUDEFILES; 
		LPITEMIDLIST idl = SHBrowseForFolder(&bi); 
		if (NULL == idl) 
		{ 
			return _T(""); 
		} 
		BOOL bRet = SHGetPathFromIDList(idl,szBuffer);
		if ( bRet )
		{
			strFolder = szBuffer;
			return strFolder;
		}
	}
	else
	{
		HRESULT hr;
		IFileOpenDialog *pOpenFolderDialog;

		// CoCreate the dialog object.
		hr = CoCreateInstance(CLSID_FileOpenDialog, 
			NULL, 
			CLSCTX_INPROC_SERVER,
			IID_PPV_ARGS(&pOpenFolderDialog));

		if (SUCCEEDED(hr))
		{
			pOpenFolderDialog->SetOptions(FOS_PICKFOLDERS);

			// Show the dialog
			hr = pOpenFolderDialog->Show(hwnd);

			if (SUCCEEDED(hr))
			{
				// Obtain the result of the user's interaction with the dialog.
				IShellItem *psiResult;
				hr = pOpenFolderDialog->GetResult(&psiResult);

				if (SUCCEEDED(hr))
				{
					// Do something with the result.
					LPWSTR pwsz = NULL;

					hr = psiResult->GetDisplayName ( SIGDN_FILESYSPATH, &pwsz );

					if ( SUCCEEDED(hr) ) {
						strFolder = pwsz;
						CoTaskMemFree ( pwsz );
					}
					psiResult->Release();
				}
			}
			pOpenFolderDialog->Release();
		}
		return strFolder;
	}
	
	return strFolder;
}

bool DisplaySystemPrintDialogForImage(const std::vector<CString>& files, HWND hwnd) {
	static const CLSID CLSID_PrintPhotosDropTarget ={ 0x60fd46de, 0xf830, 0x4894, { 0xa6, 0x28, 0x6f, 0xa8, 0x1b, 0xc0, 0x19, 0x0d } };
	CComPtr<IShellFolder> desktop; // namespace root for parsing the path
	HRESULT hr = SHGetDesktopFolder(&desktop);
	if (!SUCCEEDED(hr)) {
		return false;
	}
	CComPtr<IShellItem> psi;
	CComPtr<IDataObject> pDataObject;
	std::vector<LPITEMIDLIST> list;
	for ( std::vector<CString>::const_iterator it= files.begin(); it != files.end() ; it++) {
		CString fileName = *it;
		PIDLIST_RELATIVE newPIdL;
		hr = desktop->ParseDisplayName(hwnd, nullptr, const_cast<LPWSTR>(static_cast<LPCTSTR>(fileName)), nullptr, &newPIdL, nullptr);
		if (SUCCEEDED(hr)) {
			list.push_back(newPIdL);
		}
	}
	if (!list.empty()) {
		hr = desktop->GetUIObjectOf(hwnd, list.size(), const_cast<LPCITEMIDLIST*>(&list[0]), IID_IDataObject, 0, reinterpret_cast<void**>(&pDataObject));
		if (SUCCEEDED(hr)) {
			// Create the Photo Printing Wizard drop target.
			CComPtr<IDropTarget> spDropTarget;
			hr = CoCreateInstance(CLSID_PrintPhotosDropTarget, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&spDropTarget));
			if (SUCCEEDED(hr)) {
				// Drop the data object onto the drop target.
				POINTL pt = { 0 };
				DWORD dwEffect = DROPEFFECT_LINK | DROPEFFECT_MOVE | DROPEFFECT_COPY;
				spDropTarget->DragEnter(pDataObject, MK_LBUTTON, pt, &dwEffect);
				spDropTarget->Drop(pDataObject, MK_LBUTTON, pt, &dwEffect);
				return true;
			}
		}
	}
	return false;
}
VOID CMainDlg::OnCommand(UINT wNotifyCode , int wID , HWND hwndCtl)
{
	switch( wID )
	{
	case MENU_CMD_OPEN_FILE:
		{
			OnOpen();
			break;
		}
	case MENU_CMD_OPEN_PATH:
		{
			SStringT strFolder = BrowseForFolder(m_hWnd);
			if ( strFolder.GetLength() )
			{
				OpenPath(strFolder);
			}
			break;
		}
	case MENU_CMD_TOP_MOUST:
		{
			BOOL bTopMoust = msapi::CApp(AppName).GetSetting(L"top_moust", FALSE);
			msapi::CApp(AppName).SetSetting(L"top_moust", !bTopMoust);
			if ( !bTopMoust ) //取消
				::SetWindowPos(m_hWnd,HWND_TOPMOST,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
			else
				::SetWindowPos(m_hWnd,HWND_NOTOPMOST,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
			break;
		}

	case MENU_CMD_SETTING:
		{
			CSettingDlg setDlg(m_pRot);
			setDlg.DoModal(m_hWnd);
			break;
		}

	case MENU_CMD_UPDATE:
		{
			/*
			SStringT strCurPath;
			msapi::GetCurrentPath(strCurPath.GetBufferSetLength(MAX_PATH), MAX_PATH);
			strCurPath.ReleaseBuffer();
			strCurPath.Append(_T("\\update.exe"));

			msapi::ShellProcess(strCurPath, NULL, TRUE);
			*/
			break;
		}
	case MENU_CMD_OPEN_EXPLORER:
		{
			if (m_ImageCanvas && m_ImageCanvas->IsImageValide())
			{
				SStringT strLastSelectPath = m_ImageCanvas->GetImageFile();
				std::wstring sFolder = _T( "/Select,\"" ) + strLastSelectPath + _T("\"");

				// Prepare shell execution params
				SHELLEXECUTEINFO shExecInfo   = { 0 };
				shExecInfo.cbSize             = sizeof(shExecInfo);
				shExecInfo.lpFile             = _T("explorer.exe");
				shExecInfo.lpParameters       = sFolder.c_str();
				shExecInfo.nShow              = SW_SHOWNORMAL;
				shExecInfo.lpVerb             = _T("open"); // Context menu item
				shExecInfo.fMask              = SEE_MASK_NOCLOSEPROCESS | SEE_MASK_FLAG_NO_UI;

				try
				{
					ShellExecuteEx(&shExecInfo);
				}
				catch(...){}
			}
			break;
		}

	case MENU_CMD_COPY:
		{
			if (m_ImageCanvas && m_ImageCanvas->IsImageValide())
			{
				SStringT strLastSelectPath = m_ImageCanvas->GetImageFile();
				Cf_Hdrop( strLastSelectPath );
				Cf_Html( strLastSelectPath);
				Cf_QQEdit( strLastSelectPath );
			}

			break;
		}
	case MENU_CMD_PASTE:
		{
			if (OpenClipboard( NULL ))
			{
				HDROP hDrop = (HDROP)GetClipboardData(CF_HDROP);
				if(NULL != hDrop)
				{
					int iFiles=DragQueryFile(hDrop, -1, NULL, 0);
					if ( iFiles )
					{
						TCHAR szPathFile[ MAX_PATH ] = { 0 };
						DragQueryFile(hDrop, 0, szPathFile, MAX_PATH);
						DWORD dwAttr = GetFileAttributes( szPathFile );
						if ( dwAttr != 0xFFFFFFFF )
						{
							if ( !( dwAttr & FILE_ATTRIBUTE_DIRECTORY) )
								OpenFile( szPathFile );
						}
					}
				}

				CloseClipboard();
			}
			break;
		}
		
	case MENU_CMD_PRINT:
		{
			//DisplaySystemPrintDialogForImage()
			if (m_ImageCanvas && m_ImageCanvas->IsImageValide())
			{
				SStringT strLastSelectPath = m_ImageCanvas->GetImageFile();
				if (strLastSelectPath.GetLength() )
				{
					strLastSelectPath.Replace(L"\\\\", L"\\");
					std::vector<CString> files;
					files.push_back(strLastSelectPath.GetBuffer(0));
					DisplaySystemPrintDialogForImage(files, m_hWnd);

				}

			}

			break;
		}

	case MENU_CMD_CUT_SCREEN:
		{
			/*
			msapi::ShellProcess(_T("imScreen.exe"), NULL, TRUE);
			*/
			break;
		}
	}
}

void CMainDlg::OnDestory()
{
	MSG msg;
	while (PeekMessage(&msg, m_hWnd, WM_EXECUTE_MSG, WM_EXECUTE_MSG, PM_REMOVE)) 
	{
		DispatchMessage(&msg);
	}

	SAFE_RELEASE(m_pRot);
	SetMsgHandled(FALSE);
}



void CMainDlg::Post(const std::function<void()> &f)
{
	auto func = new std::function<void()>(f);
	PostMessage( WM_EXECUTE_MSG, 0, (LPARAM)func);
}

VOID CMainDlg::SetTabIndex(int nIndex)
{
}

//TODO:消息映射
void CMainDlg::OnClose()
{
	m_bStopEnumFile = TRUE;
	m_EnumThreadEx.WaitForExit(1000);
	CSimpleWnd::DestroyWindow();
}

void CMainDlg::OnMaximize()
{
	SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE);
}
void CMainDlg::OnRestore()
{
	SendMessage(WM_SYSCOMMAND, SC_RESTORE);
}
void CMainDlg::OnMinimize()
{
	SendMessage(WM_SYSCOMMAND, SC_MINIMIZE);
}

void CMainDlg::OnFullScreen()
{

}

void CMainDlg::OnMenu()
{
	SImageButton* pBtn = FindChildByName2<SImageButton>(L"btn_menu");
	SOUI::CRect rc_menu;
	pBtn->GetClientRect(&rc_menu);
	ClientToScreen(&rc_menu);


	BOOL bTopMoust = msapi::CApp(AppName).GetSetting(L"top_moust", FALSE);
	if ( bTopMoust )
		m_MenuMain.GetMenuItem(MENU_CMD_TOP_MOUST)->SetAttribute(L"check", L"1");
	else
		m_MenuMain.GetMenuItem(MENU_CMD_TOP_MOUST)->SetAttribute(L"check", L"0");

	m_MenuMain.GetMenuItem(MENU_CMD_PRINT)->EnableWindow(FALSE, TRUE);
	if (m_ImageCanvas && m_ImageCanvas->IsImageValide())
	{
		SStringT strLastSelectPath = m_ImageCanvas->GetImageFile();
		if (strLastSelectPath.GetLength() )
		{
			m_MenuMain.GetMenuItem(MENU_CMD_PRINT)->EnableWindow(TRUE, TRUE);
		}
		
	}

	UINT nRet = m_MenuMain.TrackPopupMenu(0, rc_menu.left, rc_menu.bottom, m_hWnd);
}

void CMainDlg::OnOpen()
{
	TCHAR szBuffer[MAX_PATH] = {0};   
	OPENFILENAME ofn= {0};   
	ofn.lStructSize = sizeof(ofn);   
	ofn.hwndOwner = m_hWnd;   
	//SStringT sFileExts = GetFileTypes(TRUE);
	//ofn.lpstrFilter =sFileExts;
	//ofn.lpstrDefExt = L".png\0";

	SStringT sExt =  GetFileTypes(TRUE);
	ofn.lpstrFilter = sExt;

	TCHAR szDocPath[MAX_PATH] = { 0 };
	SHGetSpecialFolderPath(NULL, szDocPath, CSIDL_MYDOCUMENTS ,FALSE);

	ofn.lpstrInitialDir = szDocPath;//默认的文件路径   
	ofn.lpstrFile = szBuffer;//存放文件的缓冲区   
	ofn.nMaxFile = sizeof(szBuffer)/sizeof(*szBuffer);   
	ofn.nFilterIndex = 0;   
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_EXPLORER ;//标志如果是多选要加上OFN_ALLOWMULTISELECT  

	if ( GetOpenFileName(&ofn) )
	{
		OpenFile(szBuffer);
	}
}

void CMainDlg::OnSkin()
{
	SImageButton* pBtn = FindChildByName2<SImageButton>(L"btn_menu");
	SOUI::CRect rc_menu;
	pBtn->GetClientRect(&rc_menu);
	ClientToScreen(&rc_menu);

	if ( msapi::CApp(AppName).GetSetting(L"skin_id", SKIN_DEFAULT) == SKIN_DEFAULT)
	{
		CheckMenuItem(m_MenuSkin.m_hMenu, 1001, MF_CHECKED);
		CheckMenuItem(m_MenuSkin.m_hMenu, 1002, MF_UNCHECKED);
	}
	else
	{
		CheckMenuItem(m_MenuSkin.m_hMenu, 1002, MF_CHECKED);
		CheckMenuItem(m_MenuSkin.m_hMenu, 1001, MF_UNCHECKED);
	}
	UINT nRet = m_MenuSkin.TrackPopupMenu(0, rc_menu.left, rc_menu.bottom, m_hWnd);

	switch( nRet )
	{
	case 1001:	//默认皮肤
		{
			SetSkin(SKIN_DEFAULT);
			break;
		}
	case 1002:	//极简皮肤
		{
			SetSkin(SKIN_SIMPLE);
			break;
		}
	

		break;
	}
}

void CMainDlg::OnSize(UINT nType, SOUI::CSize size)
{
	SetMsgHandled(FALSE);
	if (!m_bLayoutInited) return;
	
	__super::OnSize(nType, size);

	SWindow *pBtnMax = FindChildByName(L"btn_max");
	SWindow *pBtnRestore = FindChildByName(L"btn_restore");
	if(!pBtnMax || !pBtnRestore) return;
	
	if (nType == SIZE_MAXIMIZED)
	{
		pBtnRestore->SetVisible(TRUE);
		pBtnMax->SetVisible(FALSE);
	}
	else if (nType == SIZE_RESTORED)
	{
		pBtnRestore->SetVisible(FALSE);
		pBtnMax->SetVisible(TRUE);
	}

	if ( m_ImageCanvas && m_ImageCanvas->IsImageValide() && m_bFitSize)
	{
		SetCanvasFitsize();
	}
}





VOID CMainDlg::OnBarOrignalsize()
{
	SetFitSize(!m_bFitSize);
}

VOID CMainDlg::OnBarFitsize()
{
	SetFitSize(!m_bFitSize);
}

VOID CMainDlg::OnBarZoomin()
{
	float fScale = ( m_fScale * 1.2);
	if( fScale > 32)
	{
		if ( m_ZoomRatioWnd )
			m_ZoomRatioWnd->SetZoomRatio(-1);
		return;
	}

	SetScale(fScale);
	m_ZoomRatioWnd->SetZoomRatio(fScale*100);
}

VOID CMainDlg::OnBarZoomout()
{
	float fScale = ( m_fScale * 0.8);
	if( fScale < 0.05)
	{
		if ( m_ZoomRatioWnd )
			m_ZoomRatioWnd->SetZoomRatio(0);
		return;
	}

	SetScale(fScale);
	m_ZoomRatioWnd->SetZoomRatio(fScale*100);
}

VOID CMainDlg::OnBarPrev()
{
	INT nCount = GetImageCount();
	
	if ( !nCount )
		return ;

	INT nCur = GetCurImageIndex();

	if ( nCur > 0 )
	{
		nCur = nCur - 1;
		
	}
	else
	{
		nCur = nCount -1;
	}

	AUTOLOCK_CS(m_FileList);
	OpenImageFile(m_FileList[nCur]);
}

VOID CMainDlg::OnBarNext()
{
	INT nCount = GetImageCount();

	if ( !nCount )
		return ;

	INT nCur = GetCurImageIndex();


	if (nCur + 1 < nCount )
	{
		nCur = nCur+1;
	}
	else
		nCur = 0;

	AUTOLOCK_CS(m_FileList);
	OpenImageFile(m_FileList[nCur]);
}

VOID CMainDlg::OnBarRotateleft()
{
	if ( m_ImageCanvas && m_ImageCanvas->IsImageValide() && m_ImageCanvas->CanEdition() )
	{
		m_ImageCanvas->RotateLeft();
	}
}

VOID CMainDlg::OnBarRotateright()
{
	if ( m_ImageCanvas && m_ImageCanvas->IsImageValide() && m_ImageCanvas->CanEdition() )
	{
		m_ImageCanvas->RotateRight();
	}
}

VOID CMainDlg::OnBarDelete()
{
	if ( !m_ImageCanvas )
		return;

	if ( !_tcslen( m_ImageCanvas->GetImageFile()) )
		return;



	SHFILEOPSTRUCT  shDelFile;
	memset(&shDelFile,0,sizeof(SHFILEOPSTRUCT));
	shDelFile.fFlags |= FOF_SILENT;      // 不显示进度
	shDelFile.fFlags |= FOF_NOERRORUI;     //不报告错误信息
	//shDelFile.fFlags |= FOF_NOCONFIRMATION;    //直接删除，不进行确认
	// 复制路径到一个以双NULL结束的string变量里
	TCHAR buf[_MAX_PATH + 1]; 
	_tcscpy(buf, m_ImageCanvas->GetImageFile());   // 复制路径
	buf[_tcslen(buf)+1]=0;   // 在末尾加两个NULL

	BOOL bDelete = FALSE;
	// 设置SHFILEOPSTRUCT的参数为删除做准备
	shDelFile.wFunc = FO_DELETE;       // 执行的操作
	shDelFile.pFrom = buf;         // 操作的对象，也就是目录
	shDelFile.pTo = NULL;          // 必须设置为NULL
	if (bDelete) //根据传递的bDelete参数确定是否删除到回收站  
		shDelFile.fFlags &= ~FOF_ALLOWUNDO;    //直接删除，不进入回收站
	else       
		shDelFile.fFlags |= FOF_ALLOWUNDO;    //删除到回收站
	
	if ( !SHFileOperation(&shDelFile) )//删除
	{
		int nIdex = GetCurImageIndex();
		if ( GetImageCount() )
		{
			OnBarNext();
			if ( nIdex != -1)
			{
				AUTOLOCK_CS( m_FileList);
				m_FileList.erase(m_FileList.begin() + nIdex);
				if ( m_FileList.size())
				{
					SetTitleText();
				}
			}
		}
		

		if ( !GetImageCount() )
		{
			m_ImageCanvas->DestoryImage();
			m_tableCtrl->SetCurSel(_T("page_index"),FALSE);
			m_TextTitle->SetWindowText(GETSTRING(L"@string/title"));
			SetBarPaneState();
		}	
	}
}

VOID CMainDlg::OnBarMore()
{

}

//multiframe
VOID CMainDlg::OnMutiframPrev()
{
	if ( m_ImageCanvas->IsMutiframe() )
	{
		if ( m_ImageCanvas->IsAniming() )
			m_ImageCanvas->PauseAnim();

		if ( m_ImageCanvas->GetFrameCount() > m_ImageCanvas->GetCurFrame() && m_ImageCanvas->GetFrameCount() > 0)
			m_ImageCanvas->PrevFrame();

		SetMutiframState();

	}
}

VOID CMainDlg::OnMutiframPlay()
{
	if ( m_ImageCanvas->IsAnimation() && !m_ImageCanvas->IsAniming() )
	{
		m_ImageCanvas->StartAnim();
		SetMutiframState();
	}
}

VOID CMainDlg::OnMutiframPause()
{
	if ( m_ImageCanvas->IsAnimation() && m_ImageCanvas->IsAniming() )
	{
		m_ImageCanvas->PauseAnim();
		SetMutiframState();
	}
}

VOID CMainDlg::OnMutiframNext()
{
	if ( m_ImageCanvas->IsMutiframe() )
	{
		if ( m_ImageCanvas->IsAniming() )
			m_ImageCanvas->PauseAnim();
		
		if ( m_ImageCanvas->GetFrameCount() > m_ImageCanvas->GetCurFrame() )
			m_ImageCanvas->NextFrame();
		
		SetMutiframState();

	}
}

VOID CMainDlg::OnMutiframSave()
{

}

VOID CMainDlg::OnMutiframSaveAll()
{

}

LRESULT CMainDlg::OnExecute(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL/* bHandled*/)
{
	auto func = (std::function<void()> *)lParam;
	(*func)();
	delete func;
	return 0;
}


void CMainDlg::OnMouseMove(UINT nFlags, SOUI::CPoint point)
{
	if ( m_PanelBar->GetClientRect().PtInRect(point) )
	{
		if ( !m_CtrlBar->IsVisible() )
		{
			m_CtrlBar->SetVisible(TRUE, TRUE);
		}
	}
	else if ( m_PanelLeft->GetClientRect().PtInRect(point) )
	{
		if ( !m_BtnBarPrev->IsVisible() )
			m_BtnBarPrev->SetVisible(TRUE, TRUE);
	}
	else if ( m_PanelRight->GetClientRect().PtInRect(point))
	{
		if ( !m_BtnBarNext->IsVisible() )
			m_BtnBarNext->SetVisible(TRUE, TRUE);
	}
	else
	{
		if ( m_CtrlBar->IsVisible() )
			m_CtrlBar->SetVisible(FALSE, TRUE);

		if ( m_BtnBarPrev->IsVisible() )
			m_BtnBarPrev->SetVisible(FALSE, TRUE);

		if ( m_BtnBarNext->IsVisible() )
			m_BtnBarNext->SetVisible(FALSE, TRUE);
	}

	__super::OnMouseMove(nFlags, point);
}

void CMainDlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch( nChar )
	{
	case VK_ADD:
		{
			OnBarZoomin();
			break;
		}
	case VK_SUBTRACT:
		{
			OnBarZoomout();
			break;
		}
	case VK_LEFT:
		{
			OnBarPrev();
			break;
		}
	case VK_RIGHT :
		{
			OnBarNext();
			break;
		}
	case VK_UP:
		{
			OnMutiframPrev();
			break;
		}
	case VK_DOWN:
		{
			OnMutiframNext();
			break;
		}
	case VK_SPACE :
		{
			if ( m_ImageCanvas->IsAniming() )
				OnMutiframPause();
			else
				OnMutiframPlay();
			break;
		}
	case VK_DELETE:
		{
			OnBarDelete();
			break;
		}
	case VK_ESCAPE:
		{
			OnClose();
			break;
		}
	case 'C':
		{
			if ( HIBYTE(GetKeyState(VK_CONTROL)))
			{
				SendMessage(WM_COMMAND, MAKEWPARAM(MENU_CMD_COPY,0), (LPARAM)m_hWnd);
			}

			break;
		}

	case 'V':
		{
			if ( HIBYTE(GetKeyState(VK_CONTROL)))
			{
				SendMessage(WM_COMMAND, MAKEWPARAM(MENU_CMD_PASTE,0), (LPARAM)m_hWnd);
			}

			break;
		}
	}
}

VOID CMainDlg::OnCheckUpdata(EventArgs *pEvt /*= NULL*/)
{


}


bool CMainDlg::OnCanvasSize(EventSwndSize *pEvt)
{
	return true;
}

bool CMainDlg::OnCanvasMouseMove(EventCanvasMouseMove* pEvt)
{
	//if ( !m_bFitSize )
	//{
	
		//m_ScalPoint = pEvt->point;
	//}
	return true;
}

bool CMainDlg::OnCanvasMouseLeave(EventSwndMouseLeave* pEvt)
{
	m_ScalPoint = m_ImageView->GetClientRect().CenterPoint();
	return true;
}

bool CMainDlg::OnCanvasWndMove(EventCanvasWndMove* pEvt)
{
	//触发该消息说明该取消自适应窗口模式了
	if ( !m_bLayoutInited )
		return true;
	
	if ( !m_bFitSize )
	{
		//SetFitSize(!m_bFitSize);
	}

	return true;
}

bool CMainDlg::OnImageViewMouseWhell(EventImageViewMouseWhell* pEvt)
{
	if ( !( m_ImageView && m_ImageCanvas && m_ImageCanvas->IsImageValide()))
		return true;

	if ( pEvt->zDelta > 0)
		OnBarZoomin();
	else
		OnBarZoomout();
	
	return true;
}

bool CMainDlg::OnCanvasFrameChange(EventCanvasFrameChange* pEvt)
{
	SStringT sText;
	sText.Format( _T("%d/%d"), m_ImageCanvas->GetCurFrame() + 1, m_ImageCanvas->GetFrameCount());
	m_TextFrameCount->SetWindowText(sText);
	if ( !m_ImageCanvas->IsAniming() )
		SetCanvasFitsize();

	
	return true;
}

bool CMainDlg::OnCanvasImageLoad(EventCanvasImageLoad* pEvt)
{
	if ( pEvt->bResult )
	{
		m_tableCtrl->SetCurSel(_T("page_image"),FALSE);
		if( m_ImageCanvas->IsAnimation())
			m_ImageCanvas->StartAnim();
		SetCanvasFitsize();
		SetMutiframState();
	}
	else
	{
		m_tableCtrl->SetCurSel(_T("page_error"),FALSE);
	}
	SetBarPaneState();
	SetTitleText();
	return true;
}

bool CMainDlg::OnImageMenu(EventCtxMenu* pEvt)
{
	SOUI::CPoint point = pEvt->pt;
	SOUI::CRect rcCantainer=GetContainer()->GetContainerRect();
	point.Offset(rcCantainer.TopLeft());
	HWND hHost=GetContainer()->GetHostHwnd();
	::ClientToScreen(hHost,&point);
	UINT nRet = m_MenuImage.TrackPopupMenu(0, point.x, point.y, m_hWnd);

	

	return true;
}

/*
VOID CMainDlg::OnWorkCheckUpdata()
{
	network::CHttpImplement http;
	network::CUrlParamValueMap urlParamMap;
	TCHAR szDiskSer[MAX_PATH] = { 0 };
	TCHAR szVer[ MAX_PATH ] = { 0 };
	TCHAR szOper[MAX_PATH] = { 0 };

	msapi::CApp softApp(CW2T(GetEnvParamString("productname")));

	urlParamMap["prouctid"] = softApp.GetSetting(_T("prod"), 0);
	urlParamMap["packetid"] = softApp.GetSetting(_T("packet"), 0);
	urlParamMap["version"]	= softApp.GetVersion(szVer, MAX_PATH);
	urlParamMap["oper"]		= softApp.GetSetting(_T("oper"), _T("0"), szOper, MAX_PATH);
	urlParamMap["hdd"]	    = msapi::CApp::GetDiskSerial(szDiskSer, MAX_PATH);
	urlParamMap["uguid"]	= msapi::CApp::GetUserGuid();
	urlParamMap["ant"]		= msapi::CEnumAntivirus().Enum();
	urlParamMap["random"] = GetTickCount();
	if ( http.GetRequest("api.kyan.com.cn", 80, "imageview/update", urlParamMap) )
	{
		return ;
	}

	Json::Value root;
	std::string sText = (const char*)http.GetRequestData();
	if ( !Json::Reader().parse(sText, root) )
	{
		return ;
	}

	Json::Value retCode =  root["code"];
	if (retCode.type() != Json::intValue )
	{
		return ;
	}

	//已是最新版本
	if ( retCode.asInt() == 0 )
	{
		return ;
	}

	//检测到最新版本
	if (  retCode.asInt() == 1 )
	{
		TCHAR szVer[ MAX_PATH ] = { 0 };
		Json::Value Msg = root["msg"];
		SStringT strNetVer = CW2T(Msg["version"].asWString().c_str());

		if ( CVerCmp::CompareStr(strNetVer, softApp.GetVersion( ).c_str()) > 0)
		{
			Post([this]{ OnCheckUpdateUI();}); //检查到更新了
		}
	}

}
*/

VOID CMainDlg::DoReportActive()
{
	msdk::network::CUrlParamValueMap ParamValueMap;
	ParamValueMap["apiver"]			=		1;					//当前API版本
	//ParamValueMap["hdd"]			=		msapi::CApp::GetDiskSerial();
	ParamValueMap["uguid"]			=		msapi::CApp::GetUserGuid();
	//ParamValueMap["mac"]			=		CGetNetAdapterInfo::GetMac();

	ParamValueMap["osver"]			=		msapi::GetMicroVersion();
	ParamValueMap["wow64"]			=		msapi::IsWindowsX64();

	//ParamValueMap["ant"]			=		msapi::CEnumAntivirus().Enum();
	ParamValueMap["prod"]			=		_Def_Prod;
	ParamValueMap["packet"]			=		_Def_Packet;
	ParamValueMap["subprod"]		=		0;	//当前模块的子产品
	ParamValueMap["idate"]			=		msapi::GetCurrentDateStr(FALSE);
	ParamValueMap["itime"]			=		msapi::GetCurrentDateStr(FALSE);
	ParamValueMap["prodver"]		=		m_strLocalVer.GetBuffer(0);
	ParamValueMap["oper"]			=		GetOper();
	ParamValueMap["random"]			=		GetTickCount();
	msdk::network::CHttpImplement HttpImpl;
	HttpImpl.GetRequest(UPDATE_URL, 80, "active" , ParamValueMap);
}


VOID CMainDlg::OnCheckUpdateUI()
{
	SImageButton* pSysMenu = FindChildByName2<SImageButton>(L"btn_menu");
	if( pSysMenu )
	{
		pSysMenu->SetAttribute(L"skin", L"skin_sys_menu_red");
	}

	m_MenuMain.GetMenuItem(2005)->SetAttribute(L"icon", L"6");
}

BOOL CMainDlg::SetFitSize(BOOL bFitSize)
{
// 	if ( bFitSize != m_bFitSize )
// 	{
		m_bFitSize = bFitSize;
		SImageButton* pBtn = FindChildByName2<SImageButton>(L"bar_orignalsize");
		if ( pBtn )
			pBtn->SetVisible(!bFitSize);

		pBtn = FindChildByName2<SImageButton>(L"bar_fitsize");
		if ( pBtn )
			pBtn->SetVisible(bFitSize);
	//}


	if ( !m_bFitSize )
	{
		SOUI::CRect rect = m_ImageView->GetClientRect();
		m_ScalPoint = rect.CenterPoint();
		SetScale(1);
	}
	else
	{
		SOUI::CRect rect = m_ImageView->GetClientRect();
		if(m_ImageCanvas->GetImageWidth()<=rect.Width() && m_ImageCanvas->GetImageHeight() <= rect.Height()) //小图片，不缩放
		{
			SOUI::CRect rect = m_ImageView->GetClientRect();
			m_ScalPoint = rect.CenterPoint();
			SetScale(1);
		}
		else
		{
			float xScale=(float)rect.Width()/(float)m_ImageCanvas->GetImageWidth();  
			float yScale=(float)rect.Height()/(float)m_ImageCanvas->GetImageHeight();  
			float ScaleIndex = (xScale >= yScale ? yScale : xScale);  //选择一个较小的比例
			m_ScalPoint = m_ImageView->GetClientRect().CenterPoint();
			SetScale(ScaleIndex);
		}
	}

	return m_bFitSize;
}

BOOL CMainDlg::SetScale(float fScale)
{
	m_fScale = fScale;
	if( !( m_ImageCanvas && m_ImageCanvas->IsImageValide() ))
		return FALSE;

	float fImageWidth = m_ImageCanvas->GetImageWidth();
	float fImageHeitht = m_ImageCanvas->GetImageHeight();

	int width = fScale * fImageWidth;
	int height = fScale * fImageHeitht;

	//计算中心位置

	

	/*m_ScalPoint.x = m_ScalPoint.x*/
	SOUI::CPoint x = m_ScalPoint;
	x.Offset(-(width / 2) ,  -(height / 2));

	SOUI::CPoint y = m_ScalPoint;
	y.Offset(width / 2 , height / 2);

	SOUI::CRect rect = SOUI::CRect(x,y);

	m_ImageCanvas->Move(rect);
	if ( m_Caption )
	{
		//m_Caption->Invalidate();
	}
	SetTitleText();
	return TRUE;
}

VOID CMainDlg::SetMutiframState()
{
	if ( !( m_ImageCanvas && m_MutiframPanel) )
		return ;

	m_MutiframPanel->SetVisible(m_ImageCanvas->IsMutiframe(), TRUE);
	m_MultiframePause->SetVisible(FALSE);
	m_MultiframePlay->SetVisible(TRUE,TRUE);


	if ( m_ImageCanvas->IsAnimation() )
	{
		m_MultiframePlay->EnableWindow(TRUE);
		if ( m_ImageCanvas->IsAniming() )
		{
			m_MultiframePause->SetVisible(TRUE);
			m_MultiframePlay->SetVisible(FALSE,TRUE);
		}
	}
	else
		m_MultiframePlay->EnableWindow(FALSE,TRUE);


	if ( m_ImageCanvas->IsMutiframe() )
	{
		if ( m_ImageCanvas->GetCurFrame() == (m_ImageCanvas->GetFrameCount() - 1) )
		{
			m_MultiframeNext->EnableWindow(FALSE);
			m_MultiframePrev->EnableWindow(TRUE,TRUE);

		}
		else if( m_ImageCanvas->GetCurFrame() ==0 )
		{
			m_MultiframeNext->EnableWindow(TRUE);
			m_MultiframePrev->EnableWindow(FALSE);
		}
		else
		{
			m_MultiframeNext->EnableWindow(TRUE);
			m_MultiframePrev->EnableWindow(TRUE,TRUE);
		}
	}

	m_MutiframPanel->Invalidate();
}

VOID CMainDlg::SetBarPaneState()
{
	/*
	<imgbtn name="bar_orignalsize" skin="skin_btn_orignalsize" pos="|-206,|-15" visible="0"/>
	<imgbtn name="bar_fitsize" skin="skin_btn_fitsize" pos="|-206,|-15"/>
	<imgbtn name="bar_zoomin" skin="skin_btn_zoomin" pos="[,|-15"/>
	<imgbtn name="bar_zoomout" skin="skin_btn_zoomout" pos="[,|-15"/>
	<imgbtn name="bar_prev" skin="skin_bar_prev" pos="[,|-15"/>
	<imgbtn name="bar_next" skin="skin_bar_next" pos="[,|-15"/>
	<imgbtn name="bar_rotateleft" skin="skin_bar_rotateleft" pos="[,|-15"/>
	<imgbtn name="bar_rotateright" skin="skin_bar_rotateright" pos="[,|-15"/>
	<imgbtn name="bar_delete" skin="skin_bar_delete" pos="[,|-15"/>
	<imgbtn name="bar_more" skin="skin_bar_more" pos="[,|-15" visible="0"/>
	*/

	BOOL isImageValide = m_ImageCanvas->IsImageValide();
	BOOL isEdit = CanBeEdit();

	SImageButton*pBtn = FindChildByName2<SImageButton>(L"bar_orignalsize");
	pBtn->EnableWindow(isImageValide, TRUE);

	pBtn = FindChildByName2<SImageButton>(L"bar_fitsize");
	pBtn->EnableWindow(isImageValide, TRUE);

	pBtn = FindChildByName2<SImageButton>(L"bar_zoomin");
	pBtn->EnableWindow(isImageValide, TRUE);

	pBtn = FindChildByName2<SImageButton>(L"bar_zoomout");
	pBtn->EnableWindow(isImageValide, TRUE);
	
	pBtn = FindChildByName2<SImageButton>(L"bar_rotateleft");
	pBtn->EnableWindow(isEdit, TRUE);

	pBtn = FindChildByName2<SImageButton>(L"bar_rotateright");
	pBtn->EnableWindow(isEdit, TRUE);

	pBtn = FindChildByName2<SImageButton>(L"bar_prev");
	pBtn->EnableWindow(GetImageCount(), TRUE);

	pBtn = FindChildByName2<SImageButton>(L"bar_next");
	pBtn->EnableWindow(GetImageCount(), TRUE);

	pBtn = FindChildByName2<SImageButton>(L"bar_delete");
	pBtn->EnableWindow(_tcslen(m_ImageCanvas->GetImageFile()), TRUE);


}

VOID CMainDlg::SetTitleText()
{
	if ( m_TextTitle )
	{
		SStringT sText;
		if ( m_ImageCanvas->IsImageValide() )
		{
			//<title_format value="%s（%d×%d像素）- 快眼看图王 - 第d% %d张 d% %%"/>
			int nScale = m_fScale * 100;
			sText.Format(GETSTRING(L"@string/title_format"), 
				PathFindFileName( m_ImageCanvas->GetImageFile() ), 
				m_ImageCanvas->GetImageWidth(),
				m_ImageCanvas->GetImageHeight(),
				GetCurImageIndex() + 1,GetImageCount(), nScale );
			m_TextTitle->SetWindowText(sText);
		}
		else
		{
			sText.Format(_T("%s - %s - 第%d/%d张"), PathFindFileName( m_ImageCanvas->GetImageFile() ), GETSTRING(L"@string/title"),GetCurImageIndex() + 1,GetImageCount());
			m_TextTitle->SetWindowText(sText);
		}
	}
}

VOID CMainDlg::SetCanvasFitsize()
{
	if ( !( m_ImageCanvas && m_ImageView ))
		return ;

	SOUI::CRect rect = m_ImageView->GetClientRect();
	if(m_ImageCanvas->GetImageWidth()<=rect.Width() && m_ImageCanvas->GetImageHeight() <= rect.Height()) //小图片，不缩放
	{
		SOUI::CRect rect = m_ImageView->GetClientRect();
		m_ScalPoint = rect.CenterPoint();
		SetScale(1);
	}
	else
	{
		float xScale=(float)rect.Width()/(float)m_ImageCanvas->GetImageWidth();  
		float yScale=(float)rect.Height()/(float)m_ImageCanvas->GetImageHeight();  
		float ScaleIndex = (xScale >= yScale ? yScale : xScale);  //选择一个较小的比例
		m_ScalPoint = m_ImageView->GetClientRect().CenterPoint();
		SetScale(ScaleIndex);
	}
}

VOID CMainDlg::SetCanvasOrignalsize()
{
	if ( !( m_ImageCanvas && m_ImageView ))
		return ;

	SOUI::CRect rect = m_ImageView->GetClientRect();
	m_ScalPoint = rect.CenterPoint();
	SetScale(1);
}



BOOL ImageFileEnumFunction(LPCTSTR lpszFile, LPVOID lpParam)
{
	CMainDlg* pDlg = (CMainDlg*)lpParam;
	if ( pDlg->m_bStopEnumFile )
		return FALSE;

	SStringT sFile = lpszFile; sFile.MakeLower();
	if ( sFile.Find(_T("$recycle.bin")) != -1)
	{
		return TRUE;
	}
	if ( GetFileAttributes(lpszFile) & FILE_ATTRIBUTE_DIRECTORY)
		return TRUE;

	LPCTSTR lpszExt = PathFindExtension( lpszFile );
	if ( !(lpszExt && _tcslen( lpszExt)) )
		return TRUE;

	int nImageType = CxImage::GetTypeIdFromName(lpszExt + 1);
	if ( nImageType == CXIMAGE_FORMAT_UNKNOWN )
		return TRUE;
	
	
	pDlg->Post([sFile,pDlg]{pDlg->OneImageFileFind(sFile);});
	
	
	return TRUE;
}

VOID CMainDlg::EnumImageFile(LPCTSTR lpstrPath, BOOL bStrike/* = FALSE*/)
{
	SStringT strPath = lpstrPath;

	{
		AUTOLOCK_CS(m_FileList);
		m_FileList.clear();
	}
	m_bStopEnumFile = TRUE;
	m_EnumThreadEx.WaitForExit( 1000 );
	m_EnumThreadEx.Do([=]
	{
		m_bStopEnumFile = FALSE;
		msapi::CEnumFile (ImageFileEnumFunction, this,FALSE, bStrike).EnumFile(strPath, _T("*.*"));
		Post([=]{SetTitleText();});
	});
}

INT CMainDlg::GetCurImageIndex()
{
	AUTOLOCK_CS(m_FileList);
	if ( !m_ImageCanvas )
		return -1;


	LPCTSTR lpszFile = m_ImageCanvas->GetImageFile();
	if ( _tcslen(lpszFile) == 0)
		return -1;
	

	for ( int nLoop = 0;  nLoop < m_FileList.size();nLoop++ )
	{
		if ( m_FileList[nLoop].CompareNoCase(lpszFile) == 0)
		{
			return nLoop;
		}
	}

	return -1;
}

INT CMainDlg::GetImageCount()
{
	AUTOLOCK_CS(m_FileList);
	return m_FileList.size();
}

VOID CMainDlg::OneImageFileFind(LPCTSTR lpszFile)
{
	AUTOLOCK_CS(m_FileList);
	m_FileList.push_back(lpszFile);

	if ( m_ImageCanvas )
	{
		if ( _tcslen( m_ImageCanvas->GetImageFile()) == 0)
		{
			AUTOLOCK_CS(m_FileList);
			if ( m_FileList.size() )
			{
				m_ImageCanvas->SetImageFile(m_FileList[0]);
			}
		}
	}
	if ( m_FileList.size() == 1)
	{
		SetBarPaneState();
		SetTitleText();
	}

	//
}

VOID CMainDlg::Cf_Hdrop(LPCTSTR lpszFile)
{

	DWORD dwHtmlFormat = RegisterClipboardFormat(_T("html format"));
	DWORD dwQQEditFormat = RegisterClipboardFormat(_T("qq_richedit_format"));;


	/*
	if ( OpenClipboard(NULL) )
	{
		int nFormat = 0;
		while( nFormat = EnumClipboardFormats(nFormat))
		{
			WCHAR szFormatName[ MAX_PATH ] = { 0 };
			GetClipboardFormatName( nFormat, szFormatName, MAX_PATH);
			if ( wcsicmp( szFormatName, L"qq_richedit_format") == 0)
			{
				dwQQEditFormat = nFormat;
			}
			if ( wcsicmp( szFormatName, L"html format") == 0)
			{
				dwHtmlFormat = nFormat;
			}
		}

			CloseClipboard();
	}
	*/
	LPDWORD lpdDropEffect = 0;
	DROPFILES stDrop = { 0 };
	stDrop.fWide = TRUE;
	stDrop.pFiles = sizeof(DROPFILES);

	DWORD dwLen = sizeof(DROPFILES)+ (_tcslen(lpszFile) + 2) * sizeof(wchar_t);
	HGLOBAL hGblFiles = GlobalAlloc(GMEM_ZEROINIT | GMEM_MOVEABLE | GMEM_DDESHARE,  dwLen);
	if ( hGblFiles )
	{
		PBYTE lpData = (PBYTE)GlobalLock(hGblFiles);
		memcpy(lpData,&stDrop,sizeof(DROPFILES));
		BYTE *pFileList = lpData + sizeof(DROPFILES);
		UINT uLen = ( _tcslen(lpszFile)  + 1) * sizeof(TCHAR);  
		 memcpy(pFileList, (BYTE*)(LPCTSTR)(lpszFile),  uLen);  
		GlobalUnlock(hGblFiles);
	}


	SStringA strHtml ;
	strHtml.Format( 
			"Version:0.9"
			"StartHTML:00000112"
			"EndHTML:00000316"
			"StartFragment:00000126"
			"EndFragment:00000280"
			"<!doctype html><html><body>"
			"<!--StartFragment --><DIV>"
			"<IMG src=\"file:///%s\" >"
			"</DIV><!--EndFragment-->"
			"</body>"
			"</html>", CW2A(lpszFile));


	dwLen = sizeof( TCHAR ) * ( strHtml.GetLength() + 1);
	HGLOBAL hGblHtml = GlobalAlloc(GMEM_ZEROINIT | GMEM_MOVEABLE | GMEM_DDESHARE,  dwLen);
	if ( hGblHtml )
	{
		PBYTE lpData = (PBYTE)GlobalLock(hGblHtml);
		memcpy(lpData, (BYTE*)(LPCSTR)(strHtml),  dwLen);  
		GlobalUnlock(hGblHtml);
	}


	SStringA strQQEdit;
	strQQEdit.Format("<QQRichEditFormat><Info version=\"1001\"></Info><EditElement type=\"1\" filepath=\"%s\" shortcut=\"\"></EditElement></QQRichEditFormat>",CW2A(lpszFile));
	dwLen = sizeof( TCHAR ) * ( strQQEdit.GetLength() + 1);

	HGLOBAL hGblQQEdit = GlobalAlloc(GMEM_ZEROINIT | GMEM_MOVEABLE | GMEM_DDESHARE,  dwLen);
	if ( hGblQQEdit )
	{
		PBYTE lpData = (PBYTE)GlobalLock(hGblQQEdit);
		memcpy(lpData, (BYTE*)(LPCSTR)(strQQEdit),  dwLen);  
		GlobalUnlock(hGblQQEdit);
	}

	if ( OpenClipboard(NULL) )
	{
		EmptyClipboard();
		SetClipboardData(CF_HDROP,hGblFiles);//CF_HDROP
		SetClipboardData(dwHtmlFormat,hGblHtml);  //html
		SetClipboardData(dwQQEditFormat,hGblQQEdit);  //qqedit
		CloseClipboard();
	}
}

VOID CMainDlg::Cf_Html(LPCTSTR lpszFile)
{

}

VOID CMainDlg::Cf_QQEdit(LPCTSTR lpszFile)
{

}

VOID CMainDlg::OpenImageFile(LPCTSTR lpszImageFile)
{
	if ( !( m_tableCtrl && m_ImageCanvas ) )
		return ;
	
	LPCTSTR lpszExt = PathFindExtension( lpszImageFile );
	if ( !(lpszExt && _tcslen( lpszExt)) )
		return ;

	int nImageType = CxImage::GetTypeIdFromName(lpszExt + 1);
	if ( nImageType == CXIMAGE_FORMAT_UNKNOWN )
		return ;

	BOOL bRet = m_ImageCanvas->SetImageFile( lpszImageFile );
	
}

VOID CMainDlg::OpenPath(LPCTSTR lpszPath)
{
	if ( !(GetFileAttributes(lpszPath) & FILE_ATTRIBUTE_DIRECTORY))
		return ;
	if ( m_ImageCanvas )
	{
		m_ImageCanvas->DestoryImage();
	}

	EnumImageFile(lpszPath,TRUE);
}

BOOL CMainDlg::IsImageValide()
{
	if ( m_ImageCanvas )
		return m_ImageCanvas->IsImageValide();
	
	return FALSE;
}

BOOL CMainDlg::CanBeEdit()
{

	if ( m_ImageCanvas )
		return m_ImageCanvas->CanEdition() && m_ImageCanvas->IsImageValide();
	return FALSE;
}

BOOL CMainDlg::IsImageAnim()
{
	if ( m_ImageCanvas )
		return m_ImageCanvas->IsAnimation();

	return FALSE;
}

VOID CMainDlg::SetSkin(INT nSkin)
{
	msapi::CApp(AppName).SetSetting(L"skin_id", nSkin);
	SWindow* pWnd = FindChildByName2<SWindow>(L"title_wind");
	if ( SKIN_DEFAULT == nSkin )
	{
		pWnd->SetAttribute(L"skin", _T("skin_header_img"),TRUE);
		pWnd->SetAttribute(L"alpha",L"255");
	}
	else
	{
		pWnd->SetAttribute(L"skin", _T("skin_bk_20"),TRUE);
		pWnd->SetAttribute(L"alpha",L"60");
	}

	pWnd = FindChildByName2<SWindow>(L"table");
	if ( SKIN_DEFAULT == nSkin )
		pWnd->SetAttribute(L"skin", _T("skin_bk"),TRUE);
	else
		pWnd->SetAttribute(L"skin", _T(""),TRUE);

	pWnd = FindChildByName2<SWindow>(L"btn_open");
	if ( SKIN_DEFAULT == nSkin )
		pWnd->SetAttribute(L"skin", _T("skin_btn_open"),TRUE);
	else
		pWnd->SetAttribute(L"skin", _T("skin_btn_open_trans"),TRUE);

	pWnd = FindChildByName2<SWindow>(L"ctrl_bar");
	if ( nSkin == SKIN_DEFAULT )
	{
		pWnd->SetAttribute(L"alpha",L"255");
		pWnd->SetAttribute(L"skin", _T("skin_bk_bar"),TRUE);
	}
	else
	{
		pWnd->SetAttribute(L"alpha",L"100");
		pWnd->SetAttribute(L"skin", _T("skin_bk_40"),TRUE);
	}
	
	pWnd = FindChildByName2<SWindow>(L"bar_orignalsize");
	if ( nSkin == SKIN_DEFAULT )
		pWnd->SetAttribute(L"skin", _T("skin_btn_orignalsize"),TRUE);
	else
		pWnd->SetAttribute(L"skin", _T("skin_btn_orignalsize_black"),TRUE);

	pWnd = FindChildByName2<SWindow>(L"bar_fitsize");
	if ( nSkin == SKIN_DEFAULT )
		pWnd->SetAttribute(L"skin", _T("skin_btn_fitsize"),TRUE);
	else
		pWnd->SetAttribute(L"skin", _T("skin_btn_fitsize_black"),TRUE);

	pWnd = FindChildByName2<SWindow>(L"bar_zoomin");
	if ( nSkin == SKIN_DEFAULT )
		pWnd->SetAttribute(L"skin", _T("skin_btn_zoomin"),TRUE);
	else
		pWnd->SetAttribute(L"skin", _T("skin_btn_zoomin_black"),TRUE);

	pWnd = FindChildByName2<SWindow>(L"bar_zoomout");
	if ( nSkin == SKIN_DEFAULT )
		pWnd->SetAttribute(L"skin", _T("skin_btn_zoomout"),TRUE);
	else
		pWnd->SetAttribute(L"skin", _T("skin_btn_zoomout_black"),TRUE);

	pWnd = FindChildByName2<SWindow>(L"bar_prev");
	if ( nSkin == SKIN_DEFAULT )
		pWnd->SetAttribute(L"skin", _T("skin_bar_prev"),TRUE);
	else
		pWnd->SetAttribute(L"skin", _T("skin_bar_prev_black"),TRUE);

	pWnd = FindChildByName2<SWindow>(L"bar_next");
	if ( nSkin == SKIN_DEFAULT )
		pWnd->SetAttribute(L"skin", _T("skin_bar_next"),TRUE);
	else
		pWnd->SetAttribute(L"skin", _T("skin_bar_next_black"),TRUE);

	pWnd = FindChildByName2<SWindow>(L"bar_rotateleft");
	if ( nSkin == SKIN_DEFAULT )
		pWnd->SetAttribute(L"skin", _T("skin_bar_rotateleft"),TRUE);
	else
		pWnd->SetAttribute(L"skin", _T("skin_bar_rotateleft_black"),TRUE);

	pWnd = FindChildByName2<SWindow>(L"bar_rotateright");
	if ( nSkin == SKIN_DEFAULT )
		pWnd->SetAttribute(L"skin", _T("skin_bar_rotateright"),TRUE);
	else
		pWnd->SetAttribute(L"skin", _T("skin_bar_rotateright_black"),TRUE);

	pWnd = FindChildByName2<SWindow>(L"bar_delete");
	if ( nSkin == SKIN_DEFAULT )
		pWnd->SetAttribute(L"skin", _T("skin_bar_delete"),TRUE);
	else
		pWnd->SetAttribute(L"skin", _T("skin_bar_delete_black"),TRUE);

	pWnd = FindChildByName2<SWindow>(L"bar_more");
	if ( nSkin == SKIN_DEFAULT )
		pWnd->SetAttribute(L"skin", _T("skin_bar_more"),TRUE);
	else
		pWnd->SetAttribute(L"skin", _T("skin_bar_more_black"),TRUE);

	this->RequestRelayout(this,TRUE);
}
BOOL CMainDlg::CheckUpdate()
{
	network::CHttpImplement HttpImpl;
	network::CUrlParamValueMap ParamValueMap;

	ParamValueMap["apiver"]			=		1;					//当前API版本
	//ParamValueMap["hdd"]			=		msapi::CApp::GetDiskSerial();
	ParamValueMap["uguid"]			=		msapi::CApp::GetUserGuid();
	//ParamValueMap["mac"]			=		CGetNetAdapterInfo::GetMac();

	ParamValueMap["osver"]			=		msapi::GetMicroVersion();
	ParamValueMap["wow64"]			=		msapi::IsWindowsX64();

	ParamValueMap["ant"]			=		msapi::CEnumAntivirus().Enum();
	ParamValueMap["prod"]			=		_Def_Prod;
	ParamValueMap["packet"]			=		_Def_Packet;
	ParamValueMap["subprod"]		=		0;	//当前模块的子产品
	ParamValueMap["idate"]			=		msapi::GetCurrentDateStr(FALSE);
	ParamValueMap["itime"]			=		msapi::GetCurrentDateStr(FALSE);
	ParamValueMap["prodver"]		=		_Def_Version;
	ParamValueMap["oper"]			=		GetOper();

	Json::Value Root;
	if ( HttpImpl.PostRequest( _Def_ApiHost, 80, _Def_Update_NetApi,ParamValueMap) )
	{
		return FALSE;
	}

	if ( !(HttpImpl.GetRequestLength() && HttpImpl.GetRequestData()) )
	{
		return FALSE;
	}
	std::string sJson = (const char*)HttpImpl.GetRequestData();
	//sJson = AES::DecryptionAES(_Def_AesKey, _Def_AesVi,sJson);
	if ( !Json::Reader().parse( sJson, Root) )
		return FALSE;

	if ( Root["version"].isString() )
	{
		m_updateInfo.strVersion	= utf82w(Root["version"].asString()).c_str();
	}

	if ( Root["url"].isString())
	{
		m_updateInfo.strUrl	= utf82w(Root["url"].asString()).c_str();
	}
	if ( Root["Param"].isString())
	{
		m_updateInfo.strParam	= utf82w(Root["Param"].asString()).c_str();
	}

	if ( Root["content"].isString())
	{
		m_updateInfo.strContent	= utf82w(Root["content"].asString()).c_str();
	}

	if ( Root["webhost"].isString())
	{
		m_updateInfo.strWebHost	= utf82w(Root["webhost"].asString()).c_str();
	}

	if ( Root["force"].asBool() )
	{
		m_updateInfo.bForce	= Root["force"].asBool();
	}

	//m_updateInfo.strLocVer = m_MachineInfo.strVersion;
	return TRUE;
}

VOID CMainDlg::DoUpdate()
{
	
	

	CThreadEx().Do([=]{
		if ( CheckUpdate() )
		{
			if ( CVerCmp::CompareStr( m_updateInfo.strVersion , m_updateInfo.strLocVer ) > 0 )
			{
				if( !m_updateInfo.bForce && dlgUpdate::IsSkip( m_updateInfo.strVersion ) )
					return ;

				Post([=]{

					dlgUpdate dlg( m_updateInfo );
					if ( dlg.DoModal( m_hWnd) == IDCANCEL)
					{
						if ( m_updateInfo.bForce )
						{
							TerminateProcess( GetCurrentProcess(), 0);
						}
					}
				});
			}
		}
		//判断升级
	});
}

DWORD CMainDlg::GetOper()
{
	SStringT strIniFile;
	msapi::GetCurrentPath(strIniFile.GetBufferSetLength(MAX_PATH), MAX_PATH);
	strIniFile.ReleaseBuffer();
	strIniFile.Append(L"\\setting.ini");

	return GetPrivateProfileInt( L"Setting", L"Oper", _Def_Oper, strIniFile);
}
