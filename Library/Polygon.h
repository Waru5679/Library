#pragma once

#include "Main.h"
#include "Camera.h"
#include "ObjLoader.h"
#include <windef.h>

struct RECT_F
{
	float m_top;
	float m_left;
	float m_right;
	float m_bottom;
};

//�`��N���X
class CDraw
{
public:
	//������
	void Init();

	//�`��
	void Draw3D(int TexId, D3DXMATRIX matWorld);
	void Draw3D(ID3D10ShaderResourceView* pResView, D3DXMATRIX matWorld);
	void Draw2D(int TexId, RECT_F* Src, RECT_F*Out, float Color[4], float Rad);
	void Draw2D(ID3D10ShaderResourceView* pResView, RECT_F* Src, RECT_F*Out, float Color[4],float Rad);

	//�e�N�X�`���ǂݍ���
	void LoadTexture(int Id, const char* Name);

	//�J�����Z�b�g
	void SetCamera(Camera* pCamera) { m_pCamera = pCamera; }

	//���
	void Release();
	
private:
	void DrawPolygon();		//�|���S���`��
	HRESULT CreateBartex();	//�o�[�e�b�N�X�o�b�t�@�̍쐬
	
	ID3D10Buffer* m_pBuffer;//�o�[�e�b�N�X�o�b�t�@
	MY_VERTEX* m_Vertex;	//���_
	   	
	Camera* m_pCamera;	//�J�����|�C���^
};

//RECT_F�̃Z�b�g
void RectSet(float x, float y, float Size_x, float Size_y,RECT_F* pOut );


extern CDraw g_Draw;