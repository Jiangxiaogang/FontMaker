#pragma once

class CBitFont
{
public:
	CBitFont(void);
	~CBitFont(void);
	void SetFont(HFONT hFont);
	void SetOffset(INT dx, INT dy);
	void SetSize(INT width, INT height);
	void PaintFont(WCHAR ch);
	INT  GetBits(INT mode, BYTE* pBits, INT size);
	HDC  GetDC();
private:
	void UpdateBitmap();
	BYTE GetPixel(int x, int y);
private:
	HDC m_hdc;
	HBITMAP m_hBitmap;
	BYTE* m_pPixels;
	INT m_nBytesPerLine;
	INT m_nOffsetX;
	INT m_nOffsetY;
	INT m_nWidth;
	INT m_nHeight;
};
