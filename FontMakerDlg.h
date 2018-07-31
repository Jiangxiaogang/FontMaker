// FontMakerDlg.h : 头文件
#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "BitFont.h"
#include "Charset.h"
#include "EditBox.h"
#include "PreviewWnd.h"

// CFontMakerDlg 对话框
class CFontMakerDlg : public CDialog
{
// 构造
public:
	CFontMakerDlg(CWnd* pParent = NULL);	// 标准构造函数
	enum { IDD = IDD_MAIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

private:
	CPreviewWnd m_draw;
	CEditBox m_ebox;
	CBitFont m_bitfont;
	CCharset m_charset;
	HFONT m_hFont;
	WCHAR m_wChar;
	UINT  m_nCharIndex;
	BOOL m_bInitOK;
	int m_nFontSize;
	int m_nFontWidth;
	int m_nFontHeight;
	int m_nOffsetX;
	int m_nOffsetY;
	CString m_szCharsetPath;
	CComboBox m_listFontName;
	CComboBox m_listFontStyle;
	CComboBox m_listCharset;
	CButton m_btnEdit;
	CSpinButtonCtrl m_spFontSize;
	CSpinButtonCtrl m_spFontWidth;
	CSpinButtonCtrl m_spFontHeight;
	CSpinButtonCtrl m_spOffsetX;
	CSpinButtonCtrl m_spOffsetY;
	HICON m_hIcon;
public:
	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	virtual void PostNcDestroy();
	void RedrawPreview();
	void OnFontChange();
	void OnCharChange();
	void OnCharsetChange();
	void OnCharTableChange();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnBnClickedBtnEdit();
	afx_msg void OnBnClickedBtnSave();
    afx_msg void OnBnClickedBtnStd();
	afx_msg void OnBnClickedBtnUser();
	afx_msg void OnCbnSelchangeListFontName();
	afx_msg void OnCbnSelchangeListFontStyle();
    afx_msg void OnCbnSelchangeListCharset();
	afx_msg void OnEnChangeEditFontSize();
	afx_msg void OnEnChangeEditWidth();
	afx_msg void OnEnChangeEditHeight();
	afx_msg void OnEnChangeEditHorz();
	afx_msg void OnEnChangeEditVert();
    afx_msg void OnDeltaposSpin6(NMHDR *pNMHDR, LRESULT *pResult);
};
