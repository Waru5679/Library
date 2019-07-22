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
	void SetCamera(CCameraBase* pCamera);

	//取得	
	CObjBase* GetObj(int Id);
	CCameraBase* GetCamera() { return m_pCamera; }
	
private:
	list<CObjBase*> m_Obj;			//オブジェクト
	CCameraBase* m_pCamera;
};

extern CTask g_Task;
