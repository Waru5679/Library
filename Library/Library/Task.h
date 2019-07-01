#pragma once
#include <d3dx10.h>
#include <D3D10.h>
#include <vector>
#include <windef.h>

#include "CObj.h"
#include "Scene.h"
#include "Camera.h"
using namespace std;

//�e�N�X�`���Ǘ��p
struct MY_TEXTURE
{
	//�e�N�X�`��
	ID3D10ShaderResourceView* m_pTex;
	int m_Width;	//��
	int m_Height;	//����
	int m_Id;		//�Ǘ��pID
};

//�Ǘ��p�@�N���X
class CTask
{
public:
	void Update();	//�X�V
	void Draw();	//�`��
	void Release();	//���
		
	//�o�^
	void InsertObj(CObjBase* pObj, int Id);
	void Insert(ID3D10ShaderResourceView* pTex, int Id, int Width, int Height);
	
	//�擾	
	CObjBase* GetObj(int Id);
	MY_TEXTURE* GetTex(int Id);
	
private:
	vector<CObjBase*> m_Obj;	//�I�u�W�F�N�g
	vector<MY_TEXTURE> m_Tex;	//�e�N�X�`��
};

extern CTask g_Task;
