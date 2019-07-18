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
	
	//�擾	
	CObjBase* GetObj(int Id);
	
private:
	list<CObjBase*> m_Obj;			//�I�u�W�F�N�g
};

extern CTask g_Task;
