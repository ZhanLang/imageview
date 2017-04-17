#pragma once

#include <GdiPlus.h>
using namespace Gdiplus;

typedef struct tagSize
{
	float        cx;
	float        cy;
} _SIZE, *_PSIZE, *_LPSIZE;


class CGdiPlusImage
{
	//变量定义
protected:
	Image *							m_pImage;							//图片对象
	tagSize							m_szZoom;							//缩放变量
	int								m_nRotateAngle;						//旋转角度
	BYTE							m_cbAlphaDepth;						//Alpha数值

public:
	CGdiPlusImage();
	virtual ~CGdiPlusImage();
	
public:
	bool IsNull();
	INT GetWidth();
	INT GetHeight();
public:

	bool DestroyImage();
	bool LoadImage(LPCTSTR pszFileName);

public:
	//设置旋转
	void SetRotation(int nAngle);
	//设置缩放
	void SetScale(float cx,float cy);
	//设置Alpha
	void SetAlpha(BYTE cbAlphaDepth);
	//保存图像
	bool Save(LPCTSTR pszFileName,LPCTSTR format=TEXT("image/png"));
	//获取解码器
	int GetEncoderClsid(LPCTSTR format, CLSID* pClsid);

	//绘画函数
public:
	//绘画图像
	bool DrawImage(HDC pDC, CRect rcDraw);
};
