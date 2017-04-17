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
	//��������
protected:
	Image *							m_pImage;							//ͼƬ����
	tagSize							m_szZoom;							//���ű���
	int								m_nRotateAngle;						//��ת�Ƕ�
	BYTE							m_cbAlphaDepth;						//Alpha��ֵ

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
	//������ת
	void SetRotation(int nAngle);
	//��������
	void SetScale(float cx,float cy);
	//����Alpha
	void SetAlpha(BYTE cbAlphaDepth);
	//����ͼ��
	bool Save(LPCTSTR pszFileName,LPCTSTR format=TEXT("image/png"));
	//��ȡ������
	int GetEncoderClsid(LPCTSTR format, CLSID* pClsid);

	//�滭����
public:
	//�滭ͼ��
	bool DrawImage(HDC pDC, CRect rcDraw);
};
