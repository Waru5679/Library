#pragma once

#include "../Library/LibraryHeader.h"
#include "../Library/CObj.h"

#include "../Library/VmdLoader.h"
#include "..//Library/PmxLoader.h"

//VmdƒeƒXƒg
class CVmdTest:public CObj3DBase
{
public:
	CVmdTest(D3DXVECTOR3 vPos, D3DXVECTOR3 vAngle, D3DXVECTOR3 vScale);
	void Init();
	void Update();
	void Draw();
	void Release();
private:
	VMD_DATA m_data;
	PMX_DATA m_PmxData;
};