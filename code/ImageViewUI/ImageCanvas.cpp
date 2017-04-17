#include "StdAfx.h"
#include "ImageCanvas.h"
#include <helper/MemDC.h>
#include "..\FreeImage\Source\FreeImage.h"
#include <imagestone/include/image_codec/codecfactory_freeimage.h>
#include "imagestone/include/effect/effect_ext.h"
#include <ximaico.h>
#include <util/memmapfile.h>
using namespace Gdiplus;
#define EVENT_AMIN_TIEM_ID	100


bool mem_to_global( const void *buf, size_t size, HGLOBAL global )
{
	void *dest = ::GlobalLock( global );
	if( dest == NULL )
	{
		return false;
	}
	memcpy( dest, buf, size );
	::GlobalUnlock( global );
	return true;
}

Gdiplus::Image *mi_from_memory( const void *buf, size_t size )
{
	IStream *stream = NULL;
	HGLOBAL global = ::GlobalAlloc( GMEM_FIXED, size );
	if( global == NULL )
	{
		return NULL;
	}
	/* copy the buf content to the HGLOBAL */
	if( !mem_to_global( buf, size, global ) )
	{
		::GlobalFree( global );
		return NULL;
	}
	/* get the IStream from the global object */
	if( ::CreateStreamOnHGlobal( global, TRUE, &stream ) != S_OK )
	{
		::GlobalFree( global );
		return NULL;
	}
	/* create the image from the stream */
	Gdiplus::Image *image = Gdiplus::Image::FromStream( stream );

	stream->Release();
	/* i suppose when the reference count for stream is 0, it will 
	GlobalFree automatically. The Image maintain the object also.*/	
	return image;
}

Gdiplus::Image* LoadImage( LPCTSTR lpszFile)
{
	CMemMapFile memMap;
	if ( !memMap.MapFile(lpszFile, TRUE) )
	{
		return NULL;
	}
	
	return mi_from_memory( memMap.GetBuffer(), memMap.GetLength());
}

Gdiplus::Image* LoadImage( HBITMAP hBmp)
{
	DIBSECTION  dibsection = {0};
	int nBytes = ::GetObject( hBmp, sizeof( DIBSECTION ), &dibsection );

	Gdiplus::Bitmap* bitmap = 0;

	if(dibsection.dsBm.bmBitsPixel != 32)  
	{
		bitmap = Gdiplus::Bitmap::FromHBITMAP(hBmp, NULL);
	}
	else
	{
		int         width, height, pitch;
		LPVOID      bits = NULL;

		width = dibsection.dsBm.bmWidth;
		height = abs( dibsection.dsBm.bmHeight );
		pitch = (((width*dibsection.dsBm.bmBitsPixel)+31)/32)*4;        //计算行宽，四字节对齐 ATL::CImage::ComputePitch // 32位位图不存在对齐问题，so其实没必要
		bits = 0;
		// 
		// 		if( dibsection.dsBmih.biHeight > 0 ) // 对于DDB，不会取到dsBmih数据，所以biHeight成员始终为0
		// 		{
		// 			bits = LPBYTE( bits )+((height-1)*pitch);           
		// 			pitch = -pitch;
		// 		}

		bitmap = new Gdiplus::Bitmap(width, height, pitch, PixelFormat32bppARGB, /*static_cast< BYTE* >(bits )*/NULL);

		if(0 == bits)
		{
			BitmapData   bitmapData; 
			Rect rc(0, 0, width, height);
			bitmap->LockBits(&rc, ImageLockModeWrite, PixelFormat32bppARGB, &bitmapData);
			GetBitmapBits(hBmp, pitch * height, bitmapData.Scan0);      // 上面的bits在biHeight>0时要倒置的，但是这里不知道要不要，也不好测试
			bitmap->UnlockBits(&bitmapData);
		}
	}
	return bitmap;
}

BOOL LoadImage(CxImage&image, LPCTSTR lpszFile, int imagetype)
{
	CMemMapFile memMap;
	if ( !memMap.MapFile(lpszFile, TRUE) )
		return FALSE;
	//uint8_t * buffer, uint32_t size, uint32_t imagetype
	return image.Decode((uint8_t *)memMap.GetBuffer(), memMap.GetLength(), imagetype) ? TRUE : FALSE;
}

BOOL LoadImage(FCObjImage& image,LPCTSTR lpszFile, IMAGE_TYPE imagetype)
{
	CMemMapFile memMap;
	if ( !memMap.MapFile(lpszFile, TRUE) )
		return FALSE;
	//uint8_t * buffer, uint32_t size, uint32_t imagetype
	return image.Load(memMap.GetBuffer(), memMap.GetLength(), imagetype) ? TRUE : FALSE;
}

class GlobalImageStoneInit
{
public:
	GlobalImageStoneInit()
	{
		FCObjImage::SetImageCodecFactory (new FCImageCodecFactory_FreeImage) ;
	}
};

GlobalImageStoneInit globalImageStoneInit;
CImageCanvas::CImageCanvas()
{
	m_bLayoutInited = FALSE;
	
	m_evtSet.addEvent(EVENTID(EventCanvasMouseMove));
	m_evtSet.addEvent(EVENTID(EventCanvasWndMove));
	m_evtSet.addEvent(EVENTID(EventCanvasFrameChange));
	m_evtSet.addEvent(EVENTID(EventCanvasImageLoad));
	//m_bPaintBit = FALSE;
	m_dwImageWidth = 0;
	m_dwImageHeight = 0;
	m_nImageType = CXIMAGE_FORMAT_UNKNOWN;
	m_nFramePosition = 0;
	m_bMulitFrame = 0;
	m_nFrameCount = 0;
	m_bAnime = FALSE;
	m_pImage = NULL;
	m_bAniming =FALSE;
}

CImageCanvas::~CImageCanvas(void)
{
	DestoryImage();
}






BOOL CImageCanvas::SetImageFile(LPCTSTR lpszImageFile)
{
	DestoryImage();
	m_strImageFile = lpszImageFile;
	EventCanvasImageLoad evtImageLoad(this);
	
	if ( GetFileAttributes(lpszImageFile) == INVALID_FILE_ATTRIBUTES )
		evtImageLoad.bResult = FALSE;
	else
		evtImageLoad.bResult = Load();

	FireEvent(evtImageLoad);
	return evtImageLoad.bResult;
}


DWORD CImageCanvas::GetImageWidth()
{
	Gdiplus::Image* image = NULL;
	if ( m_pImage )
		image = m_pImage;
	else if ( m_BitmapFrame.size())
		image = m_BitmapFrame[m_nFramePosition];

	if ( image) return image->GetWidth();

	return 0;
}

DWORD CImageCanvas::GetImageHeight()
{
	Gdiplus::Image* image = NULL;
	if ( m_pImage )
		image = m_pImage;
	else if ( m_BitmapFrame.size())
		image = m_BitmapFrame[m_nFramePosition];
	if ( image) return image->GetHeight();

	return 0;
}

BOOL CImageCanvas::IsImageValide()
{

	Gdiplus::Image* image = NULL;
	if ( m_pImage )
		image = m_pImage;
	else if ( m_BitmapFrame.size())
		image = m_BitmapFrame[m_nFramePosition];
	

	if ( ! ( image && Gdiplus::ImageTypeUnknown != image->GetType() )) 
		return FALSE;

	return TRUE;
}

void CImageCanvas::OnSize(UINT nType, SOUI::CSize size)
{
	SWindow::OnSize(0, size);
	EventCanvasWndMove evtHover(this);
	FireEvent(evtHover);
}

int CImageCanvas::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
#ifdef DWMBLUR	//win7毛玻璃开关
	MARGINS mar = {5,5,30,5};
	DwmExtendFrameIntoClientArea ( m_hWnd, &mar );
#endif
	return 0;
}

BOOL CImageCanvas::OnInitDialog(HWND wndFocus, LPARAM lInitParam)
{
	return TRUE;
}

void CImageCanvas::OnDestory()
{
	SetMsgHandled(FALSE);
}

void CImageCanvas::OnClose()
{

}

void CImageCanvas::OnUpdateFloatPosition(const SOUI::CRect & rcParent)
{

}

LRESULT CImageCanvas::OnExecute(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL/* bHandled*/)
{
	auto func = (std::function<void()> *)lParam;
	(*func)();
	delete func;
	return 0;
}

typedef struct tagSize
{
	float        cx;
	float        cy;
} _SIZE, *_PSIZE, *_LPSIZE;

void CImageCanvas::OnPaint(IRenderTarget *pRT)
{
	SOUI::CRect rect = GetClientRect();
	
	//获取属性
	Gdiplus::Image* image = NULL;
	if ( m_pImage )
		image = m_pImage;
	else if ( m_BitmapFrame.size())
		image = m_BitmapFrame[m_nFramePosition];
	 

	if ( image )
	{
		INT nImageWidth = image->GetWidth();
		INT nImageHeight = image->GetHeight();

		HDC hDc = pRT->GetDC();
		Gdiplus::Graphics graphics(hDc);
		Gdiplus::GraphicsContainer Containter = graphics.BeginContainer();
		graphics.SetSmoothingMode(Gdiplus::SmoothingModeHighQuality);

		Gdiplus::RectF rcDrawRect;
		rcDrawRect.X = rect.left;//(REAL)nXPos;
		rcDrawRect.Y = rect.top;//(REAL)nYPos;
		rcDrawRect.Width=rect.Width();
		rcDrawRect.Height=rect.Height();

		SOUI::CRect r = GetParent()->GetClientRect();

		graphics.DrawImage(image,rcDrawRect,0,0,(Gdiplus::REAL)nImageWidth,(Gdiplus::REAL)nImageHeight,Gdiplus::UnitPixel,/*&Attributes*/NULL);
		graphics.ResetTransform();
		graphics.EndContainer(Containter);
		pRT->ReleaseDC(hDc);
	}
}

void CImageCanvas::OnPaint(HDC hDC)
{
	/*
	return ;;
	SetMsgHandled(TRUE);

	SOUI::CRect rect = GetWindowRect();
	if ( !m_img.IsValidImage() )
		return ;

	if(m_img.Width()<=rect.Width() && m_img.Height() <= rect.Width()) //小图片，不缩放  
	{
		SOUI::CPoint x = rect.CenterPoint();
		x.Offset(-(m_img.Width() / 2) , - (m_img.Height() / 2));

		SOUI::CPoint y = rect.CenterPoint();
		y.Offset(m_img.Width() / 2 , m_img.Height() / 2);

		m_img.Draw(hDC,SOUI::CRect(x,y));
	}
	else
	{
		//缩放比例
		float xScale=(float)rect.Width()/(float)m_img.Width();  
		float yScale=(float)rect.Height()/(float)m_img.Height();  

		float ScaleIndex = (xScale >= yScale ? yScale : xScale);  //选择一个较小的比例

		int width = ScaleIndex * m_img.Width();
		int height = ScaleIndex * m_img.Height();


		SOUI::CPoint x = rect.CenterPoint();
		x.Offset(-(width / 2) , - (height / 2));

		SOUI::CPoint y = rect.CenterPoint();
		y.Offset(width / 2 , height / 2);

		m_img.Draw(hDC,SOUI::CRect(x,y));
	}
	*/
}

void CImageCanvas::OnNcPaint(IRenderTarget *pRT)
{
	SetMsgHandled(FALSE);
}

BOOL CImageCanvas::OnEraseBkgnd(IRenderTarget *pRT)
{
	return TRUE;
}

VOID CImageCanvas::OnMouseMove(UINT nFlags,SOUI::CPoint pt)
{
	SOUI::CRect rcTemp = GetClientRect();
	__super::OnMouseMove(nFlags, pt);
	if ( rcTemp != GetClientRect() )
	{
		EventCanvasWndMove evtHover(this);
		FireEvent(evtHover);
	}
	EventCanvasMouseMove evtHover(this);
	evtHover.point = pt;
	FireEvent(evtHover);
}

void CImageCanvas::OnLButtonDown(UINT nFlags,SOUI::CPoint pt)
{
	__super::OnLButtonDown(nFlags, pt);
}

void CImageCanvas::OnLButtonUp(UINT nFlags,SOUI::CPoint pt)
{
	__super::OnLButtonUp(nFlags, pt);
	//Invalidate();
}

void CImageCanvas::OnTimer(char cTimerID)
{
	if ( cTimerID != EVENT_AMIN_TIEM_ID )
		return;

	if ( m_pPropertyItem == NULL )
	{
		return ;
	}

	KillTimer( EVENT_AMIN_TIEM_ID );
	this->Invalidate();

	NextFrame();
	long lPause = ((long*) m_pPropertyItem->value)[m_nFramePosition] * 10;
	if ( lPause == 0 ) lPause = 100;

	

	SetTimer(EVENT_AMIN_TIEM_ID, lPause );
}

VOID CImageCanvas::DestoryImage()
{
	if ( m_bAniming )
		PauseAnim();
	
	if ( m_pImage )
	{
		delete m_pImage;
		m_pImage = NULL;
	}

	for ( int nLoop = 0 ; nLoop < m_BitmapFrame.size(); nLoop++)
	{
		delete m_BitmapFrame[nLoop];
	}
	m_BitmapFrame.clear();

	m_dwImageWidth = 0;
	m_dwImageHeight = 0;
	m_nImageType = CXIMAGE_FORMAT_UNKNOWN;
	m_nFramePosition = 0;
	m_bMulitFrame = 0;
	m_nFrameCount = 0;
	m_bAnime = FALSE;
	m_pImage = NULL;
	m_bAniming =FALSE;
	m_strImageFile.Empty();
}

BOOL CImageCanvas::Load()
{
	//DestoryImage();
	m_BitmapFrame.clear();
	m_bAnime = FALSE;
	m_nImageType = CXIMAGE_FORMAT_UNKNOWN;
	LPCTSTR lpszExt = PathFindExtension( m_strImageFile );
	if ( lpszExt && _tcslen( lpszExt) && lpszExt++ )
		m_nImageType = CxImage::GetTypeIdFromName(lpszExt);

	if ( m_nImageType == CXIMAGE_FORMAT_UNKNOWN)
		return FALSE;

	switch( m_nImageType)
	{
	case CXIMAGE_FORMAT_PNG:
	case CXIMAGE_FORMAT_JPG:
	case CXIMAGE_FORMAT_BMP:
		{
			m_pImage =  LoadImage((LPCWSTR)m_strImageFile);
			if ( !( m_pImage && Gdiplus::ImageTypeUnknown != m_pImage->GetType() ))
				break;

			break;
		}

	case CXIMAGE_FORMAT_GIF:
		{
			m_pImage =  LoadImage((LPCWSTR)m_strImageFile);
			if ( !( m_pImage && Gdiplus::ImageTypeUnknown != m_pImage->GetType() ))
				break;

			//动画类
			UINT nCount = m_pImage->GetFrameDimensionsCount();
			GUID* pDimensionIDs	=	new GUID[ nCount ];
			m_pImage->GetFrameDimensionsList( pDimensionIDs, nCount );
			m_nFrameCount	=	m_pImage->GetFrameCount( &pDimensionIDs[0] );

			int nSize		=	m_pImage->GetPropertyItemSize(PropertyTagFrameDelay );
			m_pPropertyItem	=	(Gdiplus::PropertyItem*) malloc( nSize );

			Gdiplus::Status st = m_pImage->GetPropertyItem( PropertyTagFrameDelay, nSize, m_pPropertyItem );
			if ( st == Gdiplus::PropertyNotFound )
			{
				free(m_pPropertyItem);
				m_pPropertyItem = NULL;
			}

			delete[]  pDimensionIDs;
			pDimensionIDs = NULL;

			if ( m_nFrameCount >1 )
			{
				m_bAnime = TRUE;
				m_bMulitFrame = TRUE;
			}

			break;
		}

	case CXIMAGE_FORMAT_ICO:
	case CXIMAGE_FORMAT_TIF:
		{
			/*
			std::vector<FCObjImage*> rImageList;
			if ( !FCObjImage::Load(m_strImageFile, rImageList) )
				break;

			for ( int i = 0 ; i < rImageList.size() ; i++ )
			{
				FCObjImage& image = *(rImageList[i]);

				Image* pImage = LoadImage(image);
				if ( pImage )
					m_BitmapFrame.push_back(pImage);

			}
			*/


			CxImage img;  
			if ( !img.Load(m_strImageFile, m_nImageType))
				break;

			int iNumFrames = img.GetNumFrames();  
			for (int i = 0; i < iNumFrames; i++) {  
				CxImage newImage;  
				newImage.SetFrame(i);  
				if ( newImage.Load(m_strImageFile, m_nImageType) )
				{
					HBITMAP hBitmap = newImage.MakeBitmap(NULL, true);
					if ( hBitmap )
					{
						Image* pImage = LoadImage(hBitmap);
						if ( pImage )
							m_BitmapFrame.push_back(pImage);
						DeleteObject(hBitmap);
					}
				}
			}  


			m_nFrameCount = m_BitmapFrame.size();
			m_bMulitFrame = m_nFrameCount > 1 ? TRUE : FALSE;
			break;
		}

	case CXIMAGE_FORMAT_PSD:
		{
			FCObjImage image;
			if ( !LoadImage(image, m_strImageFile, IMG_PSD) )
				break;

			if ( image.ColorBits() == 32)
				image.ApplyEffect (FCEffectPremultipleAlpha()) ;
			m_pImage = LoadImage(image);
			break;
		}
	default:
		{
			CxImage image;
			if ( !LoadImage(image, m_strImageFile, m_nImageType ) )
				break;

			HBITMAP hBitmap = image.MakeBitmap(NULL, true);
			if ( hBitmap )
			{
				m_pImage = LoadImage(hBitmap);
				DeleteObject(hBitmap);
			}
		}
	}

	

	this->Invalidate();
	if (IsImageValide() && IsMutiframe() )
	{

		EventCanvasFrameChange evtHover(this);
		FireEvent(evtHover);
	}

	return IsImageValide();
}

BOOL CImageCanvas::IsAnimation()
{
	return m_bAnime;
}

BOOL CImageCanvas::IsMutiframe()
{
	return m_bMulitFrame;
}

BOOL CImageCanvas::IsAniming()
{
	return m_bAniming;
}

BOOL CImageCanvas::NextFrame()
{
	if ( m_bMulitFrame)
	{
		if ( m_nFramePosition < m_nFrameCount )
		{
			m_nFramePosition = (++m_nFramePosition) % m_nFrameCount;
			GUID pageGuid = Gdiplus::FrameDimensionTime;
			if ( m_pImage )
				m_pImage->SelectActiveFrame( &pageGuid, m_nFramePosition );
			
			
			this->Invalidate();
			EventCanvasFrameChange evtHover(this);
			FireEvent(evtHover);

			return TRUE;
		}
	}

	return FALSE;
}

BOOL CImageCanvas::PrevFrame()
{
	if ( m_bMulitFrame)
	{
		if ( m_nFramePosition > 0 )
		{
			m_nFramePosition = (--m_nFramePosition) % m_nFrameCount;
			GUID pageGuid = Gdiplus::FrameDimensionTime;
			if ( m_pImage )
				m_pImage->SelectActiveFrame( &pageGuid, m_nFramePosition );
			this->Invalidate();
			EventCanvasFrameChange evtHover(this);
			FireEvent(evtHover);
			return TRUE;
		}
	}

	return FALSE;
}

UINT CImageCanvas::GetCurFrame()
{
	return m_nFramePosition;
}

UINT CImageCanvas::GetFrameCount()
{
	return m_nFrameCount;
}

BOOL CImageCanvas::StartAnim()
{
	if ( m_bAnime )
	{
		if ( !m_pPropertyItem )
		{
			return FALSE;
		}

		m_bAniming = TRUE;
		long lPause = ((long*) m_pPropertyItem->value)[m_nFramePosition] * 10;
		if ( lPause == 0 ) lPause = 100;
		return SetTimer(EVENT_AMIN_TIEM_ID, lPause );
	}

	return FALSE;
}

BOOL CImageCanvas::PauseAnim()
{
	if ( m_bAniming && m_bAnime )
	{
		KillTimer( EVENT_AMIN_TIEM_ID );
		m_bAniming = FALSE;
	}
	return FALSE;
}

BOOL CImageCanvas::CanEdition()
{
	if ( m_nImageType == CXIMAGE_FORMAT_UNKNOWN)
		return FALSE;

	HANDLE hFile = CreateFile( m_strImageFile, GENERIC_READ | GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL,NULL);
	if ( hFile == INVALID_HANDLE_VALUE )
		return FALSE;
	CloseHandle(hFile);


	for ( int n = 0 ; n < _countof( doctypes) ; n++ )
	{
		if ( doctypes[n].nID == m_nImageType )
			return doctypes[n].bWrite;
	}

	return FALSE;
}

BOOL CImageCanvas::RotateLeft()
{
	bool bRet = false;
	CxImage image;
	if ( image.Load(m_strImageFile, m_nImageType) )
		bRet = image.RotateLeft();
	
	if ( bRet )
	{
		SStringT strImgFile = m_strImageFile;
		image.Save(m_strImageFile, m_nImageType);
		return SetImageFile(strImgFile);
	}
	
	return FALSE;
}

BOOL CImageCanvas::RotateRight()
{
	bool bRet = false;
	CxImage image;
	if ( image.Load(m_strImageFile, m_nImageType) )
		bRet = image.RotateRight();

	if ( bRet )
	{
		SStringT strImgFile = m_strImageFile;
		image.Save(m_strImageFile, m_nImageType);
		return SetImageFile(strImgFile);
	}

	return FALSE;
}

LPCTSTR CImageCanvas::GetImageFile()
{
	return m_strImageFile;
}
