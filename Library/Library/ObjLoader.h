#pragma once

#include "Struct.h"

//OBJデータ読み込みクラス
class CObjLoader
{
public:

	//解放
	void Release();

	//Mesh取得
	MY_MESH* GetMesh(int Id);

	//Meshの読み込み
	void LoadMesh(int Id,const char* Name);

	//メッシュ描画
	void Draw( D3DMATRIX matWorld, MY_MESH* pMesh, ColorData* pColor);

	//OBJの読み込み
	HRESULT LoadObj(const char* FileName, MY_MESH* pMesh);
private:
	//マテリアルの読み込み
	HRESULT LoadMaterial(char* FileName, MY_MESH* pMesh);

	//最大と最小のチェック
	void MinAndMax(D3DXVECTOR3 Pos,MY_MESH* pMesh);

	//メッシュ
	vector<MY_MESH>m_Mesh;
};

extern CObjLoader g_Loader;