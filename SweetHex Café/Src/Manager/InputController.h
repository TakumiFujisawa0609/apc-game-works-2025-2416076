#pragma once

class InputController
{
public:

	// インスタンスを明示的に生成
	static void CreateInstance(void);

	// インスタンスの取得
	static InputController& GetInstance(void);

	// リソースの破棄
	void Destroy(void);

	// ゲームで使用するアクションを登録

	// 上下左右
	bool IsUp(void);
	bool IsDown(void);
	bool IsLeft(void);
	bool IsRight(void);

	// ダッシュ
	bool IsDash(void);

	// 攻撃
	bool IsAttack(void);

	// 使用
	bool IsUse(void);

	// 決定
	bool IsDecide(void);

private:
	// シングルトン用インスタンス
	static InputController* instance_;

	// デフォルトコンストラクタをprivateにして、
	// 外部から生成できない様にする
	InputController(void);

	// コピーコンストラクタも同様
	InputController(const InputController& instance) = default;

	// デストラクタも同様
	~InputController(void) = default;

};

