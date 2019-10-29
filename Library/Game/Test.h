#pragma once

//ライブラリファイル
#include "../Library/CObj.h"
#include "../Library/LibraryHeader.h"

#include "../Library/XLoader.h"
#include "../Library/XLoader_Skin.h"

//テストクラス
class CTest :public CObj3DBase
{
public:
	CTest(D3DXVECTOR3 vPos, D3DXVECTOR3 vAngle, D3DXVECTOR3 vScale);
	void Init();
	void Update();
	void Draw();
	void Release();

private:

	SKIN_MESH m_SkinMesh;
	MY_MESH m_Mesh;
	int m_Frame;
};