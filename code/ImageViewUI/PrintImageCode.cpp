/*
作者：陈明 [http://blog.csdn.net/qmroom]
(转载请注明出处)

文章"Print a Image[bmp, jpg, gif, png...] with full page by gdiplus"的源代码：
http://blog.csdn.net/qmroom/archive/2008/06/05/2512568.aspx
*/


#include "stdafx.h"
#include <atlbase.h>
#include <Windows.h>
#include <winspool.h>
#include <gdiplus.h>
#include <Gdiplusgraphics.h>

// #include "Common.h"
// #include "Setting.h"

#pragma comment(lib, "Winspool.lib")
#pragma comment(lib, "gdiplus.lib")

using namespace Gdiplus;

BOOL FileExist(LPCTSTR sFilePath)
{
	//PathFileExists( sFilePath )
	WIN32_FIND_DATA w32fd;
	HANDLE hFile=FindFirstFile(sFilePath,&w32fd);
	if(hFile!=INVALID_HANDLE_VALUE)
	{
		FindClose(hFile);
		return TRUE;
	}

	return FALSE; 
}

BOOL FolderExist(LPCTSTR strPath)
{
	WIN32_FIND_DATA wfd;
	BOOL rValue = FALSE;
	HANDLE hFind;
	int maxIndex = lstrlen(strPath) - 1;
	if (strPath[maxIndex] == TEXT('\\') || strPath[maxIndex] == TEXT('/'))
	{
		TCHAR sPath[MAX_PATH];
		lstrcpy(sPath,strPath);
		sPath[maxIndex] = TEXT('\0');
		hFind = FindFirstFile(sPath, &wfd);
	}
	else
	{
		hFind = FindFirstFile(strPath, &wfd);
	}
	if ((hFind != INVALID_HANDLE_VALUE) && (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
	{
		rValue = TRUE;
	}
	FindClose(hFind);
	return rValue;
}

BOOL GetParentFolder(LPTSTR strPath)
{
	int i,iLen;

	iLen = static_cast<int>(_tcslen(strPath));
	if(strPath[iLen-1] == TEXT('\\'))
		strPath[--iLen] = TEXT('\0');

	if(strPath[iLen-1] == TEXT(':') ||
		(iLen > 1 && strPath[iLen-1] == TEXT('.') && strPath[iLen-2] == TEXT('.')) ||
		(iLen == 1 && strPath[iLen-1] == TEXT('.')))
		return FALSE;

	for (i=iLen - 1; i >= 0; i--)
	{
		if(strPath[i] == TEXT('\\'))
		{
			strPath[i] = TEXT('\0');
			return TRUE;
		}
	}

	return FALSE;
}

BOOL CreateFolder(LPCTSTR strPath)
{
	if(FolderExist(strPath))
	{
		return TRUE;
	}
	else
	{
		TCHAR sPath[MAX_PATH];
		lstrcpy(sPath, strPath);

		if(GetParentFolder(sPath))
			CreateFolder(sPath);
	}

	if(!::CreateDirectory(strPath, NULL))
	{
		if(183 == GetLastError())	//文件夹已经存在
			return TRUE;
		return FALSE;
	}
	return TRUE;
}

BOOL DeleteFolder(LPCTSTR sPath)//如删除   DeleteFolder("c:\\aaa")  或 DeleteFolder("c:\\aaa\\")     
{
	CFileFind   tempFind;   
	CString		tempFileFind;
	TCHAR		DirName[MAX_PATH];

	lstrcpy(DirName,sPath);
	FormatFolderName2(DirName);
	tempFileFind.Format(TEXT("%s\\*.*"),DirName);

	BOOL  IsFinded=tempFind.FindFile(tempFileFind);   
	while(IsFinded)
	{
		IsFinded=tempFind.FindNextFile();
		if(!tempFind.IsDots())
		{
			if(tempFind.IsDirectory())
			{
				CString   tempDir;
				tempDir.Format(TEXT("%s\\%s"),DirName,tempFind.GetFileName());
				DeleteFolder((LPCTSTR)tempDir);
			}   
			else
			{
				CString   tempFileName;
				tempFileName.Format(TEXT("%s\\%s"),DirName,tempFind.GetFileName());
				DeleteFile((LPCTSTR)tempFileName);
			}
		}
	}
	tempFind.Close();
	if(!RemoveDirectory(DirName))
	{
		//MessageBox(0,"删除目录失败！","警告信息",MB_OK);//比如没有找到文件夹,删除失败，可把此句删除
		return   FALSE;
	}
	return   TRUE;   
}

BOOL FormatFolderName(LPTSTR sPath)
{
	if (NULL == sPath)
		return FALSE;
	int len = lstrlen(sPath);
	if (sPath[len-1] != TEXT('\\') && sPath[len-1] != TEXT('/'))
	{
		lstrcat(sPath, TEXT("\\"));
	}
	return TRUE;
}
BOOL FormatFolderName2(LPTSTR sPath)
{
	if (NULL == sPath)
		return FALSE;
	int maxIndex = lstrlen(sPath) - 1;
	while(maxIndex >= 0 && (sPath[maxIndex] == TEXT('\\') || sPath[maxIndex] == TEXT('/')))
	{
		sPath[maxIndex] = TEXT('\0');
		maxIndex--;
	}
	return TRUE;
}

BOOL GetFileOnlyName(LPTSTR fileFullPath)
{
	int i;
	int len = lstrlen(fileFullPath);
	int start = 0;
	for (i=len-1; i >= 0; i--)
	{
		if (fileFullPath[i] == TEXT('\\') || fileFullPath[i] == TEXT('/'))
		{
			start = i+1;
			break;
		}
	}
	TCHAR sPath[MAX_PATH];
	lstrcpy(sPath, fileFullPath+start);
	lstrcpy(fileFullPath, sPath);
	return TRUE;
}

BOOL GetFileOnlyMainName(LPTSTR fileFullPath)
{
	int i;
	int len = lstrlen(fileFullPath);
	int start = 0;
	int pointStart = 0;
	BOOL pointFlag(FALSE);
	for (i=len-1; i >= 0; i--)
	{
		if (fileFullPath[i] == TEXT('\\') || fileFullPath[i] == TEXT('/'))
		{
			start = i+1;
			break;
		}

		if ((!pointFlag) && fileFullPath[i] == TEXT('.'))
		{
			fileFullPath[i] = TEXT('\0');
			pointFlag = TRUE;
		}
	}
	TCHAR sPath[MAX_PATH];
	lstrcpy(sPath, fileFullPath+start);
	lstrcpy(fileFullPath, sPath);
	return TRUE;
}

BOOL GetFileOnlyMainName2(LPTSTR fileOnlyName)
{
	int i;
	int len = lstrlen(fileOnlyName);
	for (i=len-1; i >= 0; i--)
	{
		if (fileOnlyName[i] == TEXT('.'))
		{
			fileOnlyName[i] = TEXT('\0');
			return TRUE;
		}
	}
	return FALSE;
}

//打印一张图片
BOOL PrintImage(LPCTSTR filename, LPCTSTR printer) 
{
	if (!FileExist(filename))
	{
		SetLastError(2);	//系统找不到指定的文件
		return FALSE;
	}
	CPrintDialog printDlg(FALSE);

	// Or get from user:
	//INT_PTR result = printDlg.DoModal();
	//if (result == IDCANCEL)
	//	return;

	TCHAR pOldPrinter[MAX_PATH];
	pOldPrinter[0] = TEXT('\0');
	if (printer != NULL)
	{
		DWORD nSize(MAX_PATH);
		if(!GetDefaultPrinter(pOldPrinter,&nSize))
		{
			pOldPrinter[0] = TEXT('\0');
		}
		if(!SetDefaultPrinter(printer))
		{
			ShowLastError();
			return FALSE;
		}
	}
	printDlg.GetDefaults();
	if (printer != NULL && pOldPrinter[0] != TEXT('\0'))
	{
		SetDefaultPrinter(pOldPrinter);
	}

	CDC dc;
	if (!dc.Attach(printDlg.GetPrinterDC())) 
	{
		//::MessageBox(NULL, TEXT("No printer found!"), TEXT("Error"), MB_OK|MB_ICONINFORMATION);
		SetLastError(2150);	//打印机不存在
		return FALSE;
	} 

	dc.m_bPrinting = TRUE; 
	DOCINFO di;    
	// Initialise print document details
	::ZeroMemory (&di, sizeof (DOCINFO));
	di.cbSize = sizeof (DOCINFO);
	di.lpszDocName = filename; 
	BOOL bPrintingOK = dc.StartDoc(&di); // Begin a new print job 
	// Get the printing extents
	// and store in the m_rectDraw field of a 
	// CPrintInfo object
	CPrintInfo Info;
	Info.SetMaxPage(1); // just one page 
	float maxw = static_cast<float>(dc.GetDeviceCaps(HORZSIZE) / 0.254);
	float maxh = static_cast<float>(dc.GetDeviceCaps(VERTSIZE) / 0.254);
	Info.m_rectDraw.SetRect(0, 0, static_cast<int>(maxw), static_cast<int>(maxh));
	RectF rectf(0, 0, maxw, maxh);

	for (UINT page = Info.GetMinPage(); page <= 
		Info.GetMaxPage() && bPrintingOK; page++) 
	{
		dc.StartPage();    // begin new page
		Info.m_nCurPage = page;

		{
			Graphics graphics(dc.GetSafeHdc());
			Image img(filename);
			graphics.DrawImage(&img, rectf, 0, 0, (REAL)img.GetWidth(), (REAL)img.GetHeight(), UnitPixel);
		}

		bPrintingOK = (dc.EndPage() > 0);   // end page
	} 

	if (bPrintingOK)
	{
		dc.EndDoc(); // end a print job
		return TRUE;
	}
	else 
	{
		dc.AbortDoc();           // abort job. 
		return FALSE;
	}
}

//查找并打印文件夹下指定图片，可以包含通配符 *, ?
BOOL PrintFolderImages(LPCTSTR sImgPath, LPCTSTR printer, UINT * pdwCount) 
{
	CFileFind ff;
	BOOL res=ff.FindFile(sImgPath);
	UINT count(0);
	while (res)
	{
		res=ff.FindNextFile();
		count++;
	}

	if (0 == count)
	{
		if(NULL != pdwCount)
			*pdwCount = count;
		return TRUE;
	}

	TCHAR sParentFolder[MAX_PATH];
	lstrcpy(sParentFolder,sImgPath);
	GetParentFolder(sParentFolder);
	FormatFolderName2(sParentFolder);
	GetFileOnlyName(sParentFolder);

	CPrintDialog printDlg(FALSE);

	// Or get from user:
	//INT_PTR result = printDlg.DoModal();
	//if (result == IDCANCEL)
	//	return;

	TCHAR pOldPrinter[MAX_PATH];
	pOldPrinter[0] = TEXT('\0');
	if (printer != NULL)
	{
		DWORD nSize(MAX_PATH);
		if(!GetDefaultPrinter(pOldPrinter,&nSize))
		{
			pOldPrinter[0] = TEXT('\0');
		}
		if(!SetDefaultPrinter(printer))
		{
			ShowLastError();
			return FALSE;
		}
	}
	printDlg.GetDefaults();
	if (printer != NULL && pOldPrinter[0] != TEXT('\0'))
	{
		SetDefaultPrinter(pOldPrinter);
	}

	CDC dc;
	if (!dc.Attach(printDlg.GetPrinterDC())) 
	{
		//::MessageBox(NULL, TEXT("No printer found!"), TEXT("Error"), MB_OK|MB_ICONINFORMATION);
		SetLastError(2150);	//打印机不存在
		return FALSE;
	} 

	dc.m_bPrinting = TRUE; 
	DOCINFO di;    
	// Initialise print document details
	::ZeroMemory (&di, sizeof (DOCINFO));
	di.cbSize = sizeof (DOCINFO);
	di.lpszDocName = sParentFolder; 
#ifdef _DEBUG
	//::MessageBox(NULL, di.lpszDocName, TEXT("测试1"), MB_OK|MB_ICONINFORMATION);
#endif
	BOOL bPrintingOK = dc.StartDoc(&di); // Begin a new print job 

#ifdef _DEBUG
	//::MessageBox(NULL, TEXT("StartDoc"), TEXT("测试2"), MB_OK|MB_ICONINFORMATION);
#endif
	// Get the printing extents
	// and store in the m_rectDraw field of a 
	// CPrintInfo object
	CPrintInfo Info;
	Info.SetMaxPage(count); // just one page 
	float maxw = static_cast<float>(dc.GetDeviceCaps(HORZSIZE) / 0.254);
	float maxh = static_cast<float>(dc.GetDeviceCaps(VERTSIZE) / 0.254);
	Info.m_rectDraw.SetRect(0, 0, static_cast<int>(maxw), static_cast<int>(maxh));
	RectF rectf(0, 0, maxw, maxh);

#ifdef _DEBUG
	CString wh;
	//wh.Format(TEXT("w=%d,h=%d"),maxw,maxh);
	//::MessageBox(NULL, wh, TEXT("页面宽度和高度"), MB_OK|MB_ICONINFORMATION);
	wh.Format(TEXT("w=%d,h=%d"),dc.GetDeviceCaps(HORZSIZE),dc.GetDeviceCaps(VERTSIZE));
	//::MessageBox(NULL, wh, TEXT("页面宽度和高度2"), MB_OK|MB_ICONINFORMATION);
#endif

	res=ff.FindFile(sImgPath);
	count = 0;
	for (UINT page = Info.GetMinPage(); page <= 
		Info.GetMaxPage() && bPrintingOK && res; page++) 
	{
		res=ff.FindNextFile();
		dc.StartPage();    // begin new page
#ifdef _DEBUG
		//::MessageBox(NULL, TEXT("StartPage"), TEXT("测试3"), MB_OK|MB_ICONINFORMATION);
#endif
		Info.m_nCurPage = page;

		{
			Graphics graphics(dc.GetSafeHdc());
			Image img(ff.GetFilePath());
			graphics.DrawImage(&img, rectf, 0, 0, (REAL)img.GetWidth(), (REAL)img.GetHeight(), UnitPixel);
		}
		count++;

		bPrintingOK = (dc.EndPage() > 0);   // end page
	} 

	if(NULL != pdwCount)
		*pdwCount = count;

	if (bPrintingOK)
	{
		dc.EndDoc(); // end a print job
		return TRUE;
	}
	else 
	{
		dc.AbortDoc();           // abort job. 
		return FALSE;
	}
}