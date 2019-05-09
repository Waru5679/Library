#pragma once
#include <d3dx10.h>
#include <D3D10.h>
#include <vector>
#include <windef.h>

#include "CObj.h"
#include "Scene.h"
#include "Camera.h"
using namespace std;

//テクスチャ管理用
struct MY_TEXTURE
{
	ID3D10ShaderResourceView* m_pTex;	//テクスチャ
	int m_Id;							//管理用ID

};

//シーン管理用
struct MY_SCENE
{
	CScene* m_pScene;	//シーン
	int m_Id;			//管理用ID
};

//管理用　クラス
class CTask
{
public:
	void Update();	//更新
	void Draw();	//描画
	void Release();	//解放
		
	//登録
	void InsertObj(CObj* pObj, int Id);
	void InsertScene(CScene* pScene, int Id);
	void Insert(ID3D10ShaderResourceView* pTex, int Id);
	
	//取得	
	CObj* GetObj(int Id);
	CScene* GetScene(int Id);
	ID3D10ShaderResourceView* GetTex(int Id);
	
	//OBJリストデータ削除
	void ObjDelete();
private:
	vector<MY_SCENE> m_Scene;	//シーン
	vector<CObj*> m_Obj;		//オブジェクト
	vector<MY_TEXTURE> m_Tex;	//テクスチャ
};

extern CTask g_Task;
