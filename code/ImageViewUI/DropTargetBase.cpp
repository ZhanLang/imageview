#include "StdAfx.h"
#include "DropTargetBase.h"


CDropTargetBase::CDropTargetBase(void)
{
	nRef=0;
}


CDropTargetBase::~CDropTargetBase(void)
{
}


//////////////////////////////////////////////////////////////////////////
// IUnknown
HRESULT STDMETHODCALLTYPE CDropTargetBase::QueryInterface( 
	/* [in] */ REFIID riid,
	/* [iid_is][out] */ __RPC__deref_out void __RPC_FAR *__RPC_FAR *ppvObject)
{
	HRESULT hr=S_FALSE;
	if(riid==__uuidof(IUnknown))
		*ppvObject=(IUnknown*) this,hr=S_OK;
	else if(riid==__uuidof(IDropTarget))
		*ppvObject=(IDropTarget*)this,hr=S_OK;
	if(SUCCEEDED(hr)) AddRef();
	return hr;

}

ULONG STDMETHODCALLTYPE CDropTargetBase::AddRef( void){return ++nRef;}

ULONG STDMETHODCALLTYPE CDropTargetBase::Release( void) { 
	ULONG uRet= -- nRef;
	if(uRet==0) delete this;
	return uRet;
}

//////////////////////////////////////////////////////////////////////////
// IDropTarget

HRESULT STDMETHODCALLTYPE CDropTargetBase::DragEnter( 
	/* [unique][in] */ __RPC__in_opt IDataObject *pDataObj,
	/* [in] */ DWORD grfKeyState,
	/* [in] */ POINTL pt,
	/* [out][in] */ __RPC__inout DWORD *pdwEffect)
{
	*pdwEffect=DROPEFFECT_LINK;
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CDropTargetBase::DragOver( 
	/* [in] */ DWORD grfKeyState,
	/* [in] */ POINTL pt,
	/* [out][in] */ __RPC__inout DWORD *pdwEffect)
{
	*pdwEffect=DROPEFFECT_LINK;
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CDropTargetBase::DragLeave( void)
{
	return S_OK;
}