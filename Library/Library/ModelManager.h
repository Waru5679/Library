#pragma once

#include "Singleton.h"

//�}�e���A���f�[�^
class CMaterialData
{

};

//���f���f�[�^�N���X
class CModelData
{
	//vector<MY_MATERIAL> Material;	//�}�e���A��

	//D3DXVECTOR3 vMin;	//���_�̍ŏ����W
	//D3DXVECTOR3 vMax;	//���_�̍ő���W

	int Id;//�Ǘ��p��ID
};

//�I�u�W�F�N�g�Ǘ��N���X
class ModelManager :public CSingleton<ModelManager>
{

};