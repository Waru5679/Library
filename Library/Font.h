#pragma once 
#include "Main.h"
#include <locale.h>
#include <wchar.h>
#include <memory>
#include <list>
#include "Polygon.h"

using namespace std;

//�������J���p
#define SAFE_RELEASE(p)      { if (p) { (p)->Release();    (p)=nullptr;  }	}
#define SAFE_RESET(p)		 { if (p) { (p).reset(nullptr);				 }  }

//�������ʃN���X
class CCharClass
{
public:
	//�R���X�g���N�^
	CCharClass()
	{
		m_pc.reset();
		m_pTexture = nullptr;
	}
	//�f�X�g���N�^
	~CCharClass()
	{
		m_pc.reset();
		SAFE_RELEASE(m_pTexResView);
		SAFE_RELEASE(m_pTexture);
	}

	wchar_t* GetChar() { return m_pc.get(); }
	ID3D10ShaderResourceView* GetTex() { return m_pTexResView; }

	//�����e�N�X�`���쐬
	void CreateCharTex(wchar_t c,HDC hdc,TEXTMETRIC TM);	
private:
	unique_ptr<wchar_t>			m_pc;			//���ʗp����
	ID3D10Texture2D*			m_pTexture;		//�����̃e�N�X�`�����
	ID3D10ShaderResourceView*	m_pTexResView;	//�e�N�X�`�����V�F�[�_�ɑ���
};

//Font�N���X
class CFont
{
public:
	static void Init();		//������
	static void Release();	//���

	//������`��
	static void DrawStr(const wchar_t* str,float x,float y,float size);

private:
	//����������ɕ����e�N�X�`�����쐬
	static void CreateStrTex(const wchar_t* str);

	static HFONT m_hFont;	//�t�H���g�n���h��(�_��)
	static HFONT m_oldFont;	//�t�H���g�n���h��(����)
	static HDC m_hdc;		//�f�B�X�v���C�f�o�C�X�R���e�L�X�g�̃n���h��
	static TEXTMETRIC m_TM;	//�t�H���g���i�[�p

	static list< unique_ptr<CCharClass>>* list_char_tex; //�������X�g
};

