#pragma once 
#include "Main.h"
#include <locale.h>
#include <wchar.h>
#include <memory>
#include <list>
#include "Polygon.h"

using namespace std;

//メモリ開放用
#define SAFE_RELEASE(p)      { if (p) { (p)->Release();    (p)=nullptr;  }	}
#define SAFE_RESET(p)		 { if (p) { (p).reset(nullptr);				 }  }

//文字識別クラス
class CCharClass
{
public:
	//コンストラクタ
	CCharClass()
	{
		m_pc.reset();
		m_pTexture = nullptr;
	}
	//デストラクタ
	~CCharClass()
	{
		m_pc.reset();
		SAFE_RELEASE(m_pTexResView);
		SAFE_RELEASE(m_pTexture);
	}

	wchar_t* GetChar() { return m_pc.get(); }
	ID3D10ShaderResourceView* GetTex() { return m_pTexResView; }

	//文字テクスチャ作成
	void CreateCharTex(wchar_t c,HDC hdc,TEXTMETRIC TM);	
private:
	unique_ptr<wchar_t>			m_pc;			//識別用文字
	ID3D10Texture2D*			m_pTexture;		//文字のテクスチャ情報
	ID3D10ShaderResourceView*	m_pTexResView;	//テクスチャをシェーダに送る
};

//Fontクラス
class CFont
{
public:
	static void Init();		//初期化
	static void Release();	//解放

	//文字列描画
	static void DrawStr(const wchar_t* str,float x,float y,float size);

private:
	//文字列を元に文字テクスチャを作成
	static void CreateStrTex(const wchar_t* str);

	static HFONT m_hFont;	//フォントハンドル(論理)
	static HFONT m_oldFont;	//フォントハンドル(物理)
	static HDC m_hdc;		//ディスプレイデバイスコンテキストのハンドル
	static TEXTMETRIC m_TM;	//フォント情報格納用

	static list< unique_ptr<CCharClass>>* list_char_tex; //文字リスト
};

