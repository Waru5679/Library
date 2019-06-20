#pragma once
#include <Windows.h>
#include "Main.h"

constexpr float MIN_FRAME_TIME{ 1.0f / FRAME_RATE };

//フレームレート計算クラス
class CFrameRate
{
public:
	void Init();	//初期化
	void Release();	//解放

	void FrameCount();//フレーム計算

private:
	void Draw();	//描画

	DWORD m_NowTime;	//現在時刻
	DWORD m_StartTime;	//開始時刻
	DWORD m_DrawTime;	//描画時刻

	int m_Count;   //フレームレート
};

extern CFrameRate g_Frame;