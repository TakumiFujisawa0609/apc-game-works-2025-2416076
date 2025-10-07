#include "MatrixUtility.h"

MATRIX MatrixUtility::GetMatrixRotateXYZ(const VECTOR& euler)
{
    MATRIX ret = MGetIdent();
    ret = MMult(ret, MGetRotX(euler.x));
	ret = MMult(ret, MGetRotY(euler.y));
    ret = MMult(ret, MGetRotZ(euler.z));

    return ret;
}

MATRIX MatrixUtility::Multiplication(const MATRIX& child, const MATRIX& parent)
{
    MATRIX ret = MMult(child, parent);

    return ret;
}

MATRIX MatrixUtility::Multiplication(const VECTOR& child, const VECTOR& parent)
{
	MATRIX ret = Multiplication(GetMatrixRotateXYZ(child), GetMatrixRotateXYZ(parent));

    return ret;
}
