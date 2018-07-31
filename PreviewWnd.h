#pragma once


// CDraw
class CPreviewWnd : public CWnd
{
	DECLARE_DYNAMIC(CPreviewWnd)
public:
	void SetDC(HDC hdc);
	void SetSize(INT width, INT height);
private:
	INT m_nWidth;
	INT m_nHeight;
	INT m_nPaintX;
	INT m_nPaintY;
	RECT m_client;
	CDC m_dc;
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	virtual void PreSubclassWindow();
	afx_msg void OnDestroy();
};
