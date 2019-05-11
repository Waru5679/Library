#include"Main.h"
#include <vector>
using namespace std;


//RECT_F
struct RECT_F
{
	float m_top;
	float m_left;
	float m_right;
	float m_bottom;
};

//頂点構造体
struct MY_VERTEX
{
	D3DXVECTOR3 vPos;
	D3DXVECTOR3 vNorm;
	D3DXVECTOR2 vTex;
};

//マテリアル構造体
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
