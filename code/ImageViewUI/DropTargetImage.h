#pragma once
#include "droptargetbase.h"
class CMainDlg;
class CDropTargetImage :
	public CDropTargetBase
{
protected:
	CMainDlg *m_pmanwindow;
	HWND m_hwnd;
public:
	CDropTargetImage(CMainDlg *pwindow,HWND hwnd);
	~CDropTargetImage(void);

protected:
	virtual HRESULT STDMETHODCALLTYPE Drop(__RPC__in_opt IDataObject *pDataObj,DWORD grfKeyState,POINTL pt,__RPC__inout DWORD *pdwEffect);
	virtual HRESULT STDMETHODCALLTYPE DragOver(DWORD grfKeyState,POINTL pt,__RPC__inout DWORD *pdwEffect);
	virtual HRESULT STDMETHODCALLTYPE DragLeave( void);
};

