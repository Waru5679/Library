#include "../Library/CObj.h"
#include "../Library/Hit.h"
#include <vector>
using namespace std;

//球クラス
class CSphere:public CObj3D
{
public:
	//コンストラクタ
	CSphere(D3DXVECTOR3 vPos, D3DXVECTOR3 vAngle, D3DXVECTOR3 vScale);

	void Init();	//初期化
	void Update();	//更新
	void Draw();	//描画
private:
	float m_fRadius;	//球の半径
	SphereData m_SphereData;
};
