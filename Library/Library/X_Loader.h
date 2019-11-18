#pragma once

#include "Main.h"

#include "LibraryHeader.h"
#include "Singleton.h"
#include "Release.h"
#include "X_Struct.h"
#include <stdio.h>

//Xファイル読みこみクラス
class CX_Loader:public CSingleton<CX_Loader>
{
private:
	//シングルトン
	friend CSingleton<CX_Loader>;
	CX_Loader() {};
	~CX_Loader() {};
public:

	int GetBoneNum(FILE* fp, long lStartPos);		//ボーン数の取得
	int GetSkinWeightNum(FILE* fp, long lStartPos);	//スキンウェイトの数を取得
	int GetAnimeNum(FILE* fp, long lStartPos);		//アニメーションの数を取得
	
	//templateを飛ばした読み込み開始位置を取得する
	long GetTemplateSkipStartPos(FILE* fp);

	//メッシュ情報の読み込み
	bool LoadMesh(FILE* fp, MESH* pMesh, SKIN_MESH_HEADER* pSkinHeader, long lStartPos);

	//スキンメッシュヘッダー読み込み
	void LoadSkinMeshHeader(FILE* fp, SKIN_MESH_HEADER* pSkinHeader, long lStartPos);

	//ボーン読み込み
	bool LoadBone(FILE* fp, BONE* pBone, long lStartPos);

	//ボーン情報の読み込み(再起関数))
	BONE LoadBoneInfo(FILE* fp, int* pBoneIndex, BONE* pBone);

	//スキンウェイトの読み込み
	bool LoadSkinWeight(FILE* fp, SKIN_WEIGHT* pSkinWeight, long lStartPos);

	//アニメーション読み込み
	bool LoadAnimation(FILE* fp, ANIMATION* pAnime, long lStartPos);

	//スキンメッシュにまとめる
	void SkinMeshPutTogether(MESH Mesh, BONE* pBone, int BoneNum, SKIN_WEIGHT* pSkinWeight, int WeightNum, ANIMATION* pAnimation, int AnimeNum, SKIN_MESH* pSkinMesh, SKIN_MESH_HEADER SkinHeader);

	//ボーン毎のキー情報読み込み
	BONE_KEY LoadBoneKey(FILE* fp);

	//アニメーションキーの数を取得する
	int GetAnimeKeyNum(FILE* fp);

	//アニメーションキーの読み込み
	ANIMATOIN_KEY LoadAnimationKey(FILE* fp);

	//スキンウェイトの情報をもとに各頂点に対応ボーンとウェイトの情報を持たせる
	void VertexMatchBone(SKIN_MESH* pSkin);

	//指定文字を文字列から消す
	void ErasCharFromString(char* pSource, int Size, char Erace);
};