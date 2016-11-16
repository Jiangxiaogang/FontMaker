#pragma once

class CCharset
{
public:
	CCharset(void);
	~CCharset(void);
	BOOL  LoadFromFile(LPCTSTR pFileName);
	BOOL  Create(LPCTSTR pString);
	void  Delete();
	WCHAR GetChar(UINT index);
	UINT  GetCharCount();
private:
	WCHAR* m_pTable;
	UINT   m_nCount;
};
