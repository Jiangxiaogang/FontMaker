// AboutBox.cpp : 实现文件
//

#include "stdafx.h"
#include "FontMakerApp.h"
#include "AboutBox.h"


// CAboutBox 对话框
IMPLEMENT_DYNAMIC(CAboutBox, CDialog)

CAboutBox::CAboutBox(CWnd* pParent /*=NULL*/)
	: CDialog(CAboutBox::IDD, pParent)
{

}

CAboutBox::~CAboutBox()
{
}

void CAboutBox::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CAboutBox, CDialog)
END_MESSAGE_MAP()


// CAboutBox 消息处理程序
static WCHAR szLicense[]=
{
	L"本软件是免费软件，你可以随意使用和传播本软件.\r\n",
};

static WCHAR szVersion[]=
{
	L"软件名称: 点阵字库生成工具\r\n"
	L"软件版本: 1.2.0\r\n"
	L"编译日期: 2018.07.31\r\n"
	L"软件作者: 星沉地动\r\n"
	L"联系方式: 446252221@qq.com\r\n"
};

BOOL CAboutBox::OnInitDialog()
{
	CDialog::OnInitDialog();
	SetDlgItemText(IDC_EDIT1,szVersion);
	SetDlgItemText(IDC_EDIT2,szLicense);
	return TRUE;
}
