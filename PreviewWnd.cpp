// PreviewWnd.cpp : 实现文件
#include "stdafx.h"
#include "FontMakerApp.h"
#include "PreviewWnd.h"

// CPreviewWnd
IMPLEMENT_DYNAMIC(CPreviewWnd, CWnd)

BEGIN_MESSAGE_MAP(CPreviewWnd, CWnd)
	ON_WM_PAINT()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

void CPreviewWnd::SetDC(HDC hdc)
{
	m_dc.Attach(hdc);
}

void CPreviewWnd::SetSize(INT width, INT height)
{
	RECT rc;
	GetClientRect(&rc);
	m_nWidth  = width;
	m_nHeight = height;
	m_nPaintX = (rc.right-m_nWidth)/2;
	m_nPaintY = (rc.bottom-m_nHeight)/2;
}

// CPreviewWnd 消息处理程序
void CPreviewWnd::OnPaint()
{
	CBrush br;
	CRgn rgn;
	CPaintDC dc(this);
	rgn.CreateRectRgnIndirect(&m_client);
	br.CreateSysColorBrush(COLOR_BTNFACE);
	dc.SelectClipRgn(&rgn);
	dc.FillRect(&m_client,&br);
	dc.BitBlt(m_nPaintX,m_nPaintY,m_nWidth,m_nHeight,&m_dc,0,0,SRCCOPY);
	br.DeleteObject();
	rgn.DeleteObject();
}

void CPreviewWnd::PreSubclassWindow()
{
	m_nWidth  = 0;
	m_nHeight = 0;
	m_nPaintX = 0;
	m_nPaintY = 0;
	GetClientRect(&m_client);
	CWnd::PreSubclassWindow();
}

void CPreviewWnd::OnDestroy()
{
	CWnd::OnDestroy();
	m_dc.Detach();
}
