#pragma once

#include "Main.h"
#include "Singleton.h"
#include "Struct.h"
#include "LibraryHeader.h"
#include "X_Struct.h"
#include <stdio.h>

//Xファイル関連のクラス
class CX_Skin : public CSingleton<CX_Skin>
{
private:
	//シングルトン
	friend CSingleton<CX_Skin>;
	CX_Skin() {};	//コンストラクタ
	~CX_Skin() {};	//デストラクタ
public:
	void Release();//開放

	//スキンメッシュの読み込み
	bool LoadSkinMesh(const char* FileName, SKIN_MESH* pSkinMesh);

	//アニメーション
	void Animation(int AnimeId, int NowFrame, SKIN_MESH* pSkinMesh);

	//メッシュ描画(テスト用)
	void DrawMesh(D3DMATRIX matWorld, SKIN_MESH* pSkinMesh, CColorData* pColor);
private:
	
	//ボーンの更新
	void BoneUpdate(SKIN_MESH* pSkin, int AnimeId, int NowFrame);

	//ポーズを取得する
	D3DXMATRIX GetPose(bool* bFind, SKIN_MESH* pSkin, BONE* pBone, ANIMATION Anime, int NowFrame, int BoneID);

	//ウェイトが大きい順にソートする
	void WeightSort(SKIN_MESH* pSkin);

	//フレーム補完
	KEY FrameComplement(int NowFrame, ANIMATOIN_KEY AnimKey);
	SKIN_MESH m_SkinMesh;	//スキンメッシュ
};