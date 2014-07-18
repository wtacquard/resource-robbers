#ifndef MOREMATH_H_
# define MOREMATH_H_

#include "DXUT.h"

inline float radiansToDegrees(float rad);
D3DXVECTOR3* matrixMultiplyByVector(D3DXVECTOR3* result, const D3DXMATRIX* m, D3DXVECTOR3* v);
D3DXQUATERNION* quaternionMultiplyByVector(D3DXQUATERNION* result, D3DXQUATERNION* q, D3DXVECTOR3* v);
D3DXVECTOR3* quaternionToEulerAngles(D3DXVECTOR3* result, const D3DXQUATERNION* q);
D3DXVECTOR3* quaternionVectorRotate(D3DXVECTOR3* result, const D3DXQUATERNION* q, const D3DXVECTOR3* v);

float distance(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2);

#endif //!MOREMATH_H_