#pragma once
#include "afxwin.h"


// CEditBox 对话框
class CEditBox : public CDialog
{
	DECLARE_DYNAMIC(CEditBox)

public:
	CEditBox(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CEditBox();

// 对话框数据
	enum { IDD = IDD_EDIT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	DECLARE_MESSAGE_MAP()
public:
	WCHAR* m_pzTable;
	CEdit m_editTable;
	afx_msg void OnBnClickedBtnLoad();
	afx_msg void OnBnClickedBtnSave();
	afx_msg void OnBnClickedBtnNum();
	afx_msg void OnBnClickedBtnLetter();
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
};
