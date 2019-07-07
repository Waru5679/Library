#pragma once
#include <vector>
using namespace std;
//
////����N���X
//class CRelease
//{
//private:
//	CRelease() {};	//�R���X�g���N�^
//	~CRelease() {};	//�f�X�g���N�^
//
//	//�C���X�^���X
//	static CRelease* m_pInstance;
//public:
//	//�C���X�^���X�擾
//	static CRelease* GetInstance()
//	{
//		//�C���X�^���X������ĂȂ���΃C���X�^���X��
//		if (m_pInstance == nullptr)
//		{
//			m_pInstance = new CRelease();
//		}
//		return m_pInstance;
//	}
//
//	//���
//	void Release();
//
//	////�x�N�^�[���
//	//template <typename T>
//	//void VectorRelease(vector<T>& vec);
//
//	////�|�C���^���
//	//void PointerRelease(void* p);
//private:
//};

//�x�N�^�[���
template <typename T>
static void VectorRelease(vector<T>& vec)
{
	vec.shrink_to_fit();
	vec.resize(0);
}

//�|�C���^���
static void PointerRelease(void* p)
{
	delete p;
	p = nullptr;
}
