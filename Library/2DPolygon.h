#pragma once

#include "CObj.h"
#include "Polygon.h"

//2Dポリゴンクラス
class C2DPolygon:public CObj2D
{
public:
	void Init();	//初期化
	void Update();	//更新
	void Draw();	//描画
private:
	RECT_F m_Dst;	//描画位置
	float m_fRad;	//回転
};