#include "stdafx.h"
#include "Log.h"

void LOG(const char *) {

}

CString FormatString(const char *lpcszFormat, ...)
{
	char *pszStr = NULL;
	if (NULL != lpcszFormat)
	{
		va_list marker = NULL;
		va_start(marker, lpcszFormat); //��ʼ����������
		size_t nLength = _vscprintf(lpcszFormat, marker) + 1; //��ȡ��ʽ���ַ�������
		pszStr = new char[nLength];
		memset(pszStr, '\0', nLength);
		_vsnprintf_s(pszStr, nLength, nLength, lpcszFormat, marker);
		va_end(marker); //���ñ�������
	}
	CString strResult(pszStr);
	delete[]pszStr;
	return strResult;
}