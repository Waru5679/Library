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
	}
	//デストラクタ
	~CCharClass()
	{
		m_pc.reset();
		SAFE_RELEASE(m_pTexResView);
	}

	//文字テクスチャ作成
	void CreateCharTex(wchar_t c, HDC hdc, TEXTMETRIC TM);
	
	//識別文字取得
	wchar_t* GetChar() { return m_pc.get(); }

	//テクスチャ取得
	ID3D10ShaderResourceView* GetTex() { return m_pTexResView; }

private:
	//空テクスチャ作成
	ID3D10Texture2D* CreateTexture();

	//ShaderResourceView作成
	ID3D10ShaderResourceView* CreateResourceView(ID3D10Texture2D* pTex);
	
	unique_ptr<wchar_t>			m_pc;			//識別用文字
	ID3D10ShaderResourceView*	m_pTexResView;	//テクスチャ
};

//Fontクラス
class CFont
{
public:
	void Init();	//初期化
	void Release();	//解放

	//文字列描画
	void DrawStr(const wchar_t* Str,float Pos_x,float Pos_y,float FontSize,float Rad);

	//文字列を元に文字テクスチャを作成
	void CreateStrTex(const wchar_t* str);
private:	
	HDC m_hdc;			//ディスプレイデバイスコンテキストのハンドル
	TEXTMETRIC m_TM;	//フォント情報格納用

	list< unique_ptr<CCharClass>>* m_Char; //文字リスト
};

extern CFont g_Font;