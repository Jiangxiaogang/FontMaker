#include "StdAfx.h"
#include "BitFont.h"

CBitFont::CBitFont(void)
{
	HDC hdc;
	m_nOffsetX = 0;
	m_nOffsetY = 0;
	m_nWidth = 1;
	m_nHeight= 1;
    m_nCharWidth = 0;
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

void CBitFont::CreateBitmap(void)
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
	CreateBitmap();
}

void CBitFont::PaintChar(WCHAR ch)
{
	RECT rc;
    SIZE sz;
	SetRect(&rc,0,0,m_nWidth,m_nHeight);
	::FillRect(m_hdc,&rc,(HBRUSH)GetStockObject(BLACK_BRUSH));
	::TextOut(m_hdc,m_nOffsetX,m_nOffsetY,&ch,1);
    ::GetTextExtentPoint32W(m_hdc,&ch,1,&sz);
    m_nCharWidth = sz.cx;
}

HDC CBitFont::GetDC()
{
	return m_hdc;
}

BYTE CBitFont::GetPixel(int x, int y)
{
	int line;
	BYTE* pLine;
	line = m_nHeight - 1 - y;
	pLine = m_pPixels + m_nBytesPerLine * line;
	return pLine[x];
}

INT  CBitFont::GetBitsHorz(BYTE* pBits, INT size, BOOL msb, BOOL vw)
{
	INT x;
	INT y;
	INT index;
	INT ret;
	const BYTE mask_lsb[8] = {0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};
    const BYTE mask_msb[8] = {0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01};
    const BYTE *mask = msb ? mask_msb : mask_lsb;
	ret = m_nHeight * ((m_nWidth+7)/8);
    ret += vw ? 2 : 0;
	if(size < ret)
	{
		return ret;
	}
    if(vw)
    {
        pBits[0] = m_nCharWidth>>8;
        pBits[1] = m_nCharWidth;
        pBits += 2;
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


INT  CBitFont::GetBitsVert(BYTE* pBits, INT size, BOOL msb, BOOL vw)
{
	INT x;
	INT y;
	INT index;
	INT ret;
	const BYTE mask_lsb[8] = {0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};
    const BYTE mask_msb[8] = {0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01};
    const BYTE *mask = msb ? mask_msb : mask_lsb;
	ret = m_nWidth * ((m_nHeight+7)/8);
    ret += vw ? 2 : 0;
	if(size < ret)
	{
		return ret;
	}
    if(vw)
    {
        pBits[0] = m_nCharWidth>>8;
        pBits[1] = m_nCharWidth;
        pBits += 2;
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

INT CBitFont::GetBits(BYTE* pBits, INT size, INT scan, BOOL msb, BOOL var)
{
    switch(scan)
    {
    case 0:
        return GetBitsHorz(pBits, size, msb, var);
    case 1:
        return GetBitsVert(pBits, size, msb, var);
    default:
        return 0;
    }
}
