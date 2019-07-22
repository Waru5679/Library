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

//�Ǘ��p�@�N���X
class CTask
{
public:
	void Update();	//�X�V
	void Draw();	//�`��
	void Release();	//���
		
	//�o�^
	void InsertObj(CObjBase* pObj, int Id);
	void SetCamera(CCameraBase* pCamera);

	//�擾	
	CObjBase* GetObj(int Id);
	CCameraBase* GetCamera() { return m_pCamera; }
	
private:
	list<CObjBase*> m_Obj;			//�I�u�W�F�N�g
	CCameraBase* m_pCamera;
};

extern CTask g_Task;
