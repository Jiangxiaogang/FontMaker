#include "StdAfx.h"
#include "BitFont.h"

CBitFont::CBitFont(void)
{
	HDC hdc;
	m_nOffsetX = 0;
	m_nOffsetY = 0;
	m_nWidth = 1;
	m_nHeight= 1;
	m_hBitmap = NULL;
	hdc = ::GetDC(NULL);
	m_hdc = ::CreateCompatibleDC(hdc);
	::ReleaseDC(NULL,hdc);
	::SetTextColor(m_hdc,RGB(255,255,255));
	::SetBkColor(m_hdc,RGB(0,0,0));
}

CBitFont::~CBitFont(void)
{
	if(m_hdc)
	{
		::DeleteDC(m_hdc);
	}
	if(m_hBitmap)
	{
		::DeleteObject(m_hBitmap);
	}
}

void CBitFont::UpdateBitmap(void)
{
	HBITMAP hBitmap;
	BITMAPINFO bmi;
	ZeroMemory(&bmi,sizeof(bmi));
	bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biWidth = m_nWidth;
	bmi.bmiHeader.biHeight = m_nHeight;
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biBitCount = 8;
	bmi.bmiHeader.biCompression = BI_RGB;
	hBitmap = ::CreateDIBSection(m_hdc,&bmi,DIB_RGB_COLORS,(void**)&m_pPixels,NULL,0);
	if(hBitmap != NULL)
	{
		if(m_hBitmap != NULL)
		{
			::DeleteObject(m_hBitmap);
		}
		::SelectObject(m_hdc,hBitmap);
		m_hBitmap = hBitmap;
	}
}

void CBitFont::SetFont(HFONT hFont)
{
	::SelectObject(m_hdc,hFont);
}

void CBitFont::SetOffset(INT dx, INT dy)
{
	m_nOffsetX = dx;
	m_nOffsetY = dy;
}

void CBitFont::SetSize(INT width, INT height)
{
	m_nWidth = width;
	m_nHeight= height;
	m_nBytesPerLine = 4*((width+3)/4);
	UpdateBitmap();
}

void CBitFont::PaintFont(WCHAR ch)
{
	RECT rc;
	SetRect(&rc,0,0,m_nWidth,m_nHeight);
	::FillRect(m_hdc,&rc, (HBRUSH)GetStockObject(BLACK_BRUSH));
	::TextOut(m_hdc,m_nOffsetX,m_nOffsetY,&ch,1);
}

HDC CBitFont::GetDC()
{
	return m_hdc;
}

//点阵数据在内存中的格式:
//1.4字节对齐
//2.垂直巅倒
BYTE CBitFont::GetPixel(int x, int y)
{
	int line;
	BYTE* pLine;
	line = m_nHeight - 1 - y;
	pLine = m_pPixels + m_nBytesPerLine * line;
	return pLine[x];
}

//水平扫描,8bit,LSB
INT  CBitFont::GetBitsHorzLSB(BYTE* pBits, INT size)
{
	INT x;
	INT y;
	INT index;
	INT ret;
	const BYTE mask[8] = {0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};
	ret = m_nHeight * ((m_nWidth+7)/8);
	if(size < ret)
	{
		return ret;
	}
	for(y=0;y<m_nHeight;y++)
	{
		index = 0;
		for(x=0;x<m_nWidth;x++)
		{
			if(index==0)
			{
				*pBits = 0;
			}
			if(GetPixel(x,y) != 0)
			{
				*pBits |= mask[index];
			}
			index++;
			if(index==8)
			{
				index = 0;
				pBits++;
			}
		}
		if(index != 0)
		{
			pBits++;
		}
	}
	return ret;
}

//水平扫描,8bit,MSB
INT  CBitFont::GetBitsHorzMSB(BYTE* pBits, INT size)
{
	INT x;
	INT y;
	INT index;
	INT ret;
	const BYTE mask[8] = {0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01};
	ret = m_nHeight * ((m_nWidth+7)/8);
	if(size < ret)
	{
		return ret;
	}
	for(y=0;y<m_nHeight;y++)
	{
		index = 0;
		for(x=0;x<m_nWidth;x++)
		{
			if(index==0)
			{
				*pBits = 0;
			}
			if(GetPixel(x,y) != 0)
			{
				*pBits |= mask[index];
			}
			index++;
			if(index==8)
			{
				index = 0;
				pBits++;
			}
		}
		if(index != 0)
		{
			pBits++;
		}
	}
	return ret;
}

//垂直扫描,8bit,LSB
INT  CBitFont::GetBitsVertLSB(BYTE* pBits, INT size)
{
	INT x;
	INT y;
	INT index;
	INT ret;
	const BYTE mask[8] = {0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};
	ret = m_nWidth * ((m_nHeight+7)/8);
	if(size < ret)
	{
		return ret;
	}
	for(x=0;x<m_nWidth;x++)
	{
		index = 0;
		for(y=0;y<m_nHeight;y++)
		{
			if(index==0)
			{
				*pBits = 0;
			}
			if(GetPixel(x,y) != 0)
			{
				*pBits |= mask[index];
			}
			index++;
			if(index == 8)
			{
				index = 0;
				pBits++;
			}
		}
		if(index != 0)
		{
			pBits++;
		}
	}
	return ret;
}

//垂直扫描,8bit,MSB
INT  CBitFont::GetBitsVertMSB(BYTE* pBits, INT size)
{
	INT x;
	INT y;
	INT index;
	INT ret;
	const BYTE mask[8] = {0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01};
	ret = m_nWidth * ((m_nHeight+7)/8);
	if(size < ret)
	{
		return ret;
	}
	for(x=0;x<m_nWidth;x++)
	{
		index = 0;
		for(y=0;y<m_nHeight;y++)
		{
			if(index==0)
			{
				*pBits = 0;
			}
			if(GetPixel(x,y) != 0)
			{
				*pBits |= mask[index];
			}
			index++;
			if(index == 8)
			{
				index = 0;
				pBits++;
			}
		}
		if(index != 0)
		{
			pBits++;
		}
	}
	return ret;
}

//从BITMAP中读取点阵数据
//mode=0 水平扫描,MSB
//mode=1 垂直扫描,MSB
//mode=2 水平扫描,LSB
//mode=3 垂直扫描,LSB
INT CBitFont::GetBits(INT mode, BYTE* pBits, INT size)
{
	switch(mode)
	{
	case 0:
		return GetBitsHorzMSB(pBits,size);
	case 1:	
		return GetBitsVertMSB(pBits, size);
	case 2:
		return GetBitsHorzLSB(pBits, size);
	case 3:
		return GetBitsVertLSB(pBits, size);
	}
	return 0;
}
