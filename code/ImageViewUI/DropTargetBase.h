#pragma once
class CDropTargetBase :public IDropTarget
{
public:
	CDropTargetBase(void);
	~CDropTargetBase(void);

	// IUnknown
	virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, __RPC__deref_out void __RPC_FAR *__RPC_FAR *ppvObject);
	virtual ULONG STDMETHODCALLTYPE AddRef( void);
	virtual ULONG STDMETHODCALLTYPE Release( void);


	//IDropTarget
	virtual HRESULT STDMETHODCALLTYPE DragEnter( __RPC__in_opt IDataObject *pDataObj,DWORD grfKeyState,POINTL pt,__RPC__inout DWORD *pdwEffect);
	virtual HRESULT STDMETHODCALLTYPE DragOver(DWORD grfKeyState,POINTL pt,__RPC__inout DWORD *pdwEffect);
	virtual HRESULT STDMETHODCALLTYPE DragLeave( void);
private:
	int nRef;
};

