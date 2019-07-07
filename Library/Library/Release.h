#pragma once
#include <vector>
using namespace std;
//
////解放クラス
//class CRelease
//{
//private:
//	CRelease() {};	//コンストラクタ
//	~CRelease() {};	//デストラクタ
//
//	//インスタンス
//	static CRelease* m_pInstance;
//public:
//	//インスタンス取得
//	static CRelease* GetInstance()
//	{
//		//インスタンス化されてなければインスタンス化
//		if (m_pInstance == nullptr)
//		{
//			m_pInstance = new CRelease();
//		}
//		return m_pInstance;
//	}
//
//	//解放
//	void Release();
//
//	////ベクター解放
//	//template <typename T>
//	//void VectorRelease(vector<T>& vec);
//
//	////ポインタ解放
//	//void PointerRelease(void* p);
//private:
//};

//ベクター解放
template <typename T>
static void VectorRelease(vector<T>& vec)
{
	vec.shrink_to_fit();
	vec.resize(0);
}

//ポインタ解放
static void PointerRelease(void* p)
{
	delete p;
	p = nullptr;
}
