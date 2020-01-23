#pragma once
#include "Singleton.h"
#include "PmxStruct.h"
#include <stdio.h>

//pmx�f�[�^
struct PMX_DATA
{
	PMX_DATA()
	{	
		m_pVertex		= nullptr;	//���_�f�[�^
		m_pFace			= nullptr;	//�ʂ̃f�[�^
		m_pTex			= nullptr;	//�e�N�X�`���f�[�^
		m_pMaterial		= nullptr;	//�}�e���A���f�[�^
		m_pBone			= nullptr;	//�{�[���f�[�^
		m_pMorph		= nullptr;	//���[�t�f�[�^
		m_pDisplay		= nullptr;	//�\���g
		m_pRigidBody	= nullptr;	//����
		m_pJoint		= nullptr;	//Joint
	}
	~PMX_DATA()
	{
		//���_�f�[�^
		if (m_pVertex != nullptr)
		{
			delete[] m_pVertex;
			m_pVertex = nullptr;
		}
		//�ʂ̃f�[�^
		if (m_pFace != nullptr)
		{
			delete[] m_pFace;
			m_pFace = nullptr;
		}
		//�e�N�X�`���f�[�^
		if (m_pTex != nullptr)
		{
			delete[] m_pTex;
			m_pTex = nullptr;
		}
		//�}�e���A���f�[�^
		if (m_pMaterial != nullptr)
		{
			delete[] m_pMaterial;
			m_pMaterial = nullptr;
		}
		//�{�[���f�[�^
		if (m_pBone != nullptr)
		{
			delete[] m_pBone;
			m_pBone = nullptr;
		}
		//���[�t�f�[�^
		if (m_pMorph != nullptr)
		{
			delete[] m_pMorph;
			m_pMorph = nullptr;
		}
		//�\���g
		if (m_pDisplay != nullptr)
		{
			delete[] m_pDisplay;
			m_pDisplay = nullptr;
		}
		//����
		if (m_pRigidBody != nullptr)
		{
			delete[] m_pRigidBody;
			m_pRigidBody = nullptr;
		}
		//Joint
		if (m_pJoint != nullptr)
		{
			delete[] m_pJoint;
			m_pJoint = nullptr;
		}
	}

	int				m_VerNum;		//���_��
	int				m_FaceNum;		//�ʂ̐�
	int				m_TexNum;		//�e�N�X�`����
	int				m_MaterialNum;	//�}�e���A����
	int				m_BoneNum;		//�{�[����
	int				m_MorphNum;		//���[�t��
	int				m_DisplayNum;	//�\���g��
	int				m_RigidNum;		//���̐�
	int				m_JointNum;		//Joint��
	PMX_HEADER		m_Head;			//�w�b�_�[
	PMX_MODEL_INFO	m_ModelInfo;	//���f���f�[�^
	PMX_VERTEX*		m_pVertex;		//���_�f�[�^
	PMX_FACE*		m_pFace;		//�ʂ̃f�[�^
	PMX_TEXTURE*	m_pTex;			//�e�N�X�`���f�[�^
	PMX_MATERIAL*	m_pMaterial;	//�}�e���A���f�[�^
	PMX_BONE*		m_pBone;		//�{�[���f�[�^
	PMX_MORPH*		m_pMorph;		//���[�t�f�[�^	
	PMX_DISPLAY*	m_pDisplay;		//�\���g�f�[�^
	PMX_RIGIT_BODY* m_pRigidBody;	//���̃f�[�^
	PMX_JOINT*		m_pJoint;		//Joint�f�[�^

};

//PMX�ǂݍ���
class CPmxLoader :public CSingleton<CPmxLoader>
{
private:
	//�V���O���g��
	friend CSingleton<CPmxLoader>;
	CPmxLoader() {};
	~CPmxLoader() {};
public:

	//�ǂݍ���
	bool Load(const char* FileName, PMX_DATA* pPmxData);
	
	//�f�[�^���܂Ƃ߂�
	void DataPut(PMX_DATA* pPmxData);
	
	//�����o��
	bool Write(const char* FileName, PMX_DATA* pPmxData);
private:
	//�w�b�_���ǂ݂���
	void HeadLoad(FILE* fp, PMX_DATA* pPmxData);

	//���f�����ǂݍ���
	void ModelInfoLoad(FILE* fp, PMX_DATA* pPmxData);

	//���_�ǂݍ���
	bool VertexLoad(FILE* fp, PMX_DATA* pPmxData);
	
	//�ʓǂݍ���
	bool FaceLoad(FILE* fp, PMX_DATA* pPmxData);
	
	//�e�N�X�`���ǂݍ���
	bool TextureLoad(FILE* fp, PMX_DATA* pPmxData);

	//�}�e���A���ǂݍ���
	bool MaterialLoad(FILE* fp, PMX_DATA* pPmxData);

	//�{�[���ǂݍ���
	bool BoneLoad(FILE* fp, PMX_DATA* pPmxData);

	//���[�t�ǂݍ���
	bool MorphLoad(FILE* fp, PMX_DATA* pPmxData);

	//�\���g�ǂݍ���
	bool DisplayFrameLoad(FILE* fp, PMX_DATA* pPmxData);

	//���̓ǂݍ���
	bool RigidBodyLoad(FILE* fp, PMX_DATA* pPmxData);

	//Joint�ǂݍ���
	bool JointLoad(FILE* fp, PMX_DATA* pPmxData);
};
