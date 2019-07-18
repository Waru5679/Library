#pragma once

#include "ModelManager.h"
#include "Singleton.h"
#include <list>
using namespace std;

//OBJデータ読み込みクラス
class CObjLoader:public CSingleton<CObjLoader>
{
private:
	//シングルトン
	friend CSingleton<CObjLoader>;
	CObjLoader() {};	//コンストラクタ
	~CObjLoader() {};	//デストラクタ
public:
	//解放
	void Release();

	//メッシュ描画
	void Draw( D3DMATRIX matWorld, CModelData* pMesh, ColorData* pColor);

	//OBJの読み込み
	HRESULT LoadObj(const char* FileName, CModelData* pMesh);
private:
	//マテリアルの読み込み
	HRESULT LoadMaterial(char* FileName, CModelData* pMesh);

	//最大と最小のチェック
	void MinAndMax(D3DXVECTOR3 Pos, CModelData* pMesh);
};