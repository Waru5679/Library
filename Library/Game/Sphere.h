#include "../Library/CObj.h"
#include "../Library/Hit.h"
#include <vector>
using namespace std;

//���N���X
class CSphere:public CObj3D
{
public:
	//�R���X�g���N�^
	CSphere(D3DXVECTOR3 vPos, D3DXVECTOR3 vAngle, D3DXVECTOR3 vScale);

	void Init();	//������
	void Update();	//�X�V
	void Draw();	//�`��
private:
	float m_fRadius;	//���̔��a
	SphereData m_SphereData;
};
