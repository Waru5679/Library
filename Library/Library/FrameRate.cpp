#include "FrameRate.h"
#include "Font.h"

CFrameRate g_Frame;

//初期化
void CFrameRate::Init()
{
	//精度を上げる
	timeBeginPeriod(1);

	//開始時刻取得
	m_StartTime = timeGetTime();
	
	//描画時刻
	m_DrawTime = m_StartTime;
		
	//カウンタ
	m_Count = 0;
}

//フレームレートの計算
void CFrameRate::FrameCount()
{
	//カウンタ更新
	m_Count++;

	//現在時刻取得
	m_NowTime = timeGetTime();

	//このフレームでかかった時間
	float FrameTime = static_cast<float>(m_NowTime - m_StartTime)/1000;

	//時間に余裕があるとき
	if ( FrameTime < MIN_FRAME_TIME)
	{
		//余分な時間
		DWORD SleepTime = static_cast<DWORD>((MIN_FRAME_TIME - FrameTime)* 1000);
		
		//待機
		Sleep(SleepTime);
	}

	//1秒経過してるとき
	float draw_time = static_cast<float>(m_NowTime - m_DrawTime) / 1000;
	if ( draw_time >= 1.0f)
	{
		//描画時刻更新
		m_DrawTime = m_NowTime;

		//フレーム数描画
		Draw();

		//カウンタ初期化
		m_Count = 0;
	}

	//開始時刻更新
	m_StartTime = m_NowTime;
}

//描画
void CFrameRate::Draw()
{
	//変換
	wchar_t buf[12];
	_itow_s(m_Count, buf, 10);

	g_Font.DrawStr(buf, 0.0f, 0.0f, 32.0f, 0.0f);
}

//解放
void CFrameRate::Release()
{
	//精度を戻す
	timeEndPeriod(1);
}