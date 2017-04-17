#include "StdAfx.h"
#include "GdiPlusImage.h"
#include "ximage.h"
//构造函数
CGdiPlusImage::CGdiPlusImage(void)
{
	m_pImage=NULL;
	m_szZoom.cx = m_szZoom.cy = 1.0f;
	m_nRotateAngle = 0;
	m_cbAlphaDepth = 255;
}

CGdiPlusImage::~CGdiPlusImage(void)
{
	DestroyImage(); 
}

bool CGdiPlusImage::IsNull()
{
	if (m_pImage==NULL) return true;
	if (m_pImage->GetLastStatus()!=Ok) return true;

	return false;
}

INT CGdiPlusImage::GetWidth()
{
	if (IsNull()==true) return 0;
	return m_pImage->GetWidth();
}

INT CGdiPlusImage::GetHeight()
{
	
	if (IsNull()==true) return 0;

	return m_pImage->GetHeight();
}

bool CGdiPlusImage::DestroyImage()
{
	if (m_pImage!=NULL) 
	{
		delete m_pImage;
		m_pImage = NULL;
	}

	return true;
}

bool CGdiPlusImage::LoadImage(LPCTSTR pszFileName)
{

	if (m_pImage!=NULL) return false;

	int nImageType = CXIMAGE_FORMAT_UNKNOWN;

	LPCTSTR lpszExt = PathFindExtension( pszFileName );
	if ( lpszExt && _tcslen( lpszExt) && lpszExt++ )
		nImageType = CxImage::GetTypeIdFromName(lpszExt);

	if ( nImageType == CXIMAGE_FORMAT_UNKNOWN)
		return FALSE;

	if ( nImageType == CXIMAGE_SUPPORT_JPG || CXIMAGE_SUPPORT_PNG)
	{
		CT2CW strFileName(pszFileName);
		m_pImage=Image::FromFile((LPCWSTR)strFileName);
	}else
	{
		CxImage xImage( pszFileName, nImageType );

		if ( !xImage.IsValid())
			return FALSE;

		HBITMAP hBitmap = xImage.MakeBitmap(NULL, true);
		m_pImage = new Gdiplus::Bitmap(hBitmap, NULL);
		DeleteObject(hBitmap);
	}

	
	

	if ((m_pImage==NULL)||(m_pImage->GetLastStatus()!=Ok)) 
	{
		DestroyImage();
		return false;
	}

	return true;
}

bool CGdiPlusImage::DrawImage(HDC pDC, CRect rcDraw)
{

	if (m_pImage==NULL) return false;

	Graphics graphics(pDC);

	GraphicsContainer Containter = graphics.BeginContainer();
	graphics.SetSmoothingMode(SmoothingModeHighQuality);

	INT nImageWidth=m_pImage->GetWidth();
	INT nImageHeight=m_pImage->GetHeight();

	
	RectF rcDrawRect;
	rcDrawRect.X= rcDraw.top;
	rcDrawRect.Y=rcDraw.left;
	rcDrawRect.Width=(REAL)rcDraw.Width();
	rcDrawRect.Height=(REAL)rcDraw.Height();

	Matrix matrix;

	CPoint pt((rcDraw.Width()+nImageWidth/2), (rcDraw.Height()+nImageHeight/2));
	PointF ptBase((float)pt.x,(float)pt.y);

	//矩阵变换要注意顺序，先平移在缩放后旋转
	matrix.Translate((REAL)rcDraw.Width(),(REAL)rcDraw.Height(),MatrixOrderAppend);
	
	//缩放
	if ( m_szZoom.cx != 1.0f || m_szZoom.cy != 1.0f )
	{
		ptBase.X += m_szZoom.cx;
		ptBase.Y += m_szZoom.cy;

		matrix.Translate(-ptBase.X,-ptBase.Y,MatrixOrderAppend);
		matrix.Scale(m_szZoom.cx,m_szZoom.cy,MatrixOrderAppend);
		matrix.Translate(ptBase.X,ptBase.Y,MatrixOrderAppend);
	}

	//旋转
	if (m_nRotateAngle)
	{
		matrix.RotateAt((REAL)m_nRotateAngle,ptBase,MatrixOrderAppend);
	}

	graphics.SetTransform(&matrix);

	/*
	//透明矩阵
	ColorMatrix Matrix=
	{
		1.0f,0.0f,0.0f,0.0f,0.0f, 
		0.0f,1.0f,0.0f,0.0f,0.0f, 
		0.0f,0.0f,1.0f,0.0f,0.0f,
		0.0f,0.0f,0.0f,m_cbAlphaDepth/255.0f,0.0f, 
		0.0f,0.0f,0.0f,0.0f,1.0f
	};

	//设置属性
	ImageAttributes Attributes; 
	Attributes.SetColorMatrix(&Matrix,ColorMatrixFlagsDefault,ColorAdjustTypeBitmap); 
	*/
	//绘画图像
	graphics.DrawImage(m_pImage,rcDrawRect,0,0,(REAL)nImageWidth,(REAL)nImageHeight,UnitPixel,NULL);

	graphics.ResetTransform();

	graphics.EndContainer(Containter);

	return true;
}

void CGdiPlusImage::SetRotation( int nAngle)
{
	m_nRotateAngle = nAngle;
}

void CGdiPlusImage::SetScale(float cx,float cy)
{
	m_szZoom.cx = cx;
	m_szZoom.cy = cy;
}

void CGdiPlusImage::SetAlpha( BYTE cbAlphaDepth )
{
	if ( cbAlphaDepth < 0 ) m_cbAlphaDepth = 0;
	if ( cbAlphaDepth > 255 ) m_cbAlphaDepth = 255;
	
	m_cbAlphaDepth = cbAlphaDepth;
}

int CGdiPlusImage::GetEncoderClsid(LPCTSTR format, CLSID* pClsid)
{
	UINT  num = 0;          
	UINT  size = 0;        

	ImageCodecInfo* pImageCodecInfo = NULL;   

	GetImageEncodersSize(&num, &size);   
	if(size == 0)   
		return -1;  

	pImageCodecInfo = (ImageCodecInfo*)(malloc(size));   
	if(pImageCodecInfo == NULL)   
		return -1;  

	GetImageEncoders(num, size, pImageCodecInfo);   

	for(UINT j = 0; j < num; ++j)   
	{   
		if( StrCmp(pImageCodecInfo[j].MimeType, format) == 0 )   
		{   
			*pClsid = pImageCodecInfo[j].Clsid;   
			free(pImageCodecInfo);   
			return j; 
		}       
	}   

	free(pImageCodecInfo);   
	return -1;  
}

bool CGdiPlusImage::Save(LPCTSTR pszFileName,LPCTSTR format)
{
	//加载文件
	CT2CW strFileName(pszFileName);

	//图像保存
	CLSID Clsid;
	GetEncoderClsid(format, &Clsid);
	if( m_pImage->Save((LPCWSTR)strFileName, &Clsid) == Ok )
		return true;

	return false;
}

