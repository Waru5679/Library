#pragma once

#include"Singleton.h"

//�w�b�_�[
struct VMD_HEAD
{
	char m_Signature[30];	//�V�O�l�`��
	char m_ModelName[20];	//���f����
};

//���[�V����
struct VMD_MOTION
{
	char	BoneName[15];	//�{�[����
	int		FrameNum;		//�t���[��No
	float	Pos[3];			//�ʒu
	float	Rot[4];			//��]
	unsigned char Data[64];	//�⊮�f�[�^
};

//�X�L��
struct VMD_SKIN
{
	char SkinName[15];	//�X�L����
	int FlameNum;		//�t���[��No
	float Weight;		//�E�F�C�g
};

//VMD�f�[�^
struct VMD_DATA
{
	VMD_HEAD		m_Head;				//�w�b�_�[
	unsigned int	m_MotionRecordNum;	//���[�V������
	VMD_MOTION*		m_pMotion;			//���[�V�������X�g
	unsigned int	m_SkinRecordNum;	//�X�L����
	VMD_SKIN*		m_pSkin;			//�X�L�����X�g
};

//VMD�ǂݍ���
class CVmdLoader:public CSingleton<CVmdLoader>
{
private:
	//�V���O���g��
	friend CSingleton<CVmdLoader>;
	CVmdLoader() {};
	~CVmdLoader() {};
public:
	//�ǂݍ���
	bool Load(const char* FileName, VMD_DATA* pVmdData);
	//�����o��
	bool Write(const char* FileName, VMD_DATA* pVmdData);
private:
};
