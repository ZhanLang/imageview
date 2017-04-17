#include "StdAfx.h"
#include "DropTargetImage.h"

#include <ShellAPI.h>
#include "MainDlg.h"
CDropTargetImage::CDropTargetImage(CMainDlg *pwindow,HWND hwnd):m_pmanwindow(pwindow),m_hwnd(hwnd)
{
	if(m_pmanwindow) 
		m_pmanwindow->AddRef();
}


CDropTargetImage::~CDropTargetImage(void)
{
	if(m_pmanwindow) 
		m_pmanwindow->Release();
}

HRESULT STDMETHODCALLTYPE CDropTargetImage::Drop( 
	/* [unique][in] */ __RPC__in_opt IDataObject *pDataObj,
	/* [in] */ DWORD grfKeyState,
	/* [in] */ POINTL pt,
	/* [out][in] */ __RPC__inout DWORD *pdwEffect)
{
	FORMATETC format =
	{
		CF_HDROP, 0, DVASPECT_CONTENT, -1, TYMED_HGLOBAL
	};
	STGMEDIUM medium;
	if(FAILED(pDataObj->GetData(&format, &medium)))
	{
		return S_FALSE;
	}

	HDROP hdrop = static_cast<HDROP>(GlobalLock(medium.hGlobal));

	if(!hdrop)
	{
		return S_FALSE;
	}

	//////////////////////////////////////////////////////////////////////////
	UINT nFileCount = ::DragQueryFile(hdrop, -1, NULL, 0);

	TCHAR szFileName[_MAX_PATH] = _T("");
	DWORD dwAttribute;
	std::vector<SStringT> vctString;

	// 获取拖拽进来文件和文件夹
	for (UINT i = 0; i < nFileCount; i++)
	{
		::DragQueryFile(hdrop, i, szFileName, sizeof(szFileName));
		dwAttribute = ::GetFileAttributes(szFileName);

		// 是否为文件夹
		if ( dwAttribute & FILE_ATTRIBUTE_DIRECTORY )
		{          
			m_pmanwindow->OpenPath(szFileName);
		}
		else
		{
			m_pmanwindow->OpenFile(szFileName);
		}
	}

	//////////////////////////////////////////////////////////////////////////
	GlobalUnlock(medium.hGlobal);
	if (m_pmanwindow)
	{
		
	}
	*pdwEffect=DROPEFFECT_LINK;
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CDropTargetImage::DragOver(DWORD grfKeyState,POINTL pt,__RPC__inout DWORD *pdwEffect)
{
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CDropTargetImage::DragLeave(void)
{
	return S_OK;
}
