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
	//テクスチャ
	ID3D10ShaderResourceView* m_pTex;
	int m_Width;	//幅
	int m_Height;	//高さ
	int m_Id;		//管理用ID
};

//管理用　クラス
class CTask
{
public:
	void Update();	//更新
	void Draw();	//描画
	void Release();	//解放
		
	//登録
	void InsertObj(CObjBase* pObj, int Id);
	void Insert(ID3D10ShaderResourceView* pTex, int Id, int Width, int Height);
	
	//取得	
	CObjBase* GetObj(int Id);
	MY_TEXTURE* GetTex(int Id);
	
private:
	vector<CObjBase*> m_Obj;	//オブジェクト
	vector<MY_TEXTURE> m_Tex;	//テクスチャ
};

extern CTask g_Task;
