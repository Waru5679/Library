#include "Release.h"

////�C���X�^���X��
//CRelease* CRelease::m_pInstance = nullptr;
//
////�x�N�^�[�̉��
//template <typename T>
//void VectorRelease(vector<T>& vec)
//{
//	vec.shrink_to_fit();
//	vec.resize(0);
//}
//
////�|�C���^���
//void PointerRelease(void* p)
//{
//	delete p;
//	p = nullptr;
//}
//
////���
//void CRelease::Release()
//{
//	//�C���X�^���X�j��
//	PointerRelease(m_pInstance);
//}
