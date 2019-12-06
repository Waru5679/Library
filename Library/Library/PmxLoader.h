#pragma once
#include "Singleton.h"
#include <stdio.h>

struct	BDEF1
{
	BDEF1()
	{
		m_BoneID = -1;
	}
	int m_BoneID;
};

struct	BDEF2
{
	BDEF2()
	{
		m_BoneID[0] = -1;
		m_BoneID[1] = -1;
	}
	int m_BoneID[2];
	float m_Weight;
};

struct	BDEF4
{
	BDEF4()
	{
		m_BoneID[0] = -1;
		m_BoneID[1] = -1;
		m_BoneID[2] = -1;
		m_BoneID[3] = -1;
	}

	int m_BoneID[4];
	float m_Weight[4];
};

struct SDEF
{
	SDEF()
	{
		m_BoneID[0] = -1;
		m_BoneID[1] = -1;
	}
	int m_BoneID[2];
	float m_Weight;
	float m_Matrix[3][3];
};

//�E�F�C�g�f�[�^
struct PMX_WEIGHT_DATA
{
	unsigned char m_WeightType;//�E�F�C�g�ό`����
	BDEF1	m_Bdef1;	//BDEF1�`���̃f�[�^
	BDEF2	m_Bdef2;	//BDEF2�`���̃f�[�^
	BDEF4	m_Bdef4;	//BDEF4�`���̃f�[�^
	SDEF	m_Sdef;		//SDEF�`���̃f�[�^
};

//pmx�w�b�_�[
struct PMX_HEADER
{
	~PMX_HEADER()
	{
		delete[] m_pData;
		m_pData = nullptr;
	}
	char	m_FileType[4];	//�t�@�C�����"PMX_"�Œ�
	float	m_Ver;			//�o�[�W����
	char	m_Size;
	unsigned char*	m_pData;
};

//pmx���f�����
struct PMX_MODEL_INFO
{
	PMX_MODEL_INFO()
	{
		m_CommentEngSize = -1;
		m_CommentJpnSize = -1;
		m_NameEngSize = -1;
		m_NameJpnSize = -1;
		m_pNameJap = nullptr;
		m_pNameEng = nullptr;
		m_pCommentJap = nullptr;
		m_pCommentEng = nullptr;
	}
	~PMX_MODEL_INFO()
	{
		delete[] m_pNameJap;
		m_pNameJap = nullptr;
		delete[] m_pNameEng;
		m_pNameEng = nullptr;
		delete[] m_pCommentJap;
		m_pCommentJap = nullptr;
		delete[] m_pCommentEng;
		m_pCommentEng = nullptr;
	}
	int m_NameJpnSize;		//���f����(��)�T�C�Y
	int m_NameEngSize;		//���f����(�p)�T�C�Y
	int m_CommentJpnSize;	//�R�����g(��)�T�C�Y
	int m_CommentEngSize;	//�R�����g(�p)�T�C�Y
	
	unsigned char* m_pNameJap;	//���f����(��)
	unsigned char* m_pNameEng;	//���f����(�p)
	unsigned char* m_pCommentJap;//�R�����g(��)
	unsigned char* m_pCommentEng;//�R�����g(�p)
};

//pmx���_�f�[�^
struct PMX_VERTEX
{
	PMX_VERTEX()
	{
		m_pfAddUv = nullptr;
	}
	~PMX_VERTEX()
	{
		delete[] m_pfAddUv;
		m_pfAddUv = nullptr;
	}

	float m_fPos[3];
	float m_fNorm[3];
	float m_fUv[2];
	float* m_pfAddUv;	//�ǉ�UV
	PMX_WEIGHT_DATA m_WeightData;//�E�F�C�g�f�[�^
	float m_EdgeMagn;//�G�b�W�{��
};

//pmx�ʃf�[�^
struct PMX_FACE
{
	int m_VerIndex[3];//���_
};

//pmx�e�N�X�`��
struct PMX_TEXTURE
{
	PMX_TEXTURE()
	{
		m_pPass = nullptr;
	}
	~PMX_TEXTURE()
	{
		delete[] m_pPass;
		m_pPass = nullptr;
	}
	unsigned char* m_pPass;
};

//pmx�f�[�^
struct PMX_DATA
{
	~PMX_DATA()
	{
		delete[] m_pVertex;
		m_pVertex = nullptr;
		delete[] m_pFace;
		m_pFace = nullptr;
		delete[] m_pTex;
		m_pTex = nullptr;
	}

	PMX_HEADER	m_Head;		//�w�b�_�[
	PMX_MODEL_INFO m_ModelInfo;//���f���f�[�^
	int			m_VerNum;	//���_��
	PMX_VERTEX* m_pVertex;	//���_�f�[�^
	int			m_FaceNum;	//�ʂ̐�
	PMX_FACE*	m_pFace;	//�ʂ̃f�[�^
	int			m_TexNum;	//�e�N�X�`����
	PMX_TEXTURE* m_pTex;	//�e�N�X�`���f�[�^

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
	//�����o��
	bool Write(const char* FileName, PMX_DATA* pPmxData);
private:
	//���f�����ǂݍ���
	void ModelInfoLoad(FILE* fp, PMX_DATA* pPmxData);

	//���_�ǂݍ���
	void VertexLoad(FILE* fp, PMX_DATA* pPmxData);
	
	//�ʓǂݍ���
	void FaceLoad(FILE* fp, PMX_DATA* pPmxData);
	
	//�e�N�X�`���ǂݍ���
	void TexLoad(FILE* fp, PMX_DATA* pPmxData);
};