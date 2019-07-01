#pragma once
#include "Main.h"

constexpr float MIN_FRAME_TIME{ 1000.0f / FRAME_RATE };

//フレームレート計算クラス
class CFrameRate
{
private:
	CFrameRate() {};	//コンストラクタ
	~CFrameRate() {};	//デストラクタ

	//インスタンス
	static CFrameRate* m_pInstance;

public:
	//インスタンス取得
	static CFrameRate* GetInstance()
	{
		//インスタンス化されてなければインスタンス化
		if (m_pInstance == nullptr)
		{
			m_pInstance = new CFrameRate();
		}
		return m_pInstance;
	}

	void Init();		//初期化
	void FrameCount();	//フレーム計算
	void Draw();		//描画
	void Release();		//解放
private:
	DWORD m_NowTime;	//現在時刻
	DWORD m_StartTime;	//開始時刻
	DWORD m_DrawTime;	//描画時刻

	int m_FrameRate;	//フレームレート
	int m_Count;		//カウンタ
		
	//変換
	wchar_t* m_pDrawStr;
};