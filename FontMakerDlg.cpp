// FontMakerDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "FontMakerApp.h"
#include "FontMakerDlg.h"
#include "FileMaker.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CFontMakerDlg �Ի���
CFontMakerDlg::CFontMakerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFontMakerDlg::IDD, pParent)
{
	m_bInitOK = 0;
	m_nCharIndex = 0;
	m_wChar = L'0';
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFontMakerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_FONT_NAME, m_listFontName);
	DDX_Control(pDX, IDC_LIST_FONT_STYLE, m_listFontStyle);
	DDX_Control(pDX, IDC_LIST_CHARSET, m_listCharset);
	DDX_Control(pDX, IDC_BTN_EDIT, m_btnEdit);
	DDX_Control(pDX, IDC_SPIN1, m_spFontWidth);
	DDX_Control(pDX, IDC_SPIN3, m_spFontHeight);
	DDX_Control(pDX, IDC_SPIN2, m_spOffsetX);
	DDX_Control(pDX, IDC_SPIN4, m_spOffsetY);
	DDX_Control(pDX, IDC_SPIN5, m_spFontSize);
}

BEGIN_MESSAGE_MAP(CFontMakerDlg, CDialog)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_EDIT, &CFontMakerDlg::OnBnClickedBtnEdit)
	ON_BN_CLICKED(IDC_BTN_SAVE, &CFontMakerDlg::OnBnClickedBtnSave)
    ON_BN_CLICKED(IDC_BTN_STD, &CFontMakerDlg::OnBnClickedBtnStd)
	ON_BN_CLICKED(IDC_BTN_USER, &CFontMakerDlg::OnBnClickedBtnUser)
	ON_CBN_SELCHANGE(IDC_LIST_FONT_NAME, &CFontMakerDlg::OnCbnSelchangeListFontName)
	ON_CBN_SELCHANGE(IDC_LIST_FONT_STYLE, &CFontMakerDlg::OnCbnSelchangeListFontStyle)
	ON_CBN_SELCHANGE(IDC_LIST_CHARSET, &CFontMakerDlg::OnCbnSelchangeListCharset)
    ON_EN_CHANGE(IDC_EDIT_FONT_SIZE, &CFontMakerDlg::OnEnChangeEditFontSize)
	ON_EN_CHANGE(IDC_EDIT_WIDTH, &CFontMakerDlg::OnEnChangeEditWidth)
	ON_EN_CHANGE(IDC_EDIT_HEIGHT, &CFontMakerDlg::OnEnChangeEditHeight)
	ON_EN_CHANGE(IDC_EDIT_HORZ, &CFontMakerDlg::OnEnChangeEditHorz)
	ON_EN_CHANGE(IDC_EDIT_VERT, &CFontMakerDlg::OnEnChangeEditVert)
    ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN6, &CFontMakerDlg::OnDeltaposSpin6)
END_MESSAGE_MAP()

//ö��ϵͳ����
static int CALLBACK FontEnumProc(CONST ENUMLOGFONT *lpelfe,CONST TEXTMETRIC *lpntme,DWORD FontType,LPARAM lParam)
{
	CComboBox* pComboBox;
	pComboBox = (CComboBox*)lParam;
	if(lpelfe->elfFullName[0]=='@')
	{
		return 1;
	}
	if(pComboBox->FindString(0,lpelfe->elfFullName)==CB_ERR)
	{
		pComboBox->AddString(lpelfe->elfFullName);
	}
	return 1;
}

static void InitFontFamily(CComboBox* pComboBox)
{
	HDC hdc;
	LOGFONT lf;
	ZeroMemory(&lf,sizeof(lf));
	lf.lfCharSet = DEFAULT_CHARSET;
	hdc = GetDC(NULL);
	::EnumFontFamiliesEx(hdc,&lf,(FONTENUMPROCW)::FontEnumProc,(LPARAM)pComboBox,0);
	ReleaseDC(NULL,hdc);
	pComboBox->SetCurSel(0);
}

//�����ļ�����,������׺��
static int GetFileTitle(LPTSTR lpFile, LPTSTR lpName, int count)
{
	int ret;
	TCHAR* dot;
	ZeroMemory(lpName,count*sizeof(TCHAR));
	dot = wcsrchr(lpFile,_T('.'));
	if(dot != NULL)
	{
		ret = (int)(dot-lpFile);
		wcsncpy_s(lpName,count,lpFile,ret);
	}
	else
	{
		ret = count;
		wcsncpy_s(lpName,count,lpFile,ret);
	}
	return ret;
}

//�����ļ���,����*.cst
static UINT InitCharset(CComboBox* pComboBox)
{
	UINT count;
	HANDLE hFind;
	CString szPath;
	WCHAR title[MAX_PATH];
	WIN32_FIND_DATA wfd;
	count = 0;
	theApp.GetPath(szPath);
	szPath.Append(L"charset\\*.cst");
	hFind = FindFirstFile(szPath,&wfd);
	if(hFind!=INVALID_HANDLE_VALUE)
	{
		count++;
		GetFileTitle(wfd.cFileName,title,MAX_PATH);
		pComboBox->AddString(title);
		while(FindNextFile(hFind,&wfd))
		{
			count++;
			GetFileTitle(wfd.cFileName,title,MAX_PATH);
			pComboBox->AddString(title);
		}
		FindClose(hFind);
		pComboBox->SetCurSel(0);
	}
	return count;
}

void CFontMakerDlg::OnFontChange()
{
	int style;
	LOGFONT lf;
	ZeroMemory(&lf,sizeof(lf));
    m_listFontName.GetWindowText(lf.lfFaceName,32);
	lf.lfCharSet = DEFAULT_CHARSET;
    //lf.lfQuality = ANTIALIASED_QUALITY;
	lf.lfHeight = m_nFontSize;
	style=m_listFontStyle.GetCurSel();
	switch(style)
	{
	case 0:
		lf.lfWeight = 400;
		break;
	case 1:
		lf.lfWeight = 700;
		break;
	case 2:
		lf.lfWeight = 400;
		lf.lfItalic = 1;
		break;
	}
	if(m_hFont != NULL)
	{
		DeleteObject(m_hFont);
	}
	m_hFont = CreateFontIndirect(&lf);
	m_bitfont.SetFont(m_hFont);
	RedrawPreview();
}

void CFontMakerDlg::OnCharChange()
{
	CString str;
	m_wChar = m_charset.GetChar(m_nCharIndex);
    str.Format(L"��ǰ�ַ�: U+%04X(%d/%d)",m_wChar,m_nCharIndex+1,m_charset.GetCharCount());
	SetDlgItemText(IDC_EDIT_INDEX,str);
	RedrawPreview();
}

//��׼�ַ������
void CFontMakerDlg::OnCharsetChange()
{
	UINT count;
	CString name;
	m_listCharset.GetWindowText(name);
	name.Append(L".cst");
	name = m_szCharsetPath + name;
	m_charset.Delete();
	if(!m_charset.LoadFromFile(name))
	{
		m_nCharIndex = 0;
		MessageBox(L"�����ַ����ļ�ʧ��!",L"������ʾ",MB_OK|MB_ICONWARNING);
		return;
	}
	count = m_charset.GetCharCount();
	m_nCharIndex = 0;
	OnCharChange();
}

//�û������
void CFontMakerDlg::OnCharTableChange()
{
	UINT count;
	if(m_ebox.m_pzTable != NULL)
	{
		m_charset.Delete();
		m_charset.Create(m_ebox.m_pzTable);
		count = m_charset.GetCharCount();
		m_nCharIndex = 0;
		OnCharChange();
	}
}

// CFontMakerDlg ��Ϣ�������
BOOL CFontMakerDlg::OnInitDialog()
{
	int count;
	CDialog::OnInitDialog();
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��
	SetWindowText(L"ͨ�õ����ֿ����ɹ���");
	InitFontFamily(&m_listFontName);
	
	m_draw.SubclassDlgItem(IDC_BITMAP,this);
	m_draw.SetDC(m_bitfont.GetDC());
	
	theApp.GetPath(m_szCharsetPath);
	m_szCharsetPath.Append(L"charset\\");
	count=InitCharset(&m_listCharset);
	if(count==0)
	{
		GetDlgItem(IDC_BTN_STD)->EnableWindow(FALSE);
		CheckDlgButton(IDC_BTN_USER,1);
		OnBnClickedBtnUser();
		OnCharTableChange();
	}
	else
	{
		CheckDlgButton(IDC_BTN_STD,1);
		OnBnClickedBtnStd();
		OnCharsetChange();
	}
	m_bInitOK = TRUE;
	m_listFontStyle.SetCurSel(0);
	m_spFontSize.SetRange(1,+999);
	m_spFontWidth.SetRange(1,999);
	m_spFontHeight.SetRange(1,999);
	m_spOffsetX.SetRange(-999,+999);
	m_spOffsetY.SetRange(-999,+999);

	CheckDlgButton(IDC_BTN_SCAN1,1);
	CheckDlgButton(IDC_BTN_MSB,1);
    CheckDlgButton(IDC_BTN_FW,1);
    CheckDlgButton(IDC_BTN_BIN,1);
	SetDlgItemInt(IDC_EDIT_WIDTH,16);
	SetDlgItemInt(IDC_EDIT_HEIGHT,16);
	SetDlgItemInt(IDC_EDIT_HORZ,0);
	SetDlgItemInt(IDC_EDIT_VERT,0);
	SetDlgItemInt(IDC_EDIT_FONT_SIZE,16);
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

//����Ԥ��ͼ
void CFontMakerDlg::RedrawPreview()
{
	m_bitfont.PaintChar(m_wChar);
	m_draw.Invalidate(FALSE);
}

//�༭���
void CFontMakerDlg::OnBnClickedBtnEdit()
{
	m_ebox.DoModal();
	OnCharTableChange();
}

//������
void CFontMakerDlg::OnCbnSelchangeListFontName()
{
	OnFontChange();
}

//���α��
void CFontMakerDlg::OnCbnSelchangeListFontStyle()
{
	OnFontChange();
}

//�ֺű��
void CFontMakerDlg::OnEnChangeEditFontSize()
{
	if(m_bInitOK)
	{
        m_nFontSize = GetDlgItemInt(IDC_EDIT_FONT_SIZE);
		OnFontChange();
	}
}

//�ַ������
void CFontMakerDlg::OnCbnSelchangeListCharset()
{
	OnCharsetChange();
}

//ѡ�б�׼�ֿ�
void CFontMakerDlg::OnBnClickedBtnStd()
{
	m_listCharset.EnableWindow(1);
	m_btnEdit.EnableWindow(0);
	OnCharsetChange();
}

//ѡ���û��ֿ�
void CFontMakerDlg::OnBnClickedBtnUser()
{
	m_listCharset.EnableWindow(0);
	m_btnEdit.EnableWindow(1);
	OnCharTableChange();
}

//�����ȱ��
void CFontMakerDlg::OnEnChangeEditWidth()
{
	if(m_bInitOK)
	{
		m_nFontWidth = GetDlgItemInt(IDC_EDIT_WIDTH);
		m_bitfont.SetSize(m_nFontWidth,m_nFontHeight);
		m_draw.SetSize(m_nFontWidth,m_nFontHeight);
		RedrawPreview();
	}
}

//����߶ȱ��
void CFontMakerDlg::OnEnChangeEditHeight()
{
	if(m_bInitOK)
	{
		m_nFontHeight = GetDlgItemInt(IDC_EDIT_HEIGHT);
		m_bitfont.SetSize(m_nFontWidth,m_nFontHeight);
		m_draw.SetSize(m_nFontWidth,m_nFontHeight);
		RedrawPreview();
	}
}

//ˮƽƫ�Ʊ��
void CFontMakerDlg::OnEnChangeEditHorz()
{
	if(m_bInitOK)
	{
		m_nOffsetX = GetDlgItemInt(IDC_EDIT_HORZ);
		m_bitfont.SetOffset(m_nOffsetX,m_nOffsetY);
		RedrawPreview();
	}
}

//��ֱƫ�Ʊ��
void CFontMakerDlg::OnEnChangeEditVert()
{
	if(m_bInitOK)
	{
		m_nOffsetY = GetDlgItemInt(IDC_EDIT_VERT);
		m_bitfont.SetOffset(m_nOffsetX,m_nOffsetY);
		RedrawPreview();
	}
}

//��ű��
void CFontMakerDlg::OnDeltaposSpin6(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
    *pResult = 0;
    if(pNMUpDown->iDelta > 0)
    {
        if(m_nCharIndex < (m_charset.GetCharCount() - 1))
        {
            m_nCharIndex++;
            OnCharChange();
        }
    }
    else
    {
        if(m_nCharIndex > 0)
        {
            m_nCharIndex--;
            OnCharChange();
        }
    }
}

//�����ļ�
void CFontMakerDlg::OnBnClickedBtnSave()
{
	INT scan;
	BOOL msb;
	BOOL var;
	CFile file;
	CString name;
	CFileMaker maker;
	BOOL isCFile;

	scan = IsDlgButtonChecked(IDC_BTN_SCAN2);
	msb  = IsDlgButtonChecked(IDC_BTN_MSB);
	var  = IsDlgButtonChecked(IDC_BTN_VW);
	isCFile = IsDlgButtonChecked(IDC_BTN_C);

	if(isCFile)
	{
		CFileDialog fbox(0, _T(".c"),NULL,OFN_OVERWRITEPROMPT|OFN_ENABLESIZING,L"�����ļ�(*.c)|*.c||");
		if(fbox.DoModal()==IDOK)
		{
			name=fbox.GetFileName();
			if(!file.Open(name,CFile::modeCreate|CFile::modeReadWrite|CFile::shareDenyNone))
			{
				MessageBox(L"����Ŀ���ļ�ʧ��!",L"����ʧ��",MB_OK|MB_ICONWARNING);
				return;
			}
			maker.MakeCppFile(&m_bitfont, &m_charset, &file, scan, msb, var);
		}
	}
	else
	{
		CFileDialog fbox(0,_T(".bin"),NULL,OFN_OVERWRITEPROMPT|OFN_ENABLESIZING,L"�����ļ�(*.bin)|*.bin||");
		if(fbox.DoModal()==IDOK)
		{
			name=fbox.GetFileName();
			if(!file.Open(name,CFile::modeCreate|CFile::modeReadWrite|CFile::shareDenyNone))
			{
				MessageBox(L"����Ŀ���ļ�ʧ��!",L"����ʧ��",MB_OK|MB_ICONWARNING);
				return;
			}
			maker.MakeBinFile(&m_bitfont, &m_charset, &file, scan, msb, var);
		}
	}
	file.Close();
	MessageBox(L"�����ļ��ɹ�",L"�������",MB_OK|MB_ICONINFORMATION);
}

void CFontMakerDlg::PostNcDestroy()
{
	m_charset.Delete();
	CDialog::PostNcDestroy();
}
