// AboutBox.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "FontMakerApp.h"
#include "AboutBox.h"


// CAboutBox �Ի���
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


// CAboutBox ��Ϣ�������
static WCHAR szLicense[]=
{
	L"������������������������ʹ�úʹ��������.\r\n",
};

static WCHAR szVersion[]=
{
	L"�������: �����ֿ����ɹ���\r\n"
	L"����汾: 1.2.1\r\n"
	L"��������: 2019.08.17\r\n"
	L"�������: ��ЭȪ\r\n"
	L"��ϵ��ʽ: lxq_69697769@163.com\r\n"
};

BOOL CAboutBox::OnInitDialog()
{
	CDialog::OnInitDialog();
	SetDlgItemText(IDC_EDIT1,szVersion);
	SetDlgItemText(IDC_EDIT2,szLicense);
	return TRUE;
}
