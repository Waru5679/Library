#pragma once

#include <d3dx10.h>
#include <vector>

using namespace std;

//頂点構造体　今回は、位置だけでなく法線も（マテリアル反映は法線が必要）
struct MY_VERTEX
{
	D3DXVECTOR3 vPos;
	D3DXVECTOR3 vNorm;
	D3DXVECTOR2 vTex;
};

//オリジナル　マテリアル構造体
struct MY_MATERIAL
{
	//マテリアル名
	CHAR szName[100];
	D3DXVECTOR3 Ka;//アンビエント(環境光)
	D3DXVECTOR3 Kd;//ディフューズ(拡散光)
	D3DXVECTOR3 Ks;//スペキュラー(鏡面反射光）
	
	//面の頂点の数
	vector<int>FaceOfVer;

	//ファイス情報
	vector<int>iFaceBuffer;

	//テクスチャ
	ID3D10ShaderResourceView* pTexture;

	//バッファー
	vector<ID3D10Buffer*> pIndex;
	vector<ID3D10Buffer*> pVertex;
};

//オリジナルメッシュ
struct MY_MESH
{
	vector<MY_MATERIAL> Material;	//マテリアル
	int MaterialNum;				//マテリアルの数
	
	D3DXVECTOR3 vMin;	//頂点の最小座標
	D3DXVECTOR3 vMax;	//頂点の最大座標
};

class CObjLoader
{
public:
	//Meshの読み込み
	void LoadMesh(int Id,const char* Name);

	//メッシュ描画
	void Draw(D3DMATRIX matWorld, MY_MESH* pMesh);
private:
	//OBJの読み込み
	HRESULT LoadObj(const char* FileName, MY_MESH* pMesh);

	//マテリアルの読み込み
	HRESULT LoadMaterial(char* FileName, MY_MESH* pMesh);

	//最大と最小のチェック
	void MinAndMax(D3DXVECTOR3 Pos,MY_MESH* pMesh);

	//ポリゴン描画
	void DrawMesh(int ver_num, ID3D10Buffer* VertexBuffer, ID3D10Buffer* IndexBuffer);
};

extern CObjLoader g_Loader;