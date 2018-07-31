#include "StdAfx.h"
#include "FileMaker.h"

static const char hex_encode(int ch)
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

static int UnicodeToUTF8(const WCHAR* input, char* output, int size)
{
    memset(output, 0, size);
    return WideCharToMultiByte(CP_UTF8, 0, input, 1, output, size, NULL, NULL);		
}

//生成C文件 
BOOL CFileMaker::MakeCppFile(CBitFont* pBitFont, CCharset* pCharset, CFile* pFile, int scan, BOOL msb, BOOL var_width)
{
	INT i;
	INT count;
	INT len;
	WCHAR ch;
    char  utf8[8];
	char* text;
    UINT  text_size;
	BYTE* bits;
    UINT  bits_size;

    bits_size = pBitFont->GetBits(NULL,0,scan,msb,var_width);
	bits = (BYTE*)malloc(bits_size);
	if(bits==NULL)
	{
		return FALSE;
	}
    
    text_size = bits_size * 8 + 64;
    text = (char*)malloc(text_size);
	if(text==NULL)
    {
        free(bits);
        return FALSE;
    }

	len = sprintf_s(text, text_size,"static const unsigned char bits[][%d]=\r\n{\r\n",bits_size);
	pFile->Write(text,len);

    count = pCharset->GetCharCount();
	for(i=0;i<count;i++)
	{
		ch = pCharset->GetChar(i);
        UnicodeToUTF8(&ch, utf8, 8);
		len = sprintf_s(text, text_size, "//U+%04X(%s)\r\n", ch, utf8);
		pFile->Write(text,len);
        pBitFont->PaintChar(ch);
		len = pBitFont->GetBits(bits,bits_size,scan,msb,var_width);
		len = bin_to_hex(text,bits,len);
		pFile->Write(text,len);
		len = sprintf_s(text, text_size, "\r\n",ch);
		pFile->Write(text,len);
	}
	len = sprintf_s(text, text_size, "};\r\n");
	pFile->Write(text,len);
	
	free(bits);
	free(text);
	return TRUE;
}

//生成BIN文件 
BOOL CFileMaker::MakeBinFile(CBitFont* pBitFont, CCharset* pCharset, CFile* pFile, int scan, BOOL msb, BOOL var_width)
{
	INT i;
	INT count;
	INT size;
	WCHAR ch;
	BYTE* bits;
    UINT  bits_size;
	bits_size = pBitFont->GetBits(NULL,0,scan,msb,var_width);
	bits = (BYTE*)malloc(bits_size);
	if(bits==NULL)
	{
		return FALSE;
	}
	count = pCharset->GetCharCount();
	for(i=0;i<count;i++)
	{
		ch = pCharset->GetChar(i);
		pBitFont->PaintChar(ch);
		size = pBitFont->GetBits(bits,bits_size,scan,msb,var_width);
		pFile->Write(bits,size);
	}
	free(bits);
	return TRUE;
}
