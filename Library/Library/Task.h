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
	//void Insert(ID3D10ShaderResourceView* pTex, int Id, int Width, int Height);
	
	//�擾	
	CObjBase* GetObj(int Id);
	
private:
	list<CObjBase*> m_Obj;	//�I�u�W�F�N�g
	list<CTextureManager*> m_Tex;	//�e�N�X�`��
};

extern CTask g_Task;
