// FontMakerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "FontMaker.h"
#include "FontMakerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CFontMakerDlg 对话框
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
	DDX_Control(pDX, IDC_LIST_FORMAT, m_listFormat);
	DDX_Control(pDX, IDC_BTN_EDIT, m_btnEdit);
	DDX_Control(pDX, IDC_BTN_SAVE, m_btnSave);
	DDX_Control(pDX, IDC_SPIN1, m_spFontWidth);
	DDX_Control(pDX, IDC_SPIN3, m_spFontHeight);
	DDX_Control(pDX, IDC_SPIN2, m_spOffsetX);
	DDX_Control(pDX, IDC_SPIN4, m_spOffsetY);
	DDX_Control(pDX, IDC_SPIN5, m_spFontSize);
	DDX_Control(pDX, IDC_SPIN6, m_spCurrIndex);
}

BEGIN_MESSAGE_MAP(CFontMakerDlg, CDialog)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_EDIT, &CFontMakerDlg::OnBnClickedBtnEdit)
	ON_BN_CLICKED(IDC_BTN_SAVE, &CFontMakerDlg::OnBnClickedBtnSave)
	ON_CBN_SELCHANGE(IDC_LIST_FONT_NAME, &CFontMakerDlg::OnCbnSelchangeListFontName)
	ON_CBN_SELCHANGE(IDC_LIST_FONT_STYLE, &CFontMakerDlg::OnCbnSelchangeListFontStyle)
	ON_EN_CHANGE(IDC_EDIT_FONT_SIZE, &CFontMakerDlg::OnEnChangeEditFontSize)
	ON_CBN_SELCHANGE(IDC_LIST_CHARSET, &CFontMakerDlg::OnCbnSelchangeListCharset)
	ON_BN_CLICKED(IDC_BTN_STD, &CFontMakerDlg::OnBnClickedBtnStd)
	ON_BN_CLICKED(IDC_BTN_USER, &CFontMakerDlg::OnBnClickedBtnUser)
	ON_EN_CHANGE(IDC_EDIT_WIDTH, &CFontMakerDlg::OnEnChangeEditWidth)
	ON_EN_CHANGE(IDC_EDIT_HEIGHT, &CFontMakerDlg::OnEnChangeEditHeight)
	ON_EN_CHANGE(IDC_EDIT_HORZ, &CFontMakerDlg::OnEnChangeEditHorz)
	ON_EN_CHANGE(IDC_EDIT_VERT, &CFontMakerDlg::OnEnChangeEditVert)
	ON_EN_CHANGE(IDC_EDIT_INDEX, &CFontMakerDlg::OnEnChangeEditIndex)
END_MESSAGE_MAP()

//枚举系统字体
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

//返回文件标题,不含后缀名
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

//遍历文件夹,搜索*.cst
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
	lf.lfCharSet = DEFAULT_CHARSET;
	m_listFontName.GetWindowText(lf.lfFaceName,32);
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
	PaintFont();
}

void CFontMakerDlg::OnCharChange()
{
	CString str;
	m_wChar = m_charset.GetChar(m_nCharIndex);
	str.Format(L"%04X",m_wChar);
	SetDlgItemText(IDC_TEXT_UNICODE,str);
	PaintFont();
}

//标准字符集变更
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
		m_spCurrIndex.SetRange32(0, 0);
		SetDlgItemInt(IDC_TEXT_COUNT, 0);
		m_nCharIndex = 0;
		MessageBox(L"加载字符集文件失败!",L"错误提示",MB_OK|MB_ICONWARNING);
		return;
	}
	count = m_charset.GetCharCount();
	m_spCurrIndex.SetRange32(0, count-1);
	SetDlgItemInt(IDC_TEXT_COUNT, count);
	m_nCharIndex = 0;
	OnCharChange();
}

//用户码表变更
void CFontMakerDlg::OnCharTableChange()
{
	UINT count;
	if(m_ebox.m_pzTable != NULL)
	{
		m_charset.Delete();
		m_charset.Create(m_ebox.m_pzTable);
		count = m_charset.GetCharCount();
		m_spCurrIndex.SetRange32(0, count-1);
		SetDlgItemInt(IDC_TEXT_COUNT, count);
		m_nCharIndex = 0;
		OnCharChange();
	}
}

// CFontMakerDlg 消息处理程序
BOOL CFontMakerDlg::OnInitDialog()
{
	int count;
	CDialog::OnInitDialog();
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标
	SetWindowText(L"通用点阵提取工具");
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
	m_listFormat.SetCurSel(0);
	m_spFontSize.SetRange(1,+999);
	m_spFontWidth.SetRange(1,999);
	m_spFontHeight.SetRange(1,999);
	m_spOffsetX.SetRange(-999,+999);
	m_spOffsetY.SetRange(-999,+999);

	CheckDlgButton(IDC_BTN_SCAN1,1);
	CheckDlgButton(IDC_BTN_MSB,1);
	SetDlgItemInt(IDC_EDIT_WIDTH,16);
	SetDlgItemInt(IDC_EDIT_HEIGHT,16);
	SetDlgItemInt(IDC_EDIT_HORZ,0);
	SetDlgItemInt(IDC_EDIT_VERT,0);
	SetDlgItemInt(IDC_EDIT_FONT_SIZE,16);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

//更新预览图
void CFontMakerDlg::PaintFont()
{
	m_bitfont.PaintFont(m_wChar);
	m_draw.Invalidate(FALSE);
}

//编辑码表
void CFontMakerDlg::OnBnClickedBtnEdit()
{
	m_ebox.DoModal();
	OnCharTableChange();
}


//字体变更
void CFontMakerDlg::OnCbnSelchangeListFontName()
{
	OnFontChange();
}

//字形变更
void CFontMakerDlg::OnCbnSelchangeListFontStyle()
{
	OnFontChange();
}

//字号变更
void CFontMakerDlg::OnEnChangeEditFontSize()
{
	if(m_bInitOK)
	{
        m_nFontSize = GetDlgItemInt(IDC_EDIT_FONT_SIZE);
		OnFontChange();
	}
}

//字符集变更
void CFontMakerDlg::OnCbnSelchangeListCharset()
{
	OnCharsetChange();
}

//选中标准字库
void CFontMakerDlg::OnBnClickedBtnStd()
{
	m_listCharset.EnableWindow(1);
	m_btnEdit.EnableWindow(0);
	OnCharsetChange();
}

//选中用户字库
void CFontMakerDlg::OnBnClickedBtnUser()
{
	m_listCharset.EnableWindow(0);
	m_btnEdit.EnableWindow(1);
	OnCharTableChange();
}

//点阵宽度变更
void CFontMakerDlg::OnEnChangeEditWidth()
{
	if(m_bInitOK)
	{
		m_nFontWidth = GetDlgItemInt(IDC_EDIT_WIDTH);
		m_bitfont.SetSize(m_nFontWidth,m_nFontHeight);
		m_draw.SetSize(m_nFontWidth,m_nFontHeight);
		PaintFont();
	}
}

//点阵高度变更
void CFontMakerDlg::OnEnChangeEditHeight()
{
	if(m_bInitOK)
	{
		m_nFontHeight = GetDlgItemInt(IDC_EDIT_HEIGHT);
		m_bitfont.SetSize(m_nFontWidth,m_nFontHeight);
		m_draw.SetSize(m_nFontWidth,m_nFontHeight);
		PaintFont();
	}
}

//水平偏移变更
void CFontMakerDlg::OnEnChangeEditHorz()
{
	if(m_bInitOK)
	{
		m_nOffsetX = GetDlgItemInt(IDC_EDIT_HORZ);
		m_bitfont.SetOffset(m_nOffsetX,m_nOffsetY);
		PaintFont();
	}
}

//垂直偏移变更
void CFontMakerDlg::OnEnChangeEditVert()
{
	if(m_bInitOK)
	{
		m_nOffsetY = GetDlgItemInt(IDC_EDIT_VERT);
		m_bitfont.SetOffset(m_nOffsetX,m_nOffsetY);
		PaintFont();
	}
}

//当前序号变更
void CFontMakerDlg::OnEnChangeEditIndex()
{
	UINT count;
	UINT index;
	CString str;
	count = m_charset.GetCharCount();
	index = GetDlgItemInt(IDC_EDIT_INDEX);
	if(index < count)
	{
		m_nCharIndex = index;
		OnCharChange();
	}
}

//生成BIN文件 
BOOL CFontMakerDlg::CreateBinFile(CFile* pFile, int mode)
{
	INT i;
	INT count;
	INT len;
	INT size;
	WCHAR ch;
	BYTE* bitmap;
	size = m_bitfont.GetBits(mode,NULL,0);
	bitmap = (BYTE*)malloc(size);
	if(bitmap==NULL)
	{
		return 0;
	}
	count = m_charset.GetCharCount();
	for(i=0;i<count;i++)
	{
		ch = m_charset.GetChar(i);
		m_bitfont.PaintFont(ch);
		len = m_bitfont.GetBits(mode,bitmap,size);
		pFile->Write(bitmap,len);
	}
	free(bitmap);
	return 1;
}

static char hex_encode(char ch)
{
	static const char hex_code[]={"0123456789ABCDEF"};
	return hex_code[ch];
}

static int bin_to_hex(char* output, void* input, int len)
{
	int i;
	char* start;
	char* pch;
	start = output;
	pch = (char*)input;
	for(i=0; i<len; i++)
	{
		*output++ = '0';
		*output++ = 'x';
		*output++ = hex_encode((*pch>>4)&0x0F);
		*output++ = hex_encode((*pch)&0x0F);
		*output++ = ',';
		if((i&0xF)==0xF)
		{
			*output++ = '\r';
			*output++ = '\n';
		}
		pch++;
	}
	*output = 0;
	return (int)(output-start);
}

static int UnicodeToGBK(const WCHAR* input, char* output, int size)
{
    return WideCharToMultiByte(CP_ACP, 0, input, 1, output, size, NULL, NULL);		
}

//生成C文件 
BOOL CFontMakerDlg::CreateCFile(CFile* pFile, int mode)
{
	INT i;
	INT count;
	INT len;
	INT str_len;
	INT mem_len;
	WCHAR ch;
    CHAR ch_ansi[8];
	CHAR font_name[32];
    CHAR font_style[32];
	char* ansi;
	BYTE* bitmap;
	PCSTR scan_mode[4]=
	{
		"水平扫描,MSB",
		"垂直扫描,MSB",
		"水平扫描,LSB",
		"垂直扫描,LSB",
	};
	mem_len = 128*1024;
	bitmap = (BYTE*)malloc(mem_len);
	if(bitmap==NULL)
	{
		return 0;
	}
	ansi = (char*)malloc(mem_len);
	if(ansi==NULL)
	{
		free(bitmap);
		return 0;
	}
    
    ::GetWindowTextA(m_listFontName.GetSafeHwnd(), font_name, 32);
    ::GetWindowTextA(m_listFontStyle.GetSafeHwnd(), font_style, 32);
	count = m_charset.GetCharCount();
	str_len = sprintf_s(ansi, mem_len,
		"/******************************************************************************\r\n"
		"* 逻辑字体: %s, %s, %d\r\n"
        "* 点阵大小: %dx%d\r\n"
		"* 字符数量: %d\r\n"
		"* 扫描方式: %s\r\n"
		"*******************************************************************************/\r\n"
		"static const unsigned char data[]=\r\n{\r\n",
		font_name,font_style,m_nFontSize,m_nFontWidth,m_nFontHeight,count,scan_mode[mode]);
	pFile->Write(ansi,str_len);
	for(i=0;i<count;i++)
	{
		ch = m_charset.GetChar(i);
		m_bitfont.PaintFont(ch);
		len = m_bitfont.GetBits(mode,bitmap,mem_len);
        UnicodeToGBK(&ch, ch_ansi, 8);
		str_len = sprintf_s(ansi, mem_len, "//0x%04X(%s)\r\n", ch, ch_ansi);
		pFile->Write(ansi,str_len);
		str_len = bin_to_hex(ansi,bitmap,len);
		pFile->Write(ansi,str_len);
		str_len = sprintf_s(ansi, mem_len, "\r\n",ch);
		pFile->Write(ansi,str_len);
	}
	str_len = sprintf_s(ansi, mem_len, "};\r\n");
	pFile->Write(ansi,str_len);
	
	free(bitmap);
	free(ansi);
	return 1;
}

//保存字库
void CFontMakerDlg::OnBnClickedBtnSave()
{
	INT scan;
	INT msb;
	INT mode;
	INT type;
	CFile cf;
	CFileDialog fbox(0);
	CString szName;
	scan = IsDlgButtonChecked(IDC_BTN_SCAN2);
	msb  = IsDlgButtonChecked(IDC_BTN_LSB);
	mode = (msb<<1)+scan;
	type = m_listFormat.GetCurSel();
	if(fbox.DoModal()==IDOK)
	{
		szName=fbox.GetFileName();
		if(!cf.Open(szName,CFile::modeCreate|CFile::modeReadWrite|CFile::shareDenyNone))
		{
			MessageBox(L"创建目标文件失败!",L"操作失败",MB_OK|MB_ICONWARNING);
			return;
		}
		if(type==0)
		{
			CreateBinFile(&cf,mode);
		}
		else
		{
			CreateCFile(&cf,mode);
		}
		cf.Close();
		MessageBox(L"保存文件成功",L"操作完成",MB_OK|MB_ICONINFORMATION);
	}
}

void CFontMakerDlg::PostNcDestroy()
{
	// TODO: 在此添加专用代码和/或调用基类
	m_charset.Delete();
	CDialog::PostNcDestroy();
}
