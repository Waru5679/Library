#pragma once

//���C�u�����t�@�C��
#include "../Library/CObj.h"
#include "../Library/LibraryHeader.h"

//TitleObj
class CTitleObj :public CObj2DBase
{
public:
	void Init();	//������
	void Update();	//�X�V
	void Draw();	//�`��
	void Release() {};	//�J��
private:
};