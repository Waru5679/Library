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

	//2D�`��(Font�Ȃǐ؂���ʒu�̂Ȃ����̗p�̒��p)
	void Draw2D(int TexId, RECT_F* pSrc, RECT_F* pOut, float fColor[4], float fRad);

	//2D�`��(�e�N�X�`���̃T�C�Y����؂���ʒu�̐ݒ肪������̗p���p)
	void Draw2D(ID3D10ShaderResourceView* pTex, RECT_F* pOut, float fRad);

	//2D�`��
	void Draw2D(ID3D10ShaderResourceView* pTex, float fSrc[4], RECT_F* pOut, float fColor[4], float fRad);


	//�e�N�X�`���ǂݍ���
	void LoadTexture(int Id, const char* Name,int Width, int Height);

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