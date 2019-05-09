#pragma once
#include "CObj.h"

//’¬ƒNƒ‰ƒX
class CStreet :public CObj3D
{
public:
	CStreet(D3DXVECTOR3 vPos, D3DXVECTOR3 vAngle, D3DXVECTOR3 vScale);
	void Init();
	void Update();
	void Draw();
private:

};