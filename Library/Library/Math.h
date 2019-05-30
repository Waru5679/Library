#pragma once
#include "main.h"
#include <math.h>

//������r�p
#define COMP_X 0
#define COMP_Y 1
#define COMP_Z 2

//�x�N�g���̊|���Z
D3DXVECTOR3 operator* (D3DXVECTOR3 Vec1,D3DXVECTOR3 Vec2);

//�x�N�g���̊���Z
D3DXVECTOR3 operator/ (D3DXVECTOR3 Vec1, D3DXVECTOR3 Vec2);

//Pos����Trans�s������
D3DXMATRIX MakeMatTrans(D3DXVECTOR3 Pos);

//Angle����Rot�s������
D3DXMATRIX MakeMatRot(D3DXVECTOR3 Angle);

//Scale����Scale�s������
D3DXMATRIX MakeMatScale(D3DXVECTOR3 Scale);

//Pos,Angle,Scale����World�s������
D3DXMATRIX MakeMatWorld(D3DXVECTOR3 Pos, D3DXVECTOR3 Angle, D3DXVECTOR3 Scale);

//��Βl�x�N�g�������
D3DXVECTOR3 MakeAbsVector3(D3DXVECTOR3 Vec);

//�x�N�g�������������������Ă��邩�ǂ���������ׂ�
bool SameDirectionVector3(D3DXVECTOR3 Vec1, D3DXVECTOR3 Vec2);

//�ł����������𒲂ׂ�
int MostLongComponent(D3DXVECTOR3 Vec);

//�X�N���[�����W����3D���W�����߂�
D3DXVECTOR3* CalcScreenToWorld(D3DXVECTOR3* pOut, float Sx, float Sy, float fZ, 
	int Screen_w, int Screen_h,	D3DXMATRIX* matView,D3DXMATRIX* matProj);

//���_�V�F�[�_�p�̃}�g���b�N�X�̍쐬
void CreateVSMatrix(D3DXVECTOR3* vEye, D3DXVECTOR3* vLook, D3DXVECTOR3* vUp, D3DXMATRIX* matView, D3DXMATRIX* matProj, int width, int height);

//�F�Z�b�g
void SetColor(float r, float g,float b, float a, float pColor[4]);
