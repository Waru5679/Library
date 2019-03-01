#pragma once
#include <d3dx10.h>
#include <D3D10.h>
#include <vector>
#include <windef.h>

#include "CObj.h"
#include "Scene.h"
#include "Camera.h"
using namespace std;

//管理用　クラス
class CTask
{
public:
	void Update();	//更新
	void Draw();	//描画
	void Release();	//解放
		
	//登録
	void InsertObj(CObj* pObj, int Id);
	void Insert(MY_MESH Mesh, int Id);
	void InsertScene(CScene* Scene, int Id);
	void Insert(ID3D10ShaderResourceView* pTex, int Id);
	
	//取得	
	CObj* GetObj(int Id);
	MY_MESH GetMesh(int Id);
	CScene* GetScene(int Id);
	ID3D10ShaderResourceView* GetTex(int Id);
	
	//OBJリストデータ削除
	void ObjDelete();
private:
	vector<CScene*> m_Scene;					//シーン
	vector<MY_MESH> m_Mesh;						//メッシュ
	vector<CObj*> m_Obj;						//オブジェクト
	vector<ID3D10ShaderResourceView*> m_Tex;	//テクスチャ
};

extern CTask g_Task;
