#pragma once
#include <DxLib.h>
#include "WeaponBase.h"

class WeaponPunch : public WeaponBase
{
public:

	// コンストラクタ
	WeaponPunch(void);

	// デストラクタ
	~WeaponPunch(void);

	void Draw(void) override;
	void Release(void) override;

	// 武器を使用する
	void Use(VECTOR pos, VECTOR dir) override;

	// パンチの攻撃力を返す
	int GetPower(void) const override { return PUNCH_DAMEGE;  }

protected:
	// 画像やモデルなどのロード
	void Load(void) override;

	// パラメータ設定
	void SetParam(void) override;

private:

	// パンチの攻撃力
	static constexpr int PUNCH_DAMEGE = 1;
};