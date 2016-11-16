// Draw.cpp : 实现文件
#include "stdafx.h"
#include "FontMaker.h"
#include "Draw.h"

// CDraw
IMPLEMENT_DYNAMIC(CDraw, CWnd)

BEGIN_MESSAGE_MAP(CDraw, CWnd)
	ON_WM_PAINT()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

void CDraw::SetDC(HDC hdc)
{
	m_dc.Attach(hdc);
}

void CDraw::SetSize(INT width, INT height)
{
	RECT rc;
	GetClientRect(&rc);
	m_nWidth  = width;
	m_nHeight = height;
	m_nPaintX = (rc.right-m_nWidth)/2;
	m_nPaintY = (rc.bottom-m_nHeight)/2;
}

// CDraw 消息处理程序
void CDraw::OnPaint()
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

void CDraw::PreSubclassWindow()
{
	m_nWidth  = 0;
	m_nHeight = 0;
	m_nPaintX = 0;
	m_nPaintY = 0;
	GetClientRect(&m_client);
	CWnd::PreSubclassWindow();
}

void CDraw::OnDestroy()
{
	CWnd::OnDestroy();
	m_dc.Detach();
}
