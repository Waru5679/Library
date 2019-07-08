#pragma once
#include "Main.h"
#include "Singleton.h"

//ライブラリメインクラス
class CLibraryMain:public CSingleton<CLibraryMain>
{
private:
	//シングルトンクラスだけでインスタンス化する
	friend CSingleton<CLibraryMain>;	

	CLibraryMain() {};	//コンストラクタ
	~CLibraryMain() {};	//デストラクタ
public:
	//初期化
	bool Init(HINSTANCE hInst);

	//更新
	void Update();

	//メモリの開放
	void Release();	
private:	
	//window初期設定
	void WinInit(HINSTANCE hInst);

	//ウインドウハンドル
	HWND m_hWnd;
};
