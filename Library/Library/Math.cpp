#include "Math.h"

//�x�N�g���̊|���Z
D3DXVECTOR3 operator* (D3DXVECTOR3 Vec1, D3DXVECTOR3 Vec2)
{
	D3DXVECTOR3 Out;
	Out.x = Vec1.x*Vec2.x;
	Out.y = Vec1.y*Vec2.y;
	Out.z = Vec1.z*Vec2.z;

	return Out;
}

//�x�N�g���̊���Z
D3DXVECTOR3 operator/ (D3DXVECTOR3 Vec1, D3DXVECTOR3 Vec2)
{
	D3DXVECTOR3 Out;

	if (Vec2.x == 0)
		Vec1.x = 0.0f;
	else
		Out.x = Vec1.x/Vec2.x;
	
	if (Vec2.y == 0)
		Vec1.y = 0.0f;
	else
		Out.y = Vec1.y/Vec2.y;
	
	if (Vec2.z == 0)
		Vec1.z = 0.0f;
	else
		Out.z = Vec1.z/Vec2.z;

	return Out;
}

//Pos����Trans�s������
D3DXMATRIX MakeMatTrans(D3DXVECTOR3 Pos)
{
	D3DXMATRIX matTrans;
 	D3DXMatrixTranslation(&matTrans, Pos.x, Pos.y, Pos.z);

	return matTrans;
}

//Angle����Rot�s������
D3DXMATRIX MakeMatRot(D3DXVECTOR3 Angle)
{
	D3DXMATRIX matRot;
	D3DXMatrixRotationYawPitchRoll(&matRot, Angle.y, Angle.x, Angle.z);

	return matRot;
}

//Scale����Scale�s������
D3DXMATRIX MakeMatScale(D3DXVECTOR3 Scale)
{
	D3DXMATRIX matScale;
	D3DXMatrixScaling(&matScale, Scale.x, Scale.y, Scale.z);

	return matScale;
}

//Pos,Angle,Scale���烏�[���h�s������
D3DXMATRIX MakeMatWorld(D3DXVECTOR3 Pos, D3DXVECTOR3 Angle, D3DXVECTOR3 Scale)
{
	D3DXMATRIX matTrans,matRot,matScale,matWorld;

	D3DXMatrixTranslation(&matTrans, Pos.x, Pos.y, Pos.z);
	D3DXMatrixRotationYawPitchRoll(&matRot, Angle.y, Angle.x, Angle.z);
	D3DXMatrixScaling(&matScale, Scale.x, Scale.y, Scale.z);

	matWorld = matScale * matRot* matTrans;

	return matWorld;
}


//��Βl�x�N�g�������
D3DXVECTOR3 MakeAbsVector3(D3DXVECTOR3 Vec)
{
	D3DXVECTOR3 out;

	out.x = fabsf(Vec.x);
	out.y = fabsf(Vec.y);
	out.z = fabsf(Vec.z);

	return out;
}

//�ł����������𒲂ׂ�
int MostLongComponent(D3DXVECTOR3 Vec)
{
	Vec=MakeAbsVector3(Vec);

	if (Vec.x > Vec.y)
	{
		if (Vec.x > Vec.z)
			return COMP_X;
		else
			return COMP_Z;
	}
	else
	{
		if (Vec.y > Vec.z)
			return COMP_Y;
		else
			return COMP_Z;
	}
}

//���_�V�F�[�_�p�̃}�g���b�N�X�̍쐬
void CreateVSMatrix(D3DXVECTOR3* vEye, D3DXVECTOR3* vLook, D3DXVECTOR3* vUp, D3DXMATRIX* matView, D3DXMATRIX* matProj, int width, int height)
{
	//�r���[�s����쐬����B�t������]
	D3DXMatrixLookAtLH(matView, vEye, vLook, vUp);

	//�p�[�X�y�N�e�B�u�ˉe�s����쐬����B �g��E�k���@����p�@60�x�@�A�X�y�N�g��@����/�c�����@�j�APlane�@�t�@�[Plane�@z=10000
	D3DXMatrixPerspectiveFovLH(matProj, (float)D3DX_PI * 0.5f, (float)width / (float)height, 0.1f, 10000.0f);
}

//�F�Z�b�g
void SetColor(float r, float g, float b, float a, float pColor[4])
{
	pColor[0] = r;
	pColor[1] = g;
	pColor[2] = b;
	pColor[3] = a;
}