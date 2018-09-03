#pragma once

enum DOWNLOADSTATE {DOWNLOAD_SUCCESS, DOWNLOAD_EMPTY, DOWNLOAD_ERROR};
typedef LRESULT (CALLBACK *HTTPDLPRPC)(int nCurrentSize, float fPercent, LPVOID lpParm);

DWORD StartDownload(LPCTSTR szURL, LPCTSTR szFilePath, DWORD dwTimeOut, HTTPDLPRPC pFun,LPVOID lpParm);