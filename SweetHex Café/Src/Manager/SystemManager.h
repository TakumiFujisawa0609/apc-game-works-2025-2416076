#pragma once
#include <vector>

class SystemManager
{
public:
	// 明示的にインステンスを生成する
	static void CreateInstance(void);

	// 静的インスタンスの取得
	static SystemManager& GetInstance(void);

	// 解放処理
	void Destroy(void);

	//リセット
	void ResetGame(void);

	//人数を格納
	void SetPlayerNum(const int num) { playerNum_ = num; }

	//人数を返す
	const int GetPlayerNum() const { return playerNum_; }

	//スコアを格納
	void SetScore(int score) { score_ += score; }

	//スコアを返す
	const int GetScore() const { return score_; }

private:

	// 静的インスタンス
	static SystemManager* instance_;

	//プレイヤー人数
	int playerNum_;

	//スコア
	int score_;

	// デフォルトコンストラクタをprivateにして、
	// 外部から生成できない様にする
	SystemManager(void);
	// コピーコンストラクタも同様
	SystemManager(const SystemManager& manager) = default;
	// デストラクタも同様
	~SystemManager(void) = default;
};