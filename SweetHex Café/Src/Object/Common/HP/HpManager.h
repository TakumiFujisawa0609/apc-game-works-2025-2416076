#pragma once
#include <vector>
#include <DxLib.h>

class HpManager
{
public:

	enum class TYPE
	{
		UI,       // 画面固定表示
		WORLD     // ワールド空間から変換
	};

	struct HP
	{
		int x;
		int y;
		int z;
		float scale;
		bool isAlive;
	};

	static constexpr float HP_SCALE_SPEED = 0.02f;

	// コンストラクタ
	HpManager(VECTOR pos, int hp, int maxHp, float size, int width, TYPE type);

	// デストラクタ
	~HpManager(void);

	void Init(void);
	void Load(void);
	void LoadEnd(void);
	void Update(void);
	void Draw(void);
	void Release(void);

	void SetHp(int newHp);
	void SetPos(const VECTOR& pos) { pos_ = pos; }

private:
	// ＨＰアイコン画像ハンドル
	int imgHeart_;

	// 失ったＨＰアイコン画像ハンドル
	int imgHeartLost_;

	std::vector<HP> hps_;

	VECTOR pos_;
	int maxHp_;
	int hp_;
	float size_;
	TYPE type_;
	int iconWidth_;

	void InitHp(void);
};