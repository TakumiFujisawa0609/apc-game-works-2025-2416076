#pragma once
#include <DxLib.h>

class MatrixUtility
{
public:
	// オイラー角から回転行列XYZ順を取得
	static MATRIX GetMatrixRotateXYZ(const VECTOR& euler);

	// 親子の回転行列を豪税
	static MATRIX Multiplication(
		const MATRIX& child, const MATRIX& parent);

	// 親子のオイラー格を合成する
	static MATRIX Multiplication(
		const VECTOR& child, const VECTOR& parent);
};