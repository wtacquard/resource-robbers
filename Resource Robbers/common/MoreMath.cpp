#include "common/MoreMath.h"

D3DXVECTOR3* matrixMultiplyByVector(D3DXVECTOR3* result, const D3DXMATRIX* m, D3DXVECTOR3* v)
{
	if(result == v)
	{
		D3DXVECTOR3 temp;
		temp.x = m->_11 * v->x + m->_12 * v->y + m->_13 * v->z;
		temp.y = m->_21 * v->x + m->_22 * v->y + m->_23 * v->z;
		temp.z = m->_31 * v->x + m->_32 * v->y + m->_33 * v->z;
		*result = temp;
	}
	else
	{
		result->x = m->_11 * v->x + m->_12 * v->y + m->_13 * v->z;
		result->y = m->_21 * v->x + m->_22 * v->y + m->_23 * v->z;
		result->z = m->_31 * v->x + m->_32 * v->y + m->_33 * v->z;
	}
	return result;
}

D3DXQUATERNION* quaternionMultiplyByVector(D3DXQUATERNION* result, D3DXQUATERNION* q, D3DXVECTOR3* v)
{
	if(result == q)
	{
		D3DXQUATERNION temp;
		temp.w = -(q->x * v->x + q->y * v->y + q->z * v->z);
		temp.x =   q->w * v->x + q->y * v->z + q->z * v->y;
		temp.y =   q->w * v->y + q->z * v->x + q->x * v->z;
		temp.y =   q->w * v->z + q->x * v->y + q->y * v->x;
		*result = temp;
	}
	else
	{
		result->w = -(q->x * v->x + q->y * v->y + q->z * v->z);
		result->x =   q->w * v->x + q->y * v->z + q->z * v->y;
		result->y =   q->w * v->y + q->z * v->x + q->x * v->z;
		result->y =   q->w * v->z + q->x * v->y + q->y * v->x;
	}
	return result;
}

D3DXVECTOR3* quaternionToEulerAngles(D3DXVECTOR3* result, const D3DXQUATERNION* q)
{
	double w = q->w;
	double x = q->x;
	double y = q->y;
	double z = q->z;
	double r11, r21, r31, r32, r33, r12, r13;
	double q00, q11, q22, q33;
	double tmp;
	D3DXVECTOR3 u;
	q00 = w * w;
	q11 = x * x;
	q22 = y * y;
	q33 = z * z;
	r11 = q00 + q11 - q22 - q33;
	r21 = 2 * (x * y + w * z);
	r31 = 2 * (x * z - w * y);
	r32 = 2 * (y * z + w * x);
	r33 = q00 - q11 - q22 + q33;

	tmp = fabs(r31);
	if(tmp > 0.9999999)
	{
		r12 = 2 * (x * y - w * z);
		r13 = 2 * (x * z + w * y);
		
		result->x = radiansToDegrees(0.0f);
		result->y = radiansToDegrees((float)(-(D3DX_PI/2) * r31/tmp));
		result->z = radiansToDegrees((float)atan2(-r12, -r31*r13));
	}
	else
	{
		result->x = radiansToDegrees((float)atan2(r32, r33));
		result->y = radiansToDegrees((float)asin(-r31));
		result->z = radiansToDegrees((float)atan2(r21, r11));
	}
	return result;
}

D3DXVECTOR3* quaternionVectorRotate(D3DXVECTOR3* result, const D3DXQUATERNION* pQ, const D3DXVECTOR3* pV)
{
	D3DXQUATERNION inverse, rotated;
	D3DXQuaternionInverse(&inverse, pQ);
	//rotated = *pQ * *pV * inverse;
	result->x = rotated.x;
	result->y = rotated.y;
	result->z = rotated.z;
	return result;
}

inline float radiansToDegrees(float rad)
{
	return rad * 180.f / D3DX_PI;
}

float
distance(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2)
{
	float x = pos2.x - pos1.x;
	float y = pos2.y - pos1.y;
	float z = pos2.z - pos1.z;

	return (sqrt(x*x + y*y + z*z));
}