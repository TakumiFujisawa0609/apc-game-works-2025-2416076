#include "../../Application.h"

#include "../../Utility/Utility.h"

#include "Stage.h"

Stage::Stage(void)
{
}

Stage::~Stage(void)
{
}

void Stage::Init(void)
{
	modelId_ = MV1LoadModel("Data/Model/Stage/Stage.mv1");

	pos_ = { 1238.0f, 0.0f, 1900.0f };
	MV1SetPosition(modelId_, pos_);

	scale_ = { 50.0f, 50.0f, 50.0f };
	MV1SetScale(modelId_, scale_);

	angle_ = { 0.0f, Utility::Deg2RadF(180.0f), 0.0f };
	MV1SetRotationXYZ(modelId_, angle_);

	// è’ìÀîªíËèäñ@ÇÃç\íz
	MV1SetupCollInfo(modelId_, -1);
}

void Stage::Update(void)
{
}

void Stage::Draw(void)
{
	MV1DrawModel(modelId_);
}

void Stage::Release(void)
{
	MV1DeleteModel(modelId_);
}

int Stage::GetModelId(void)
{
	return modelId_;
}