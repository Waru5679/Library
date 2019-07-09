#pragma once

//ライブラリファイル
#include "../Library/CObj.h"
#include "../Library/LibraryHeader.h"

//2Dポリゴンクラス
class C2DPolygon:public CObj2DBase
{
public:
	void Init();	//初期化
	void Update();	//更新
	void Draw();	//描画
private:
	ColorData m_Color;	//描画色
	RECT_F m_Src;		//切り取り位置
	RECT_F m_Dst;		//描画位置
	float m_fRad;		//回転
};