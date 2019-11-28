#pragma once
#include "Singleton.h"

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

//pmx���_�f�[�^
struct PMX_VERTEX
{
	~PMX_VERTEX()
	{
		delete[] m_pfAddUv;
		m_pfAddUv = nullptr;
	}

	float m_fPos[3];
	float m_fNorm[3];
	float m_fUv[2];
	float* m_pfAddUv;	//�ǉ�UV

	unsigned char WeightType;//�E�F�C�g�ό`����
};

//pmx�f�[�^
struct PMX_DATA
{

	~PMX_DATA()
	{
		delete m_pVertex;
		m_pVertex = nullptr;
	}

	PMX_HEADER m_Head;//�w�b�_�[
	int VerNum;//���_��
	PMX_VERTEX* m_pVertex;
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
private:
	
};
