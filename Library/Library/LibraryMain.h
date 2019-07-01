#pragma once
#include "Main.h"

//ライブラリメインクラス
class CLibraryMain
{
private:
	CLibraryMain() {};	//コンストラクタ
	~CLibraryMain() {};	//デストラクタ

	//インスタンス
	static CLibraryMain* m_pInstance;

public:
	//インスタンス取得
	static CLibraryMain* GetInstance()
	{
		//インスタンス化されてなければインスタンス化
		if (m_pInstance == nullptr)
		{
			m_pInstance = new CLibraryMain();
		}
		return m_pInstance;
	}

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
