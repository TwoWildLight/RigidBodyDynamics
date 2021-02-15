#include "TwlMath.h"
#include <numeric>
#include <vector>

namespace math
{
	Vector::Vector(float x, float y, float z, float w)
		:
		v(DirectX::XMVectorSet(x, y, z, w))
	{}

	Vector::Vector(const Vector& v)
		:
		v(v.v)
	{}

	Vector::Vector(const DirectX::XMFLOAT3& f3)
		:
		Vector(f3.x, f3.y, f3.z, 0.0f)
	{}

	Vector::Vector(const DirectX::XMVECTOR& v)
		:
		v(v)
	{}

	Vector Vector::operator-() const
	{
		return DirectX::operator-(v);
	}

	Vector& Vector::operator=(const Vector& v)
	{
		this->v = v.v;
		return *this;
	}

	Vector& Vector::operator=(const DirectX::XMFLOAT3& f3)
	{
		this->v = DirectX::XMVectorSet(f3.x, f3.y, f3.z, 1.0f);
		return *this;
	}

	Vector& Vector::operator=(const DirectX::XMVECTOR& v)
	{
		this->v = v;
		return *this;
	}

	Vector Vector::operator+(const Vector& v) const
	{
		return DirectX::operator +(this->v, v.v);
	}

	Vector Vector::operator+(const DirectX::XMFLOAT3& f3) const
	{
		return DirectX::operator +(this->v, Vector(f3).v);
	}

	Vector Vector::operator+(const DirectX::XMVECTOR& v) const
	{
		return DirectX::operator +(this->v, v);
	}

	Vector& Vector::operator+=(const Vector& v)
	{
		return *this = *this + v;
	}

	Vector& Vector::operator+=(const DirectX::XMFLOAT3& f3)
	{
		return *this = *this = f3;
	}

	Vector& Vector::operator+=(const DirectX::XMVECTOR& v)
	{
		DirectX::operator +=(this->v, v); return *this;
	}

	Vector Vector::operator-(const Vector& v) const
	{
		return DirectX::operator -(this->v, v.v);
	}

	Vector Vector::operator-(const DirectX::XMFLOAT3& f3) const
	{
		return DirectX::operator -(this->v, Vector(f3).v);
	}

	Vector Vector::operator-(const DirectX::XMVECTOR& v) const
	{
		return DirectX::operator -(this->v, v);
	}

	Vector& Vector::operator-=(const Vector& v)
	{
		return *this = *this - v;
	}

	Vector& Vector::operator-=(const DirectX::XMFLOAT3& f3)
	{
		return *this = *this - f3;
	}

	Vector& Vector::operator-=(const DirectX::XMVECTOR& v)
	{
		return *this = *this - v;
	}

	Vector Vector::operator*(float fScalar) const
	{
		return DirectX::operator *(this->v, fScalar);
	}

	Vector& Vector::operator*=(float fScalar)
	{
		DirectX::operator *=(this->v, fScalar); return *this;
	}

	Vector Vector::operator/(float fScalar) const
	{
		return DirectX::operator /(this->v, fScalar);
	}

	Vector& Vector::operator/=(float fScalar)
	{
		DirectX::operator /=(this->v, fScalar); return *this;
	}

	Vector Vector::operator*(const Matrix& m) const
	{
		return DirectX::XMVector3Transform(v, m.m);
	}

	Vector Vector::operator*(const DirectX::XMMATRIX& m) const
	{
		return DirectX::XMVector3Transform(v, m);
	}

	Vector Vector::operator*=(const Matrix& m)
	{
		return *this = *this * m;
	}

	Vector Vector::operator*=(const DirectX::XMMATRIX& m)
	{
		return *this = *this * m;
	}

	float& Vector::operator[](size_t uIndex)
	{
		return v.m128_f32[uIndex];
	}

	float Vector::operator[](size_t uIndex) const
	{
		return v.m128_f32[uIndex];
	}

	float Vector::Length() const
	{
		return DirectX::XMVector3Length(v).m128_f32[0];
	}

	float Vector::LengthSq() const
	{
		return DirectX::XMVector3LengthSq(v).m128_f32[0];
	}

	float Vector::Dot(const Vector& v) const
	{
		return DirectX::XMVector3Dot(this->v, v.v).m128_f32[0];
	}

	float Vector::Dot(const DirectX::XMFLOAT3& f3) const
	{
		return DirectX::XMVector3Dot(this->v, Vector(f3).v).m128_f32[0];
	}

	float Vector::Dot(const DirectX::XMVECTOR& v) const
	{
		return DirectX::XMVector3Dot(this->v, v).m128_f32[0];
	}

	Vector Vector::Cross(const Vector& v) const
	{
		return DirectX::XMVector3Cross(this->v, v.v);
	}

	Vector Vector::Cross(const DirectX::XMFLOAT3& f3) const
	{
		return DirectX::XMVector3Cross(this->v, Vector(f3).v);
	}

	Vector Vector::Cross(const DirectX::XMVECTOR& v) const
	{
		return DirectX::XMVector3Cross(this->v, v);
	}

	Vector& Vector::Normalize()
	{
		return *this = DirectX::XMVector3Normalize(v);
	}

	Vector Vector::Normalize(const Vector& v)
	{
		return DirectX::XMVector3Normalize(v.v);
	}

	Vector Vector::Normalize(const DirectX::XMFLOAT3& f3)
	{
		return DirectX::XMVector3Normalize(Vector(f3).v);
	}

	Vector Vector::Normalize(const DirectX::XMVECTOR& v)
	{
		return DirectX::XMVector3Normalize(v);
	}

	Vector& Vector::Normalize2d()
	{
		return *this = DirectX::XMVector2Normalize(v);
	}

	Vector Vector::Normalize2d(const Vector& v)
	{
		return DirectX::XMVector2Normalize(v.v);
	}

	Vector Vector::Normalize2d(const DirectX::XMFLOAT3& f3)
	{
		return DirectX::XMVector2Normalize(Vector(f3).v);
	}

	Vector Vector::Normalize2d(const DirectX::XMVECTOR& v)
	{
		return DirectX::XMVector2Normalize(v);
	}

	Vector Vector::Slerp(const Vector& q1, const Vector& q2, float fAlpha)
	{
		return DirectX::XMQuaternionSlerp(q1.v, q2.v, fAlpha);
	}

	Matrix::Matrix()
		:
		m(DirectX::XMMatrixIdentity())
	{}

	Matrix::Matrix(const DirectX::XMMATRIX& m)
		:
		m(m)
	{}

	Matrix& Matrix::operator=(const Matrix& m)
	{
		this->m = m.m;
		return *this;
	}

	Matrix& Matrix::operator=(const DirectX::XMMATRIX& m)
	{
		this->m = m;
		return *this;
	}

	Matrix Matrix::operator*(const Matrix& m) const
	{
		return this->m * m.m;
	}

	Matrix Matrix::operator*(const DirectX::XMMATRIX& m) const
	{
		return this->m * m;
	}

	Matrix& Matrix::operator*=(const Matrix& m)
	{
		return *this = *this * m;
	}

	Matrix& Matrix::operator*=(const DirectX::XMMATRIX& m)
	{
		return *this = *this * m;
	}

	Matrix Matrix::Identity()
	{
		return DirectX::XMMatrixIdentity();
	}

	Matrix& Matrix::Transpose()
	{
		m = DirectX::XMMatrixTranspose(m);
		return *this;
	}

	Matrix Matrix::Transpose(const Matrix& m)
	{
		return DirectX::XMMatrixTranspose(m.m);
	}

	Matrix Matrix::Transpose(const DirectX::XMMATRIX& m)
	{
		return DirectX::XMMatrixTranspose(m);
	}

	Matrix Matrix::Scaling(float x, float y, float z)
	{
		return DirectX::XMMatrixScaling(x, y, z);
	}

	Matrix Matrix::Scaling(const Vector& v)
	{
		return DirectX::XMMatrixScalingFromVector(v.v);
	}

	Matrix Matrix::Rotation(float x, float y, float z)
	{
		return DirectX::XMMatrixRotationRollPitchYaw(x, y, z);
	}

	Matrix Matrix::Rotation(const Vector& v)
	{
		return DirectX::XMMatrixRotationRollPitchYawFromVector(v.v);
	}

	Matrix Matrix::RotationAxis(const Vector& v, float fAng)
	{
		return DirectX::XMMatrixRotationAxis(v.v, fAng);
	}

	Matrix Matrix::RotationQuaternion(const Vector& quaternion)
	{
		return DirectX::XMMatrixRotationQuaternion(quaternion.v);
	}

	Matrix Matrix::Translation(float x, float y, float z)
	{
		return DirectX::XMMatrixTranslation(x, y, z);
	}

	Matrix Matrix::Translation(const Vector& v)
	{
		return DirectX::XMMatrixTranslationFromVector(v.v);
	}

	Matrix Matrix::LookAt(const Vector& vPos, const Vector& vDir, const Vector& vUp)
	{
		return DirectX::XMMatrixLookAtLH(vPos.v, (vPos + vDir).v, vUp.v);
	}

	Matrix Matrix::PerspectiveFov(float fHFov, float fAR, float fN, float fF)
	{
		return DirectX::XMMatrixPerspectiveFovLH(fHFov, fAR, fN, fF);
	}

	float WrapAngle(float fValue)
	{
		float twoPi = PI_F * 2.0f;
		float mod = fmodf(fValue, twoPi);
		if (mod > PI_F)
		{
			fValue = mod - twoPi;
		}
		else if (mod < -PI_F)
		{
			fValue = mod + twoPi;
		}
		return fValue;
	}
}