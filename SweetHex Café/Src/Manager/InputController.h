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
	bool IsMoveUp(void);
	bool IsMoveDown(void);
	bool IsMoveLeft(void);
	bool IsMoveRight(void);
	bool IsDash(void);

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

