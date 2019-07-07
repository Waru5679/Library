#include "Release.h"

////インスタンス化
//CRelease* CRelease::m_pInstance = nullptr;
//
////ベクターの解放
//template <typename T>
//void VectorRelease(vector<T>& vec)
//{
//	vec.shrink_to_fit();
//	vec.resize(0);
//}
//
////ポインタ解放
//void PointerRelease(void* p)
//{
//	delete p;
//	p = nullptr;
//}
//
////解放
//void CRelease::Release()
//{
//	//インスタンス破棄
//	PointerRelease(m_pInstance);
//}
