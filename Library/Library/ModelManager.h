#pragma once

#include "Singleton.h"

//マテリアルデータ
class CMaterialData
{

};

//モデルデータクラス
class CModelData
{
	//vector<MY_MATERIAL> Material;	//マテリアル

	//D3DXVECTOR3 vMin;	//頂点の最小座標
	//D3DXVECTOR3 vMax;	//頂点の最大座標

	int Id;//管理用のID
};

//オブジェクト管理クラス
class ModelManager :public CSingleton<ModelManager>
{

};