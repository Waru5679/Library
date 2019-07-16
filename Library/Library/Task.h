#pragma once
#include <d3dx10.h>
#include <D3D10.h>
#include <list>
#include <windef.h>

#include "CObj.h"
#include "Scene.h"
#include "Camera.h"
#include "TextureManager.h"

using namespace std;


//管理用　クラス
class CTask
{
public:
	void Update();	//更新
	void Draw();	//描画
	void Release();	//解放
		
	//登録
	void InsertObj(CObjBase* pObj, int Id);
	//void Insert(ID3D10ShaderResourceView* pTex, int Id, int Width, int Height);
	
	//取得	
	CObjBase* GetObj(int Id);
	
private:
	list<CObjBase*> m_Obj;	//オブジェクト
	list<CTextureManager*> m_Tex;	//テクスチャ
};

extern CTask g_Task;
