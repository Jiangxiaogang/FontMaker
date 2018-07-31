#pragma once

class CBitFont
{
public:
	CBitFont(void);
	~CBitFont(void);
	void SetFont(HFONT hFont);
	void SetOffset(INT dx, INT dy);
	void SetSize(INT width, INT height);
	void PaintChar(WCHAR ch);
    INT  GetBits(BYTE* pBits, INT size, INT scan, BOOL msb, BOOL var);
	HDC  GetDC();
private:
	void CreateBitmap();
	BYTE GetPixel(int x, int y);
    INT  GetBitsHorz(BYTE* pBits, INT size, BOOL msb, BOOL vw);
    INT  GetBitsVert(BYTE* pBits, INT size, BOOL msb, BOOL vw);
private:
	HDC m_hdc;
	HBITMAP m_hBitmap;
	BYTE* m_pPixels;
	INT m_nBytesPerLine;
	INT m_nOffsetX;
	INT m_nOffsetY;
	INT m_nWidth;
	INT m_nHeight;
    INT m_nCharWidth;
};
