#pragma once
#include "main.h"
#include <math.h>

//成分比較用
#define COMP_X 0
#define COMP_Y 1
#define COMP_Z 2

//ベクトルの掛け算
D3DXVECTOR3 operator* (D3DXVECTOR3 Vec1,D3DXVECTOR3 Vec2);

//ベクトルの割り算
D3DXVECTOR3 operator/ (D3DXVECTOR3 Vec1, D3DXVECTOR3 Vec2);

//PosからTrans行列を作る
D3DXMATRIX MakeMatTrans(D3DXVECTOR3 Pos);

//AngleからRot行列を作る
D3DXMATRIX MakeMatRot(D3DXVECTOR3 Angle);

//ScaleからScale行列を作る
D3DXMATRIX MakeMatScale(D3DXVECTOR3 Scale);

//Pos,Angle,ScaleからWorld行列を作る
D3DXMATRIX MakeMatWorld(D3DXVECTOR3 Pos, D3DXVECTOR3 Angle, D3DXVECTOR3 Scale);

//絶対値ベクトルを作る
D3DXVECTOR3 MakeAbsVector3(D3DXVECTOR3 Vec);

//ベクトルが同じ方向を向いているかどうかをしらべる
bool SameDirectionVector3(D3DXVECTOR3 Vec1, D3DXVECTOR3 Vec2);

//最も長い成分を調べる
int MostLongComponent(D3DXVECTOR3 Vec);

//スクリーン座標から3D座標を求める
D3DXVECTOR3* CalcScreenToWorld(D3DXVECTOR3* pOut, float Sx, float Sy, float fZ, 
	int Screen_w, int Screen_h,	D3DXMATRIX* matView,D3DXMATRIX* matProj);

//頂点シェーダ用のマトリックスの作成
void CreateVSMatrix(D3DXVECTOR3* vEye, D3DXVECTOR3* vLook, D3DXVECTOR3* vUp, D3DXMATRIX* matView, D3DXMATRIX* matProj, int width, int height);

//色セット
void SetColor(float r, float g,float b, float a, float pColor[4]);
