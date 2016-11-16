#include "StdAfx.h"
#include "Charset.h"

CCharset::CCharset(void)
{
	m_pTable = NULL;
	m_nCount = 0;
}

CCharset::~CCharset(void)
{
}

//从文件加载
BOOL CCharset::LoadFromFile(LPCTSTR pFileName)
{
	CFile cf;
	ULONG len;
	if(!cf.Open(pFileName,CFile::modeRead|CFile::shareDenyNone))
	{
		return FALSE;
	}
	len = (ULONG)cf.GetLength();
	m_pTable = (WCHAR*)malloc(len+2);
	if(m_pTable==NULL)
	{
		cf.Close();
		return FALSE;
	}
	ZeroMemory(m_pTable,len+2);
	if(cf.Read(m_pTable,len)!=len)
	{
		cf.Close();
		free(m_pTable);
		m_pTable = NULL;
		return FALSE;
	}
	cf.Close();
	m_nCount = len/2;
	return TRUE;
}

BOOL CCharset::Create(LPCTSTR pString)
{
	m_nCount = (UINT)wcslen(pString);
	m_pTable = _wcsdup(pString);
	return FALSE;
}

void CCharset::Delete()
{
	free(m_pTable);
	m_pTable = NULL;
	m_nCount = 0;
}

WCHAR CCharset::GetChar(UINT index)
{
	if(m_pTable == NULL)
	{
		return L'?';
	}
	if(index < m_nCount)
	{
		return *(m_pTable+index);
	}
	return L'?';
}

UINT CCharset::GetCharCount()
{
	return m_nCount;
}
