#include "BlockManager.h"
#include "Block.h"

Block::Block(void)
{
}

Block::~Block(void)
{
}

void Block::Create(TYPE type, int baseModelId, int mapX, int mapY, int mapZ, float angle, int posY)
{
    // ブロックの種類
    type_ = type;

    // モデルのハンドルID
    modelId_ = MV1DuplicateModel(baseModelId);

    angle_ = angle;

    // 色の調整(自己発光)
    MV1SetMaterialEmiColor(modelId_, 0, COLOR_EMI);

    // 1ブロックあたりの大きさ
    const float SIZE_BLOCK = BlockManager::SIZE_BLOCK;

    // 1ブロックあたりの半分の大きさ
    const float SIZE_HALF_BLOCK = (SIZE_BLOCK / 2.0f);

    // 引数で指定されたマップ座標から座標を計算
    float x = static_cast<float>(mapX);
    float y = static_cast<float>(mapY);
    float z = static_cast<float>(mapZ);
    VECTOR pos = VGet(
        x * SIZE_BLOCK + SIZE_HALF_BLOCK,
        y + posY,
        z * SIZE_BLOCK + SIZE_HALF_BLOCK
    );

    // 座標設定
    pos_ = pos;
    MV1SetPosition(modelId_, pos_);

    // 回転設定
    MV1SetRotationXYZ(modelId_, VGet(0.0f, DX_PI_F * (angle_ / 180.0f), 0.0f));

    // 大きさ設定
    scales_ = SCALES;
    MV1SetScale(modelId_, scales_);

    // 衝突判定（モデルの全て）の前準備を行う
    MV1SetupCollInfo(modelId_, -1);

    switch (type)
    {
    case TYPE::CASH_REGISTER:
        // レジ
        tag_ = "CASH_REGISTER";
        break;
    case TYPE::DISPLAY_CASE:
        // 提供カウンター
        tag_ = "DISPLAY_CASE";
        break;
    case TYPE::FRIDGE:
        // 冷蔵庫
        tag_ = "FRIDGE";
        break;
    default:
        tag_ = "None";
        break;
    }
}

void Block::Update(void)
{
}

void Block::Draw(void)
{
    MV1DrawModel(modelId_);
}

void Block::Release(void)
{
    MV1DeleteModel(modelId_);
}
