#include "Math.h"

//ベクトルの掛け算
D3DXVECTOR3 operator* (D3DXVECTOR3 Vec1, D3DXVECTOR3 Vec2)
{
	D3DXVECTOR3 Out;
	Out.x = Vec1.x*Vec2.x;
	Out.y = Vec1.y*Vec2.y;
	Out.z = Vec1.z*Vec2.z;

	return Out;
}

//ベクトルの割り算
D3DXVECTOR3 operator/ (D3DXVECTOR3 Vec1, D3DXVECTOR3 Vec2)
{
	D3DXVECTOR3 Out;

	if (Vec2.x == 0)
		Vec1.x = 0.0f;
	else
		Out.x = Vec1.x/Vec2.x;
	
	if (Vec2.y == 0)
		Vec1.y = 0.0f;
	else
		Out.y = Vec1.y/Vec2.y;
	
	if (Vec2.z == 0)
		Vec1.z = 0.0f;
	else
		Out.z = Vec1.z/Vec2.z;

	return Out;
}

//PosからTrans行列を作る
D3DXMATRIX MakeMatTrans(D3DXVECTOR3 Pos)
{
	D3DXMATRIX matTrans;
 	D3DXMatrixTranslation(&matTrans, Pos.x, Pos.y, Pos.z);

	return matTrans;
}

//AngleからRot行列を作る
D3DXMATRIX MakeMatRot(D3DXVECTOR3 Angle)
{
	D3DXMATRIX matRot;
	D3DXMatrixRotationYawPitchRoll(&matRot, Angle.y, Angle.x, Angle.z);

	return matRot;
}

//ScaleからScale行列を作る
D3DXMATRIX MakeMatScale(D3DXVECTOR3 Scale)
{
	D3DXMATRIX matScale;
	D3DXMatrixScaling(&matScale, Scale.x, Scale.y, Scale.z);

	return matScale;
}

//Pos,Angle,Scaleからワールド行列を作る
D3DXMATRIX MakeMatWorld(D3DXVECTOR3 Pos, D3DXVECTOR3 Angle, D3DXVECTOR3 Scale)
{
	D3DXMATRIX matTrans,matRot,matScale,matWorld;

	D3DXMatrixTranslation(&matTrans, Pos.x, Pos.y, Pos.z);
	D3DXMatrixRotationYawPitchRoll(&matRot, Angle.y, Angle.x, Angle.z);
	D3DXMatrixScaling(&matScale, Scale.x, Scale.y, Scale.z);

	matWorld = matScale * matRot* matTrans;

	return matWorld;
}


//絶対値ベクトルを作る
D3DXVECTOR3 MakeAbsVector3(D3DXVECTOR3 Vec)
{
	D3DXVECTOR3 out;

	out.x = fabsf(Vec.x);
	out.y = fabsf(Vec.y);
	out.z = fabsf(Vec.z);

	return out;
}

//最も長い成分を調べる
int MostLongComponent(D3DXVECTOR3 Vec)
{
	Vec=MakeAbsVector3(Vec);

	if (Vec.x > Vec.y)
	{
		if (Vec.x > Vec.z)
			return COMP_X;
		else
			return COMP_Z;
	}
	else
	{
		if (Vec.y > Vec.z)
			return COMP_Y;
		else
			return COMP_Z;
	}
}

//スクリーン座標から3D座標を求める
D3DXVECTOR3* CalcScreenToWorld(
	D3DXVECTOR3* pOut,
	float Sx,  // スクリーンX座標
	float Sy,  // スクリーンY座標
	float fZ,  // 射影空間でのZ値（0～1）
	int Screen_w,
	int Screen_h,
	D3DXMATRIX* matView,
	D3DXMATRIX* matProj
) {
	// 各行列の逆行列を算出
	D3DXMATRIX InvView, InvPrj, VP, InvViewport;
	D3DXMatrixInverse(&InvView, NULL, matView);
	D3DXMatrixInverse(&InvPrj, NULL, matProj);
	D3DXMatrixIdentity(&VP);
	VP._11 = Screen_w / 2.0f; VP._22 = -Screen_h / 2.0f;
	VP._41 = Screen_w / 2.0f; VP._42 = Screen_h / 2.0f;
	D3DXMatrixInverse(&InvViewport, NULL, &VP);

	// 逆変換
	D3DXMATRIX tmp = InvViewport * InvPrj * InvView;
	D3DXVec3TransformCoord(pOut, &D3DXVECTOR3(Sx, Sy, fZ), &tmp);

	return pOut;
}

//頂点シェーダ用のマトリックスの作成
void CreateVSMatrix(D3DXVECTOR3* vEye, D3DXVECTOR3* vLook, D3DXVECTOR3* vUp, D3DXMATRIX* matView, D3DXMATRIX* matProj, int width, int height)
{
	//ビュー行列を作成する。逆方向回転
	D3DXMatrixLookAtLH(matView, vEye, vLook, vUp);

	//パースペクティブ射影行列を作成する。 拡大・縮小　視野角　60度　アスペクト比　横幅/縦高さ　ニアPlane　ファーPlane　z=10000
	D3DXMatrixPerspectiveFovLH(matProj, (float)D3DX_PI * 0.5f, (float)width / (float)height, 0.1f, 10000.0f);
}
