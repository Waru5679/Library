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

	//読み込み
	HRESULT LoadMesh(const char* FileName, CModelData* pMesh);
	
	//モデル描画
	void Draw(D3DMATRIX matWorld, CModelData* pMesh, CColorData* pColor);

private:
	//マテリアルの読み込み
	HRESULT LoadMaterial(char* FileName, CModelData* pMesh);

	//最大と最小のチェック
	void MinAndMax(D3DXVECTOR3 Pos, CModelData* pMesh);
};